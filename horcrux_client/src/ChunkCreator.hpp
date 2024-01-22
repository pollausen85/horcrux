#pragma once
#include "ISplitter.hpp"

struct Chunck
{
    uint32_t payloadSize;
    std::string payload;
};

class ChunckCreator : public ISPlitter<Chunck>
{
public:
    ChunckCreator() = default;

    std::vector<Chunck> split(const uint32_t chuncksCount, const std::string& fileName);
};