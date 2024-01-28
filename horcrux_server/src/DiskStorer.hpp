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

    /// @brief Save file to disk
    /// @param uuid  unique id of the file
    /// @param data  content of the file to be saved
    /// @param index index of the file
    bool save(const boost::uuids::uuid& uuid, 
              const std::string& data, 
              const uint32_t index = 0) override;

    /// @brief Load file form disk
    /// @param uuid  unique id of the file
    /// @param data  file retrieved related to the same uuid
    bool load(const boost::uuids::uuid& uuid, std::vector<std::string>& data) override;

private:

    DiskMap m_diskMap;
    fs::path m_filePath;

    /// @brief Create the filename of the file to be saved
    /// @param uuid  unique id of the file
    /// @param index index of the file
    /// @return m_filePath + uuid + "_" + index + ".hrx"
    std::string createFilename(const boost::uuids::uuid& uuid, const uint32_t index);

    /// @brief Add delimeter at the end of the path if not present
    /// @param path  path to check
    void addTrailingDelimiter(fs::path& path) 
    {
    if (!path.empty() && path.generic_string().back() != '/')
        path += '/';
    }

};