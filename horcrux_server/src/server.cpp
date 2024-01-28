#include "server.hpp"
#include <fstream>
#include "Utils.hpp"
#include "commands.hpp"
#include "Defs.hpp"

void server::doAccept(const std::string& directory) 
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
        doAccept(directory);
    });
}

void session::waitForRequestOrResponse(const std::string& /*filename*/) 
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
            
            Utils::waitForCompleteMessage<session>(data, 
                                                   std::shared_ptr<session>(this->shared_from_this()), 
                                                   m_strBuf);
        } 
        else 
        {
            std::cout << "error: " << ec << std::endl;
        }
    });
}

void session::processData(const std::string& data, const std::string& /*filename*/)
{
    std::cout << "Received data: " << data << std::endl;
    
    try 
    {
        auto j = json::parse(data);
        std::string commandName;
        j.at("command").get_to(commandName);

        if (commandName == "save")
        {
            auto sc = j.get<SaveCommand>();
            executeSave(sc);
        }
        else if (commandName == "load")
        {  
            auto lc = j.get<LoadCommand>();
            executeLoad(lc.uuid);
        }

        waitForRequestOrResponse();
    } 
    catch (const json::parse_error& e) 
    {
        std::cerr << "JSON parsing error: " << e.what() << std::endl;
        waitForRequestOrResponse();
    }
}

void session::executeLoad(const boost::uuids::uuid& uuid)
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
                for (size_t pos = 0; pos < size;)
                {
                    size_t chunkSize = std::min<size_t>(size - pos, MAX_CHUNK_SIZE);
                    if((pos +chunkSize) == size)
                    {
                        m_fileContent.resize(chunkSize+1);
                        m_fileContent.back() = '\n';
                    }
                    else
                    {
                        m_fileContent.resize(chunkSize);
                    }
                    if (file.read(m_fileContent.data(), chunkSize))
                    {
                        auto self(shared_from_this());
                        boost::asio::async_write(m_socket, boost::asio::buffer(m_fileContent.data(), m_fileContent.size()),
                        [this, self, pos, chunkSize](boost::system::error_code ec, std::size_t /*length*/)
                        {
                            if (!ec) {
                                std::cout << "Response sent " << std::endl;
                            } else {
                                std::cout << "error: " << ec << std::endl;
                            }
                        });
                        pos += chunkSize;
                        file.seekg(pos);
                    }
                }
            }
        }
    }
}

void session::executeSave(const SaveCommand &sc)
{
    StatusData sd;
    sd.commandName = sc.commandName;
    sd.code = (int)ErrCode::NoError;

    LoadResponse lr;
    lr.code = 0;
    lr.data = sc.data;

    if(!m_storer->save(sc.data.uuid, json(lr).dump(), sc.data.index))
    {
        sd.code = (int)ErrCode::ErrSavingFile;
    }

    const json resp = sd;  

    m_jsonBuf = resp.dump() + '\n';

    auto self(shared_from_this());
    boost::asio::async_write(m_socket, boost::asio::buffer(m_jsonBuf.data(), m_jsonBuf.size()),
    [this, self](boost::system::error_code ec, std::size_t /*length*/)
    {
        if (!ec) {
            std::cout << "Response sent" << std::endl;
        } else {
            std::cout << "error: " << ec << std::endl;
        }
    });
}
