
#include "client.hpp"
#include <iostream>
#include <string>

int main(int argc, char* argv[])
{
    boost::asio::io_context io_context;
    client cl(io_context);
    
    // std::string data{"some client data ..."};
    // auto result = boost::asio::write(socket, boost::asio::buffer(data));
    
    // std::cout << "data sent: " << data.length() << '/' << result << std::endl;

    cl.disconnect();

    return 0;
}