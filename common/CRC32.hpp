#pragma once
#include "IChecksum.hpp"

class CRC32 : public IChecksum
{
public:

    CRC32() = default;

    /// @brief Compute checksum of data
    /// @param data     data which execute the checksum on
    /// @param size     size in bytes of the data
    uint32_t computeChecksum(const char* const data, size_t size) override;
};