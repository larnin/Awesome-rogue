#include "serializabletype.h"
#include <stdexcept>

SerializableType fromJson(const json & j)
{
    auto it(j.find("serializableType"));
    if(it == j.end())
        throw std::runtime_error("Can't find the serialisableType on json !");
    return SerializableType(int(*it));
}
