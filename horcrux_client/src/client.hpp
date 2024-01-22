#pragma once

#include <iostream>
#include <boost/asio.hpp>
#include <memory>
#include "ISplitter.hpp"
using boost::asio::ip::tcp;

template<class T>
class Client
{
public:

    /// @brief client constructor
    /// @param io_context core I/O functionality for users of the asynchronous I/O objects
    /// @param chuncker   interface to class containg logic for splitting file
    Client(boost::asio::io_context& io_context, std::shared_ptr<ISPlitter<T>> const chuncker);

    /// @brief connect to server
    /// @param serverAddress server address
    /// @param serverPort    server port
    /// @return true if connected, false otherwise
    bool connect(const std::string& serverAddress = "127.0.0.1", const std::string& serverPort = "25000");

    /// @brief disconnect form server
    void disconnect();

    /// @brief create and send save command to server
    /// @param chunckCount number of chunck in which the file will be splitted
    /// @param fileName    file to split
    /// @return 
    bool sendSaveCommand(const uint32_t chunkCount, const std::string& fileName);

private:
    tcp::socket m_socket;
    tcp::resolver m_resolver;
    std::shared_ptr<ISPlitter<T>> m_chuncker;
};