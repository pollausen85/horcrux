#pragma once

#include <vector>
#include <string>

template<class T>
class ISPlitter
{
public:

    /// @brief Split file in chunks
    /// @param chunksCount  number of chunks
    /// @param fileName     file to be splitted
    /// @param chunks       vector containing the chunked data
    virtual bool split(const uint32_t chunksCount, const std::string& fileName, std::vector<T>& chunks) = 0;
};