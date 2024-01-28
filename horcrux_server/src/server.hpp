#pragma once

#include <iostream>
#include <boost/asio.hpp>
#include <mutex>
#include <condition_variable>
#include <memory>
#include "DiskStorer.hpp"

class SaveCommand;

using boost::asio::ip::tcp;

class session : public std::enable_shared_from_this<session>
{

public:

    session(tcp::socket socket, std::shared_ptr<IStorer> const storer)  
        : m_socket(std::move(socket))
        , m_storer(storer) { }

    void run() 
    {
        waitForRequestOrResponse();
    }

private:

    friend class Utils;

    /// @brief Process the data received by the client
    /// @param data      data to be processed 
    /// @param filename  path where save the file
    void processData(const std::string& data, const std::string& /*filename*/);

    void waitForRequestOrResponse(const std::string& filename = "");

    void executeLoad(const boost::uuids::uuid& uuid);

    void executeSave(const SaveCommand& sc);

    tcp::socket m_socket;
    boost::asio::streambuf m_buffer;
    std::string m_strBuf;
    std::string m_jsonBuf;
    std::shared_ptr<IStorer> const m_storer;
    std::vector<char> m_fileContent;
};


class server
{
public:
    server(boost::asio::io_context& io_context, short port, const std::string& directory, std::shared_ptr<IStorer> const storer) 
    : m_acceptor(io_context, tcp::endpoint(tcp::v4(), port))
    , m_storer(storer)
    {
        doAccept(directory);
    }

private:

    void doAccept(const std::string& directory);
    
    tcp::acceptor m_acceptor;
    std::shared_ptr<IStorer> const m_storer;
};