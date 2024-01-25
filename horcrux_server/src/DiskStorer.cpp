#include "DiskStorer.hpp"
#include <fstream>

bool DiskStorer::save(const boost::uuids::uuid& uuid, const std::string& data, const uint32_t index)
{
    try
    {
        std::string filename = createFilename(uuid, index);
        std::ofstream of(filename, std::ios::out | std::ios::binary);
        if(of.good())
        {
            of.write(data.c_str(), data.size());
            of.close();

            m_diskMap[uuid].push_back(filename);

            return true;
        }

        return false;
    }
    catch(const std::exception& e)
    {
        return false;
    }
}

bool DiskStorer::load(const boost::uuids::uuid& uuid, std::vector<std::string>& data)
{
    if(m_diskMap.find(uuid) != m_diskMap.end())
    {
        data = m_diskMap[uuid];
        return true;
    }

    return false;
}

std::string DiskStorer::createFilename(const boost::uuids::uuid& uuid, const uint32_t index)
{
    std::string strUUID = boost::uuids::to_string(uuid);
    return m_filePath.generic_string() + strUUID + "_" + std::to_string(index) + ".hrx";
}