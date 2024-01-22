
#include "client.hpp"
#include <iostream>
#include <string>
#include <cstring>
#include "ChunkCreator.hpp"

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
    std::shared_ptr<ChunkCreator> pChuncker(std::make_shared<ChunkCreator>());
    Client<Chunk> cl(io_context, pChuncker);
    
    if(!cl.connect())
    {
        return -1;
    }
    // std::string data{"some client data ..."};
    // auto result = boost::asio::write(socket, boost::asio::buffer(data));
    if(std::strcmp(argv[1],"save") == 0)
    {
        try 
        {
            int chunkCount = std::stoi(argv[3], nullptr);
            cl.sendSaveCommand(chunkCount, argv[4]);
        } 
        catch (std::invalid_argument const &ex) 
        {
            std::cout << "Invalid number: " << argv[3] << '\n';
        } 
        catch (std::out_of_range const &ex) 
        {
            std::cout << "Number out of range: " << argv[3] << '\n';
        }
    }

    cl.disconnect();

    return 0;
}