#pragma once
#include <string>
#include <memory>
#include <type_traits>
#include <base64pp/base64pp.h>

class Utils
{
public:

    /// @brief Wait for a complete message in order to process it 
    /// @param data     new data just read
    /// @param ptr      pointer to a class that will process data
    /// @param buffer   buffer where checking the presence of a complete message
    /// @param fileName path where save the file
    template<class T, typename std::enable_if<std::is_member_function_pointer<decltype(&T::processData)>::value, int>::type = 0>
    static void waitForCompleteMessage(const std::string& data,
                                       std::shared_ptr<T> const ptr,
                                       std::string& buffer, 
                                       const std::string& fileName = "");

    /// @brief Wait for a complete load message in order to process it 
    /// @param data     new data just read
    /// @param ptr      pointer to a class that will process data
    /// @param buffer   buffer where checking the presence of a complete message
    /// @param fileName path where save the file
    template<class T, typename std::enable_if<std::is_member_function_pointer<decltype(&T::processData)>::value, int>::type = 0,
             typename std::enable_if<std::is_member_function_pointer<decltype(&T::waitForRequestOrResponse)>::value, int>::type = 0>
    static void waitForCompleteLoadMessage(const std::string& data,
                                           std::shared_ptr<T> const ptr,
                                           std::string& buffer, 
                                           const std::string& fileName = "");

    template<class T, typename std::enable_if<std::is_member_function_pointer<decltype(&T::computeChecksum)>::value, int>::type = 0>
    static bool checkCRCcorrectness(const std::string& payload, std::shared_ptr<T> const ptr, uint32_t checksum);

    static std::string errorCodeToStr(int errCode);

private:

    template<class T, typename std::enable_if<std::is_member_function_pointer<decltype(&T::processData)>::value, int>::type = 0>
    static void searchCompleteMessage(const std::string &data, 
                                      std::shared_ptr<T> const ptr,
                                      std::string& buffer, 
                                      uint32_t& index, 
                                      uint32_t& total,
                                      const std::string& fileName = "");
};

template<class T, typename std::enable_if<std::is_member_function_pointer<decltype(&T::processData)>::value, int>::type>
void Utils::waitForCompleteMessage(const std::string &data, 
                                   std::shared_ptr<T> const ptr,
                                   std::string& buffer, 
                                   const std::string& fileName)
{
    uint32_t index, total;
    searchCompleteMessage(data, ptr, buffer, index, total, fileName);
}

template<class T, typename std::enable_if<std::is_member_function_pointer<decltype(&T::processData)>::value, int>::type,
         typename std::enable_if<std::is_member_function_pointer<decltype(&T::waitForRequestOrResponse)>::value, int>::type>
void Utils::waitForCompleteLoadMessage(const std::string& data,
                                       std::shared_ptr<T> const ptr,
                                       std::string& buffer, 
                                       const std::string& fileName)
{
    uint32_t index, total;
    searchCompleteMessage(data, ptr, buffer, index, total, fileName);

    if((index+1) < total)
    {
        ptr->waitForRequestOrResponse(fileName);
    }
}

template<class T, typename std::enable_if<std::is_member_function_pointer<decltype(&T::processData)>::value, int>::type>
void Utils::searchCompleteMessage(const std::string &data, 
                                  std::shared_ptr<T> const ptr,
                                  std::string& buffer, 
                                  uint32_t& index,
                                  uint32_t& total,
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
        ptr->processData(completeMessage, fileName, index, total);
    }
}

template<class T, typename std::enable_if<std::is_member_function_pointer<decltype(&T::computeChecksum)>::value, int>::type>
bool Utils::checkCRCcorrectness(const std::string& payload, std::shared_ptr<T> const ptr, uint32_t checksum)
{
    std::vector<uint8_t> binaryData = base64pp::decode(payload).value_or(std::vector<uint8_t>());
    uint32_t crc32 = ptr->computeChecksum(reinterpret_cast<char*>(&binaryData[0]),binaryData.size());
    return (crc32 == checksum);
}