
#include "client.hpp"
#include <iostream>
#include <string>
#include <cstring>
#include "ChunkCreator.hpp"
#include "CRC32.hpp"

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
    std::shared_ptr<IChecksum> pCRCcalculator(std::make_shared<CRC32>());
    std::shared_ptr<ChunkCreator> pChuncker(std::make_shared<ChunkCreator>(pCRCcalculator));
    auto cl = std::make_shared<Client<Chunk>>(io_context, pChuncker);
    
    if(!cl->connect())
    {
        return -1;
    }
    
    if(std::strcmp(argv[1],"save") == 0)
    {
        try 
        {
            std::string uuid;
            int chunkCount = std::stoi(argv[3], nullptr);
            cl->sendSaveCommand(chunkCount, argv[4], uuid);
            std::cout << uuid << '\n';
        } 
        catch (std::invalid_argument const &ex) 
        {
            std::cerr << "Invalid number: " << argv[3] << '\n';
        } 
        catch (std::out_of_range const &ex) 
        {
            std::cerr << "Number out of range: " << argv[3] << '\n';
        }
    }
    else if (std::strcmp(argv[1],"load") == 0)
    {
        cl->sendLoadCommand(argv[2],argv[3]);  
    }

    cl->disconnect();

    return 0;
}