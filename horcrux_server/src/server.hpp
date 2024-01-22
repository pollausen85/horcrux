#pragma once

#include <iostream>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

class session : public std::enable_shared_from_this<session>
{

public:

    session(tcp::socket socket)  : m_socket(std::move(socket)) { }

    void run() 
    {
        wait_for_request();
    }

private:

    void wait_for_request();

    tcp::socket m_socket;
    boost::asio::streambuf m_buffer;
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