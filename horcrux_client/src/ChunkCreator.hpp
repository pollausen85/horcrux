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

    /// @brief Split file in chunks
    /// @param chunksCount  number of chunks
    /// @param fileName     file to be splitted
    /// @param chunks       vector containing the chunked data
    bool split(const uint32_t chunksCount, 
               const std::string& fileName, 
               std::vector<Chunk>& chunks) override;
};