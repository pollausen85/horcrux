#pragma once
#include <stddef.h>
#include <stdint.h>
#include <memory>

class IChecksum : public std::enable_shared_from_this<IChecksum>
{
public:

    /// @brief Compute checksum of data
    /// @param data     data which execute the checksum on
    /// @param size     size in bytes of the data
    virtual uint32_t computeChecksum(const char* const data, size_t size) = 0;
};