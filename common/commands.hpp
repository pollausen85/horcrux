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
    std::string commandName;
    boost::uuids::uuid uuid;
    uint32_t index;
    uint32_t totalCount;
    std::string payload;
    uint32_t payloadSize;
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

void SaveCommandToJson(json& j, const CommandData& sc)
{
    j = json
    {
        {"command","save"},
        {"id", boost::uuids::to_string(sc.uuid)},
        {"index",sc.index},
        {"total_count",sc.totalCount},
        {"payload", sc.payload},
        {"payload_size", sc.payloadSize}
    };
}

void LoadCommandToJson(json& j, const CommandData& lc)
{
    j = json
    {
        {"command","load"},
        {"id", boost::uuids::to_string(lc.uuid)},
        {"index",lc.index},
        {"total_count",lc.totalCount},
        {"payload", lc.payload},
        {"payload_size", lc.payloadSize}
    };
}

void JsonFromCommandData(const json& j, CommandData& sc)
{
    j.at("command").get_to(sc.commandName);
    std::string uuid;
    j.at("id").get_to(uuid);
    sc.uuid = boost::lexical_cast<boost::uuids::uuid>(uuid);
    j.at("index").get_to(sc.index);
    j.at("total_count").get_to(sc.totalCount);
    j.at("payload").get_to(sc.payload);
    j.at("payload_size").get_to(sc.payloadSize);
}

void StatusDataToJson(json& j, const StatusData& sd)
{
    j = json
    {
        {"command", sd.commandName},
        {"code", sd.code}
    };
}

void JsonFromStatusData(const json& j, StatusData& sd)
{
    j.at("command").get_to(sd.commandName);
    j.at("code").get_to(sd.code);
}


void LoadCommandToJson(json& j, const LoadCommand& lc)
{
    j = json
    {
        {"command", lc.commandName},
        {"id", boost::uuids::to_string(lc.uuid)},
    };
}

void JsonFromLoadCommand(const json& j, LoadCommand& lc)
{
    j.at("command").get_to(lc.commandName);
    std::string uuid;
    j.at("id").get_to(uuid);
    lc.uuid = boost::lexical_cast<boost::uuids::uuid>(uuid);
}