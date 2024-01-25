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
    boost::asio::async_read_until(m_socket, m_buffer, '\n', 
    [this, self](boost::system::error_code ec, std::size_t /*length*/)
    {
        if (!ec)  
        {
            std::string data
            {
                std::istreambuf_iterator<char>(&m_buffer), 
                std::istreambuf_iterator<char>() 
            };
            
            wait_for_complete_message(data);
        } 
        else 
        {
            std::cout << "error: " << ec << std::endl;
        }
    });
}

void session::wait_for_complete_message(const std::string& data)
{
    // Append the received data to the buffer
    m_strBuf += data;

    // Check if the buffer contains a complete message (ends with a newline character)
    size_t newlinePos;
    while ((newlinePos = m_strBuf.find('\n')) != std::string::npos)
    {
        // Extract the complete message
        std::string completeMessage = m_strBuf.substr(0, newlinePos);
        
        // Remove the processed message from the buffer
        m_strBuf = m_strBuf.substr(newlinePos + 1);

        // Process the complete message
        manage_command(completeMessage);
    }
}

void session::manage_command(const std::string& data)
{
    std::cout << "Received data: " << data << std::endl;
    
    try 
    {
        auto j = json::parse(data);
        CommandData sc;
        JsonFromSaveCommand(j, sc);

        if (sc.commandName == "save" || sc.commandName == "load")
        {
            // boost::asio::async_write(m_socket, boost::asio::buffer("Command received"),
            //     [this](boost::system::error_code ec, std::size_t /*length*/)
            //     {
            //         if (!ec) {
            //             std::cout << "Response sent: Command received" << std::endl;
            //             // Continue waiting for the next request after sending the response
            //             wait_for_request();
            //         } else {
            //             std::cout << "error: " << ec << std::endl;
            //         }
            //     });
            wait_for_request();
        }
    } 
    catch (const json::parse_error& e) 
    {
        std::cerr << "JSON parsing error: " << e.what() << std::endl;

        wait_for_request();
    }
}
