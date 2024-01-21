#pragma once

#include <iostream>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

class client
{
public:

    /// @brief client constructor
    /// @param io_context    core I/O functionality for users of the asynchronous I/O objects
    /// @param serverAddress server address
    /// @param serverPort    server port
    client(boost::asio::io_context& io_context, 
           const std::string& serverAddress = "127.0.0.1", 
           const std::string& serverPort = "25000");

    void disconnect();

private:
    tcp::socket m_socket;
};