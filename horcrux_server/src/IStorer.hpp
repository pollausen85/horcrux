#pragma once

#include <string>
#include <boost/uuid/uuid.hpp>            // uuid class
#include <boost/uuid/uuid_io.hpp>         // streaming operators etc.
#include <vector>

class IStorer
{
public:

    /// @brief Save file to disk
    /// @param uuid  unique id of the file
    /// @param data  content of the file to be saved
    /// @param index index of the file
    virtual bool save(const boost::uuids::uuid& uuid, const std::string& data, const uint32_t index = 0) = 0;

    /// @brief Load file form disk
    /// @param uuid  unique id of the file
    /// @param data  file retrieved related to the same uuid
    virtual bool load(const boost::uuids::uuid& uuid, std::vector<std::string>& data) = 0;
};