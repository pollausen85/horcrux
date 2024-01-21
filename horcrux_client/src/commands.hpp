#pragma once
#include <string>
#include <boost/uuid/uuid.hpp>            // uuid class
#include <boost/uuid/uuid_generators.hpp> // generators
#include <boost/uuid/uuid_io.hpp>         // streaming operators etc.
#include <nlohmann/json.hpp>
using json = nlohmann::json;

struct SaveCommand
{
    std::string commandName;
    boost::uuids::uuid uuid;
    uint32_t index;
    uint32_t totalCount;
    std::string payload;
    uint32_t payloadSize;
};

void SaveCommandToJson(json& j, const SaveCommand& sc)
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

