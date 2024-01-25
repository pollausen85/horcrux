
#include <iostream>
#include <string>

#include <boost/asio.hpp>
#include "server.hpp"
#include "DiskStorer.hpp"

int main(int argc, char* argv[])
{
    boost::asio::io_context io_context;
    auto storer = std::make_shared<DiskStorer>();
    server s(io_context, 25000, R"(./)", storer);
    io_context.run();

    return 0;
}