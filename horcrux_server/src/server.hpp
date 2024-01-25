#pragma once

#include <iostream>
#include <boost/asio.hpp>
#include <mutex>
#include <condition_variable>

using boost::asio::ip::tcp;

class session : public std::enable_shared_from_this<session>
{

public:

    session(tcp::socket socket)  
        : m_socket(std::move(socket))
        // , m_ready(false)
    { }

    void run() 
    {
        wait_for_request();
    }

private:

    void wait_for_request();

    void manage_command(const std::string& data);

    void wait_for_complete_message(const std::string& data);

    tcp::socket m_socket;
    boost::asio::streambuf m_buffer;
    std::string m_strBuf;
    // std::mutex m_mutex;
    // std::condition_variable m_cv;
    // bool m_ready;
};


class server
{
public:
    server(boost::asio::io_context& io_context, short port) 
    : m_acceptor(io_context, tcp::endpoint(tcp::v4(), port)) 
    {
        do_accept();
    }

private:

    void do_accept();
    
    tcp::acceptor m_acceptor;
};