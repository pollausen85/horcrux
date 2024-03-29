#include "client.hpp"
#include "commands.hpp"
#include <base64pp/base64pp.h>
#include "ChunkCreator.hpp"
#include <vector>
#include <fstream>
#include "Utils.hpp"
#include "Defs.hpp"

template<class T>
Client<T>::Client(boost::asio::io_context& io_context, 
                  std::shared_ptr<ISPlitter<T>> const chunker)
    :m_socket(io_context)
    ,m_resolver(io_context)
    ,m_chunker(chunker)
{
}

template<class T>
bool Client<T>::connect(const std::string& serverAddress, const std::string& serverPort)
{
    boost::system::error_code err;
    boost::asio::connect(m_socket, m_resolver.resolve(serverAddress,serverPort), err);
    if(err)
    {
        std::cout << "Error connecting to the server\n";
        return false;
    }

    return true;
}

template<class T>
void Client<T>::disconnect()
{
    boost::system::error_code ec;
    m_socket.shutdown(boost::asio::ip::tcp::socket::shutdown_send, ec);
    m_socket.close();
}

template<class T>
bool Client<T>::sendSaveCommand(const uint32_t chunkCount, const std::string& fileName, std::string& strUUID)
{
    std::vector<Chunk> chunks; 
    bool res = m_chunker->split(chunkCount, fileName, chunks);
    if(!res)
    {
        return false;
    }
    
    SaveCommand sc;
    sc.commandName = "save";
    sc.data.uuid = boost::uuids::random_generator()();
    sc.data.totalCount = chunkCount;
    json j;
    for(size_t i = 0; i < chunks.size(); ++i)
    {
        sc.data.index = i;
        sc.data.payload = chunks[i].payload;
        sc.data.payloadSize = chunks[i].payloadSize;
        sc.data.checksum = chunks[i].checksum;

        j = json(sc);

        std::string jsonStr = j.dump() + '\n';
        // Send data in chunks
        for (size_t pos = 0; pos < jsonStr.size();)
        {
            size_t chunkSize = std::min<size_t>(jsonStr.size() - pos, MAX_CHUNK_SIZE);
            boost::asio::write(m_socket, boost::asio::buffer(jsonStr.data() + pos, chunkSize));
            pos += chunkSize;
        }

        waitForResponseSave();
    }

    strUUID = boost::uuids::to_string(sc.data.uuid);

    return true;
}

template<class T>
bool Client<T>::sendLoadCommand(const std::string& strUUID, const std::string& filename)
{
    LoadCommand lc;
    lc.commandName = "load";
    lc.uuid = boost::lexical_cast<boost::uuids::uuid>(strUUID);

    const json j = lc;

    boost::asio::write(m_socket, boost::asio::buffer(j.dump() + '\n'));

    waitForRequestOrResponse(filename);

    return true;
}

template<class T>
void Client<T>::waitForResponseSave()
{
    boost::system::error_code error;
    // Async read to handle the server's response
    boost::asio::read_until(m_socket, m_buffer, '\n', error);
    if (!error) 
    {
        std::string data
        {
            std::istreambuf_iterator<char>(&m_buffer), 
            std::istreambuf_iterator<char>() 
        };

        std::cout << "Received: " << data << std::endl;
    }
    else 
    {
        std::cerr << "Error reading data: " << error.message() << std::endl;
    }
}

template<class T>
void Client<T>::processData(const std::string& data, const std::string& filename, 
                            uint32_t& index, uint32_t& total)
{
    try
    {
        json j = json::parse(data);
        auto lr = j.get<LoadResponse>();
        if(lr.code == (int)ErrCode::NoError)
        {
            if(Utils::checkCRCcorrectness(lr.data.payload, 
                                          std::shared_ptr<IChecksum>(std::dynamic_pointer_cast<ChunkCreator>(m_chunker)->getChecksumCreator()->shared_from_this()), 
                                          lr.data.checksum))
            {      
                index = lr.data.index;
                total = lr.data.totalCount;

                std::ofstream file(filename, std::ios::binary | std::ios::app);
                if(file.is_open() && file.good())
                {
                    std::vector<uint8_t> binaryData = base64pp::decode(lr.data.payload).value_or(std::vector<uint8_t>());
                    file.write(reinterpret_cast<char*>(binaryData.data()), binaryData.size());
                    file.close();
                }
            }
            else
            {
                std::cerr << "Error " << lr.code << ": " << Utils::errorCodeToStr(lr.code) << std::endl;    
            }
        }
        else
        {
            std::cerr << "Error " << lr.code << ": " << Utils::errorCodeToStr(lr.code) << std::endl;    
        }
    }
    catch (const json::parse_error& e) 
    {
        std::cerr << "JSON parsing error: " << e.what() << std::endl;
    }
}

template<class T>
void Client<T>::waitForRequestOrResponse(const std::string& filename)
{
    uint32_t index, total;
    boost::system::error_code error;
    // Async read to handle the server's response
    boost::asio::read_until(m_socket, m_buffer, '\n', error);
    if (!error) 
    {
        std::string data
        {
            std::istreambuf_iterator<char>(&m_buffer), 
            std::istreambuf_iterator<char>() 
        };

        std::cout << "Received: " << data << std::endl;

        Utils::waitForCompleteLoadMessage<Client<T>>(data, 
                                                     std::shared_ptr<Client<T>>(this->shared_from_this()),
                                                     m_strBuf, filename);
    }
    else 
    {
        std::cerr << "Error reading data: " << error.message() << std::endl;
    }
}

template Client<Chunk>::Client(boost::asio::io_context& io_context, 
                               std::shared_ptr<ISPlitter<Chunk>> const chunker);
template bool Client<Chunk>::connect(const std::string& serverAddress, const std::string& serverPort);
template void Client<Chunk>::disconnect();
template bool Client<Chunk>::sendSaveCommand(const uint32_t chunkCount, const std::string& fileName, std::string& strUUID);
template bool Client<Chunk>::sendLoadCommand(const std::string& strUUID, const std::string& filename);