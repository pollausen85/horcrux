#include "client.hpp"

client::client(boost::asio::io_context& io_context,
               const std::string& serverAddress, 
               const std::string& serverPort)
    :m_socket(io_context)
{
    tcp::resolver resolver(io_context);
    boost::asio::connect(m_socket, resolver.resolve(serverAddress,serverPort));
}

void client::disconnect()
{
    boost::system::error_code ec;
    m_socket.shutdown(boost::asio::ip::tcp::socket::shutdown_send, ec);
    m_socket.close();
}