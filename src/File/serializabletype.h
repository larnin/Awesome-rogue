#ifndef SERIALIZABLETYPE_H
#define SERIALIZABLETYPE_H

#include "Libs/json.hpp"
using json = nlohmann::json;

enum SerializableType
{
    SERIALIZE_ROOM,
};

SerializableType fromJson(const json & j);

#endif // SERIALIZABLETYPE_H
