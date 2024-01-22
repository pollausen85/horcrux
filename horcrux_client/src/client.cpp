#include "Client.hpp"
#include "commands.hpp"
#include <base64pp/base64pp.h>

Client::Client(boost::asio::io_context& io_context, std::shared_ptr<ChunckCreator> const chuncker)
    :m_socket(io_context)
    ,m_resolver(io_context)
    ,m_chuncker(chuncker)
{
}

bool Client::connect(const std::string& serverAddress, const std::string& serverPort)
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

void Client::disconnect()
{
    boost::system::error_code ec;
    m_socket.shutdown(boost::asio::ip::tcp::socket::shutdown_send, ec);
    m_socket.close();
}

bool Client::sendSaveCommand()
{
    CommandData sc;
    sc.commandName = "save";
    sc.uuid = boost::uuids::random_generator()();
    sc.index = 0;
    sc.totalCount = 6;
    std::string payload = "Questa è una prova";
    sc.payload = base64pp::encode_str(payload);
    sc.payloadSize = payload.size();

    json j;

    SaveCommandToJson(j, sc);

    boost::asio::write(m_socket, boost::asio::buffer(j.dump()));

    return true;
}