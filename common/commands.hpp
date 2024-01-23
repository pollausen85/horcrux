#pragma once
#include <string>
#include <boost/uuid/uuid.hpp>            // uuid class
#include <boost/uuid/uuid_generators.hpp> // generators
#include <boost/uuid/uuid_io.hpp>         // streaming operators etc.
#include <boost/lexical_cast.hpp>
#include <nlohmann/json.hpp>
using json = nlohmann::json;

struct CommandData
{
    std::string commandName;
    boost::uuids::uuid uuid;
    uint32_t index;
    uint32_t totalCount;
    std::string payload;
    uint32_t payloadSize;
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

void JsonFromSaveCommand(const json& j, CommandData& sc)
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

