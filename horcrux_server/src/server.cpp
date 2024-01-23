#include "server.hpp"
#include "commands.hpp"

void server::do_accept() 
{
    m_acceptor.async_accept([this](boost::system::error_code ec, tcp::socket socket) 
    {
        if (!ec) 
        {
            std::cout << "creating session on: " 
                << socket.remote_endpoint().address().to_string() 
                << ":" << socket.remote_endpoint().port() << '\n';

            std::make_shared<session>(std::move(socket))->run();
        } 
        else 
        {
            std::cout << "error: " << ec.message() << std::endl;
        }
        do_accept();
    });
}

void session::wait_for_request() 
{
    auto self(shared_from_this());
    boost::asio::async_read_until(m_socket, m_buffer, "\0", 
    [this, self](boost::system::error_code ec, std::size_t /*length*/)
    {
        if (!ec)  
        {
            std::string data
            {
                std::istreambuf_iterator<char>(&m_buffer), 
                std::istreambuf_iterator<char>() 
            };
            std::cout << data << std::endl;
            wait_for_request();
        } 
        else 
        {
            std::cout << "error: " << ec << std::endl;;
        }
    });
}

void session::manage_command(const std::string& data)
{
    auto j = json::parse(data);
    CommandData sc;
    JsonFromSaveCommand(j, sc);
    if(sc.commandName == "save" || sc.commandName == "load")
    {
        boost::asio::write(m_socket, boost::asio::buffer("Command received"));
    }
}