#pragma once

#include <string>
#include <boost/uuid/uuid.hpp>            // uuid class
#include <boost/uuid/uuid_io.hpp>         // streaming operators etc.

class IStorer
{
public:

    virtual bool save(const boost::uuids::uuid& uuid, const std::string& data, const uint32_t index = 0) = 0;
};