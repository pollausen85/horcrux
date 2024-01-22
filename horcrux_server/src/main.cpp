
#include <iostream>
#include <string>

#include <boost/asio.hpp>
#include "server.hpp"

int main(int argc, char* argv[])
{
    boost::asio::io_context io_context;
    server s(io_context, 25000);
    io_context.run();

    return 0;
}