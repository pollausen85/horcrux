#pragma once

#include <vector>
#include <string>

template<class T>
class ISPlitter
{
public:

    virtual std::vector<T> split(const uint32_t chuncksCount, const std::string& fileName) = 0;
};