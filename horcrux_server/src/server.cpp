#include "server.hpp"
#include "commands.hpp"
#include <fstream>

#define MAX_CHUNK_SIZE 4096

void server::do_accept(const std::string& directory) 
{
    m_acceptor.async_accept([this, &directory](boost::system::error_code ec, tcp::socket socket) 
    {
        if (!ec) 
        {
            std::cout << "creating session on: " 
                << socket.remote_endpoint().address().to_string() 
                << ":" << socket.remote_endpoint().port() << '\n';

            
            std::make_shared<session>(std::move(socket), m_storer)->run();
        } 
        else 
        {
            std::cout << "error: " << ec.message() << std::endl;
        }
        do_accept(directory);
    });
}

void session::wait_for_request() 
{
    auto self(shared_from_this());
    boost::asio::async_read_until(m_socket, m_buffer, '\n', 
    [this, self](boost::system::error_code ec, std::size_t /*length*/)
    {
        if (!ec)  
        {
            std::string data
            {
                std::istreambuf_iterator<char>(&m_buffer), 
                std::istreambuf_iterator<char>() 
            };
            
            wait_for_complete_message(data);
        } 
        else 
        {
            std::cout << "error: " << ec << std::endl;
        }
    });
}

void session::wait_for_complete_message(const std::string& data)
{
    // Append the received data to the buffer
    m_strBuf += data;

    // Check if the buffer contains a complete message (ends with a newline character)
    size_t newlinePos;
    while ((newlinePos = m_strBuf.find('\n')) != std::string::npos)
    {
        // Extract the complete message
        std::string completeMessage = m_strBuf.substr(0, newlinePos);
        
        // Remove the processed message from the buffer
        std::string remainingData = m_strBuf.substr(newlinePos + 1);
        m_strBuf = remainingData;

        // Process the complete message
        manage_command(completeMessage);
    }
}


void session::manage_command(const std::string& data)
{
    std::cout << "Received data: " << data << std::endl;
    
    try 
    {
        auto j = json::parse(data);
        std::string commandName;
        j.at("command").get_to(commandName);

        if (commandName == "save")
        {
            CommandData sc;
            JsonFromCommandData(j, sc);

            StatusData sd;
            sd.commandName = sc.commandName;
            sd.code = (int)ErrCode::NoError;
            if(!m_storer->save(sc.uuid, data, sc.index))
            {
                sd.code = (int)ErrCode::ErrSavingFile;
            }

            json resp;
            StatusDataToJson(resp, sd);

            m_jsonBuf = resp.dump() + '\n';

            auto self(shared_from_this());
            boost::asio::async_write(m_socket, boost::asio::buffer(m_jsonBuf),
            [this, self](boost::system::error_code ec, std::size_t /*length*/)
            {
                if (!ec) {
                    std::cout << "Response sent" << std::endl;
                } else {
                    std::cout << "error: " << ec << std::endl;
                }
            });
        }
        else if (commandName == "load")
        {  
            LoadCommand lc;
            JsonFromLoadCommand(j, lc);
            if(!execute_load(lc.uuid))
            {

            }
        }

        wait_for_request();
    } 
    catch (const json::parse_error& e) 
    {
        std::cerr << "JSON parsing error: " << e.what() << std::endl;
        wait_for_request();
    }
}

bool session::execute_load(boost::uuids::uuid uuid)
{
    std::vector<std::string> data;
    if(m_storer->load(uuid, data))
    {
        for(size_t i = 0; i < data.size(); ++i)
        {
            std::ifstream file(data[i], std::ios::binary);
            if(file.is_open())
            {
                std::streamsize size = fs::file_size(data[i]);
                m_fileContent.resize(size + 1);
                if (file.read(m_fileContent.data(), size))
                {
                    m_fileContent.back() = '\n';
                    for (size_t pos = 0; pos < m_fileContent.size();)
                    {
                        size_t chunkSize = std::min<size_t>(m_fileContent.size() - pos, MAX_CHUNK_SIZE);
                        auto self(shared_from_this());
                        boost::asio::async_write(m_socket, boost::asio::buffer(m_fileContent.data() + pos, chunkSize),
                        [this, self, pos](boost::system::error_code ec, std::size_t /*length*/)
                        {
                            if (!ec) {
                                std::cout << "Response sent until " << pos << std::endl;
                            } else {
                                std::cout << "error: " << ec << std::endl;
                            }
                        });
                        pos += chunkSize;
                    }
                }
            }
        }
    }

    return true;
}
