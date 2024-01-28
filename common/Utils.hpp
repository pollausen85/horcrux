#pragma once
#include <string>
#include <memory>
#include <type_traits>

class Utils
{
public:
    template<class T, typename std::enable_if<std::is_member_function_pointer<decltype(&T::processData)>::value, int>::type = 0>
    static void waitForCompleteMessage(const std::string& data,
                                       std::shared_ptr<T> const ptr,
                                       std::string& buffer, 
                                       const std::string& fileName = "");
};

template<class T, typename std::enable_if<std::is_member_function_pointer<decltype(&T::processData)>::value, int>::type>
void Utils::waitForCompleteMessage(const std::string &data, 
                               std::shared_ptr<T> const ptr,
                               std::string& buffer, 
                               const std::string& fileName)
{
    buffer += data;

    // Check if the buffer contains a complete message (ends with a newline character)
    size_t newlinePos;
    while ((newlinePos = buffer.find('\n')) != std::string::npos)
    {
        std::string completeMessage = buffer.substr(0, newlinePos);
        
        std::string remainingData = buffer.substr(newlinePos + 1);
        buffer = remainingData;

        // Process the complete message
        ptr->processData(completeMessage, fileName);
    }
}