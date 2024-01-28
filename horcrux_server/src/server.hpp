#pragma once

#include <iostream>
#include <boost/asio.hpp>
#include <mutex>
#include <condition_variable>
#include <memory>
#include "DiskStorer.hpp"

using boost::asio::ip::tcp;

class session : public std::enable_shared_from_this<session>
{

public:

    session(tcp::socket socket, std::shared_ptr<IStorer> const storer)  
        : m_socket(std::move(socket))
        , m_storer(storer) { }

    void run() 
    {
        wait_for_request();
    }

    void processData(const std::string& data, const std::string& /*filename*/);

private:

    void wait_for_request();

    bool execute_load(boost::uuids::uuid uuid);

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
        do_accept(directory);
    }

private:

    void do_accept(const std::string& directory);
    
    tcp::acceptor m_acceptor;
    std::shared_ptr<IStorer> const m_storer;
};