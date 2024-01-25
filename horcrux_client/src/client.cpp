#include "client.hpp"
#include "commands.hpp"
#include <base64pp/base64pp.h>
#include "ChunkCreator.hpp"
#include <vector>

#define MAX_CHUNK_SIZE 4096

template<class T>
Client<T>::Client(boost::asio::io_context& io_context, std::shared_ptr<ISPlitter<T>> const chunker)
    :m_socket(io_context)
    ,m_resolver(io_context)
    ,m_chuncker(chunker)
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
bool Client<T>::sendSaveCommand(const uint32_t chunkCount, const std::string& fileName)
{
    std::vector<Chunk> chunks; 
    bool res = m_chuncker->split(chunkCount, fileName, chunks);
    if(!res)
    {
        return false;
    }
    
    CommandData sc;
    sc.commandName = "save";
    sc.uuid = boost::uuids::random_generator()();
    sc.totalCount = chunkCount;
    json j;
    for(size_t i = 0; i < chunks.size(); ++i)
    {
        sc.index = i;
        sc.payload = chunks[i].payload;
        sc.payloadSize = chunks[i].payloadSize;

        SaveCommandToJson(j, sc);

        std::string jsonStr = j.dump() + '\n';
        // Send data in chunks
        for (size_t pos = 0; pos < jsonStr.size();)
        {
            size_t chunkSize = std::min<size_t>(jsonStr.size() - pos, MAX_CHUNK_SIZE);
            boost::asio::write(m_socket, boost::asio::buffer(jsonStr.data() + pos, chunkSize));
            pos += chunkSize;
        }
    }

    return true;
}

template Client<Chunk>::Client(boost::asio::io_context& io_context, std::shared_ptr<ISPlitter<Chunk>> const chunker);
template bool Client<Chunk>::connect(const std::string& serverAddress, const std::string& serverPort);
template void Client<Chunk>::disconnect();
template bool Client<Chunk>::sendSaveCommand(const uint32_t chunkCount, const std::string& fileName);