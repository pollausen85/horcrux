#pragma once

#include <vector>
#include <string>

template<class T>
class ISPlitter
{
public:

    virtual bool split(const uint32_t chuncksCount, const std::string& fileName, std::vector<T>& chunks) = 0;
};