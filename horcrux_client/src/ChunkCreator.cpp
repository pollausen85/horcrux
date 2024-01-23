#include "ChunkCreator.hpp"
#include <filesystem>
#include <fstream>
#include <cmath>
#include "base64pp/base64pp.h"

bool ChunkCreator::split(const uint32_t chunksCount, const std::string& fileName, std::vector<Chunk>& chunks)
{
    std::ifstream binFile(fileName, std::ios::binary);
    if (!binFile.is_open()) 
    {
        return false;
    }

    chunks.resize(chunksCount);
    uintmax_t fileSize = std::filesystem::file_size(fileName);
    uintmax_t chunkSize = std::ceil(static_cast<double>(fileSize)/static_cast<double>(chunksCount));
    for(uint32_t i = 0; i < chunksCount; ++i)
    {
        std::vector<uint8_t> chunk(chunkSize);
        binFile.seekg(i*chunkSize);
        binFile.read(reinterpret_cast<char*>(&chunk[0]), chunkSize);
        if (binFile.bad())
        {
            binFile.close();
            return false;
        }

        uintmax_t actuallyCount = binFile.gcount();
        if(actuallyCount != chunkSize)
        {
            chunk.resize(actuallyCount);
        }
        chunks[i].payloadSize = actuallyCount;
        chunks[i].payload = base64pp::encode(chunk);
    }

    binFile.close();

    return true;
}