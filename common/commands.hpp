#pragma once
#include <string>
#include <boost/uuid/uuid.hpp>            // uuid class
#include <boost/uuid/uuid_generators.hpp> // generators
#include <boost/uuid/uuid_io.hpp>         // streaming operators etc.
#include <boost/lexical_cast.hpp>
#include <nlohmann/json.hpp>
using json = nlohmann::json;

enum class ErrCode
{
    ErrFileNotFound = -2,
    ErrSavingFile = -1,
    NoError = 0
};

struct CommandData
{
    boost::uuids::uuid uuid;
    uint32_t index;
    uint32_t totalCount;
    std::string payload;
    uint32_t payloadSize;
};

struct SaveCommand
{
    std::string commandName;
    CommandData data; 
};

struct LoadResponse
{
    int code;
    CommandData data; 
};

struct StatusData
{
    std::string commandName;
    int code;
};

struct LoadCommand
{
    std::string commandName;
    boost::uuids::uuid uuid;
};

void to_json(json& j, const CommandData& sc)
{
    j = json
    {
        {"id", boost::uuids::to_string(sc.uuid)},
        {"index",sc.index},
        {"total_count",sc.totalCount},
        {"payload", sc.payload},
        {"payload_size", sc.payloadSize}
    };
}

void to_json(json& j, const SaveCommand& sc)
{
    j = json
    {
        {"command","save"},
        {"data", sc.data}
    };
}

void to_json(json& j, const LoadResponse& lr)
{
    j = json
    {
        {"code",lr.code},
        {"data", lr.data}
    };
}

void from_json(const json& j, CommandData& sc)
{
    std::string uuid;
    j.at("id").get_to(uuid);
    sc.uuid = boost::lexical_cast<boost::uuids::uuid>(uuid);
    j.at("index").get_to(sc.index);
    j.at("total_count").get_to(sc.totalCount);
    j.at("payload").get_to(sc.payload);
    j.at("payload_size").get_to(sc.payloadSize);
}

void from_json(const json& j, SaveCommand& sc)
{
    j.at("command").get_to(sc.commandName);
    j.at("data").get_to(sc.data);
}

void to_json(json& j, const StatusData& sd)
{
    j = json
    {
        {"command", sd.commandName},
        {"code", sd.code}
    };
}

void from_json(const json& j, StatusData& sd)
{
    j.at("command").get_to(sd.commandName);
    j.at("code").get_to(sd.code);
}


void to_json(json& j, const LoadCommand& lc)
{
    j = json
    {
        {"command", lc.commandName},
        {"id", boost::uuids::to_string(lc.uuid)},
    };
}

void from_json(const json& j, LoadCommand& lc)
{
    j.at("command").get_to(lc.commandName);
    std::string uuid;
    j.at("id").get_to(uuid);
    lc.uuid = boost::lexical_cast<boost::uuids::uuid>(uuid);
}