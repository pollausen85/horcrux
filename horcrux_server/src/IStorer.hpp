#pragma once

#include <string>
#include <boost/uuid/uuid.hpp>            // uuid class
#include <boost/uuid/uuid_io.hpp>         // streaming operators etc.
#include <vector>

class IStorer
{
public:

    virtual bool save(const boost::uuids::uuid& uuid, const std::string& data, const uint32_t index = 0) = 0;

    virtual bool load(const boost::uuids::uuid& uuid, std::vector<std::string>& data) = 0;
};