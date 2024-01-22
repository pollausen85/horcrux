
#include "client.hpp"
#include <iostream>
#include <string>
#include <cstring>

int main(int argc, char* argv[])
{
    if(argc != 4 && argc != 5)
    {
        std::cout << "Error running horcrux_client\n";
        std::cout << "Usage: horcrux_client save -n <horcrux_count> <input_path>\n";
        std::cout << "Usage: horcrux_client load <file_id> <output_file>\n";
        return -1;
    }

    if(std::strcmp(argv[1],"load") != 0 && std::strcmp(argv[1],"save") != 0)
    {
        std::cout << "Error unknown command\n";
        std::cout << "Usage: horcrux_client save -n <horcrux_count> <input_path>\n";
        std::cout << "Usage: horcrux_client load <file_id> <output_file>\n";
        return -1;
    }

    if((std::strcmp(argv[1],"load") == 0 && argc != 4) || (std::strcmp(argv[1],"save") == 0 && argc != 5))
    {
        std::cout << "Error wrong input parameters\n";
        std::cout << "Usage: horcrux_client save -n <horcrux_count> <input_path>\n";
        std::cout << "Usage: horcrux_client load <file_id> <output_file>\n";
        return -1;
    }

    boost::asio::io_context io_context;
    std::shared_ptr<ChunckCreator> pChuncker(std::make_shared<ChunckCreator>());
    Client cl(io_context, pChuncker);
    
    if(!cl.connect())
    {
        return -1;
    }
    // std::string data{"some client data ..."};
    // auto result = boost::asio::write(socket, boost::asio::buffer(data));
    cl.sendSaveCommand();
    // std::cout << "data sent: " << data.length() << '/' << result << std::endl;

    cl.disconnect();

    return 0;
}