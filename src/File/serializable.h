#ifndef SERIALIZABLE_H
#define SERIALIZABLE_H

#include "Libs/json.hpp"
#include "serializabletype.h"
#include <vector>

using json = nlohmann::json;

class Serializable
{
public:
    Serializable(SerializableType type);
    Serializable(const Serializable &other);
    Serializable & operator =(const Serializable & other);
    Serializable(Serializable && other);
    Serializable & operator =(Serializable && other);
    virtual ~Serializable();

    static json serializeAll();

protected:
    virtual json serialize() const = 0;

private:
    static void add(Serializable * s);
    static void del(Serializable * s);
    static std::vector<Serializable*> m_serializables;
    SerializableType m_serializableType;
};

#endif // SERIALIZABLE_H
