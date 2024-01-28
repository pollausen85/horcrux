#pragma once

#include <iostream>
#include <boost/asio.hpp>
#include <memory>
#include "ISplitter.hpp"
using boost::asio::ip::tcp;

template<class T>
class Client : public std::enable_shared_from_this<Client<T>>
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
    /// @param strUUID     unique id generated
    /// @return 
    bool sendSaveCommand(const uint32_t chunkCount, const std::string& fileName, std::string& strUUID);

    /// @brief create and send load command to server
    /// @param strUUID   unique id of file to recover
    /// @param filename  path where save the file
    /// @return 
    bool sendLoadCommand(const std::string& strUUID, const std::string& filename);

private:
    
    friend class Utils;

    tcp::socket m_socket;
    tcp::resolver m_resolver;
    std::shared_ptr<ISPlitter<T>> m_chuncker;
    boost::asio::streambuf m_buffer;
    std::string m_strBuf;

    /// @brief Process the data received by the client
    /// @param data      data to be processed 
    /// @param filename  path where save the file
    void processData(const std::string& data, const std::string& filename, uint32_t& index, uint32_t& total);

    /// @brief Wait the server response of the save request
    void waitForResponseSave();

    /// @brief Wait the server response of the load request
    /// @param filename  path where save the file
    void waitForRequestOrResponse(const std::string& filename);
};