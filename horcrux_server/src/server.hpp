#pragma once

#include <iostream>
#include <boost/asio.hpp>
#include <mutex>
#include <memory>
#include "DiskStorer.hpp"
#include "IChecksum.hpp"

class SaveCommand;

using boost::asio::ip::tcp;

class session : public std::enable_shared_from_this<session>
{

public:

    session(tcp::socket socket, std::shared_ptr<IStorer> const storer,
            std::shared_ptr<IChecksum> const checksumCalculator)  
        : m_socket(std::move(socket))
        , m_storer(storer) 
        , m_checksumCalculator(checksumCalculator) { }

    /// @brief Start a new session
    void run() 
    {
        waitForRequestOrResponse();
    }

private:

    friend class Utils;

    /// @brief Process the data received by the client
    /// @param data      data to be processed 
    /// @param filename  path where save the file
    void processData(const std::string& data, const std::string& /*filename*/, 
                     uint32_t& /*index*/, uint32_t& /*total*/);

    /// @brief Wait new data
    /// @param filename not used in this function
    void waitForRequestOrResponse(const std::string& filename = "");

    /// @brief Execute the load command
    /// @param uuid unique id of file to recover
    void executeLoad(const boost::uuids::uuid& uuid);

    /// @brief Execute the save command
    /// @param sc information needed to execute the save command
    void executeSave(const SaveCommand& sc);

    /// @brief Write to the socket using async_write
    /// @param data data to write in the buffer
    /// @param size size of the data
    void write(char* const data, size_t size);

    /// @brief Modify and write to the socket using async_write the m_jsonBuf
    /// @param data data to write in the buffer
    void modifyAndWriteJsonBuffer(const std::string& data);

    /// @brief Modify and write to the socket using async_write the m_fileContent
    /// @param file opened and read file where data are stored
    /// @param size file size in bytes
    void modifyAndWriteFileContent(std::ifstream& file, const std::streamsize& size);

    bool checkCRCcorrectness(const std::string& payload, uint32_t checksum);

    tcp::socket m_socket;
    boost::asio::streambuf m_buffer;
    std::mutex m_jsonMutex;
    std::string m_strBuf;
    std::string m_jsonBuf;
    std::shared_ptr<IStorer> const m_storer;
    std::shared_ptr<IChecksum> const m_checksumCalculator;
    std::vector<char> m_fileContent;
    std::mutex m_fileContentMutex;
};


class server
{
public:
    server(boost::asio::io_context& io_context, short port, const std::string& directory, std::shared_ptr<IStorer> const storer) 
    : m_acceptor(io_context, tcp::endpoint(tcp::v4(), port))
    , m_storer(storer)
    {
        doAccept(directory);
    }

private:

    /// @brief Accept a new connection from a client
    /// @param directory directory where save the .hrx file
    void doAccept(const std::string& directory);
    
    tcp::acceptor m_acceptor;
    std::shared_ptr<IStorer> const m_storer;
};