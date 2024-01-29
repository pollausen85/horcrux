#include "CRC32.hpp"
#include <boost/crc.hpp>

uint32_t CRC32::computeChecksum(const char* const data, size_t size)
{
    boost::crc_32_type result;
    result.process_bytes(data, size);
    return result.checksum();
}