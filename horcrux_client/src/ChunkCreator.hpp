#pragma once
#include "ISplitter.hpp"

struct Chunk
{
    uint32_t payloadSize;
    std::string payload;
};

class ChunkCreator : public ISPlitter<Chunk>
{
public:
    ChunkCreator() = default;

    bool split(const uint32_t chunksCount, 
               const std::string& fileName, 
               std::vector<Chunk>& chunks) override;
};