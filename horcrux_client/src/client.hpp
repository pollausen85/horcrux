#pragma once

#include <iostream>
#include <boost/asio.hpp>
#include <memory>
#include "ChunkCreator.hpp"
using boost::asio::ip::tcp;

class Client
{
public:

    /// @brief client constructor
    /// @param io_context    core I/O functionality for users of the asynchronous I/O objects
    /// @param serverAddress server address
    /// @param serverPort    server port
    Client(boost::asio::io_context& io_context, std::shared_ptr<ChunckCreator> const chuncker);

    bool connect(const std::string& serverAddress = "127.0.0.1", const std::string& serverPort = "25000");

    void disconnect();

    bool sendSaveCommand();

private:
    tcp::socket m_socket;
    tcp::resolver m_resolver;
    std::shared_ptr<ChunckCreator> m_chuncker;
};