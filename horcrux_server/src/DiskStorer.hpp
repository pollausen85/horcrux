#pragma once

#include "IStorer.hpp"
#include <unordered_map>
#include <boost/functional/hash.hpp>
#include <filesystem>

namespace fs = std::filesystem;

class DiskStorer : public IStorer
{

public:

    using DiskMap = std::unordered_map<boost::uuids::uuid, std::vector<std::string>, boost::hash<boost::uuids::uuid>>;

    DiskStorer(const std::string& filePath = R"(./)") 
        : m_filePath(filePath) 
    {
        m_filePath = fs::canonical(m_filePath);
        addTrailingDelimiter(m_filePath);
    }

    bool save(const boost::uuids::uuid& uuid, const std::string& data, const uint32_t index = 0);

    bool load(const boost::uuids::uuid& uuid, std::vector<std::string>& data);

private:

    DiskMap m_diskMap;
    fs::path m_filePath;

    std::string createFilename(const boost::uuids::uuid& uuid, const uint32_t index);

    void addTrailingDelimiter(fs::path& path) 
    {
    if (!path.empty() && path.generic_string().back() != '/')
        path += '/';
    }

};