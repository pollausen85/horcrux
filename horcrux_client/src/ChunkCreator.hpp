#pragma once
#include "ISplitter.hpp"
#include "IChecksum.hpp"
#include <memory>

struct Chunk
{
    uint32_t payloadSize;
    std::string payload;
    uint32_t checksum;
};

class ChunkCreator : public ISPlitter<Chunk>
{
public:
    ChunkCreator(std::shared_ptr<IChecksum> const checksumCreator) 
        : m_checksumCreator(checksumCreator) {}

    /// @brief Split file in chunks
    /// @param chunksCount  number of chunks
    /// @param fileName     file to be splitted
    /// @param chunks       vector containing the chunked data
    bool split(const uint32_t chunksCount, 
               const std::string& fileName, 
               std::vector<Chunk>& chunks) override;

    virtual std::shared_ptr<IChecksum> getChecksumCreator() const {return m_checksumCreator;}

private:
    std::shared_ptr<IChecksum> const m_checksumCreator;
};