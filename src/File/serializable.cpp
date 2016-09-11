#include "serializable.h"
#include "algorithm"

std::vector<Serializable*> Serializable::m_serializables;

Serializable::Serializable(SerializableType type)
    : m_serializableType(type)
{
    add(this);
}

Serializable::Serializable(const Serializable &other)
    : m_serializableType(other.m_serializableType)
{
    add(this);
}

Serializable & Serializable::operator =(const Serializable & other)
{
    m_serializableType = other.m_serializableType;

    add(this);
    return *this;
}

Serializable::Serializable(Serializable &&other)
    : m_serializableType(other.m_serializableType)
{
    del(&other);
    add(this);
}

Serializable & Serializable::operator =(Serializable && other)
{
    m_serializableType = std::move(other.m_serializableType);

    del(&other);
    add(this);
    return *this;
}

Serializable::~Serializable()
{
    del(this);
}

void Serializable::add(Serializable * s)
{
    assert(s != nullptr);
    m_serializables.push_back(s);
}

void Serializable::del(Serializable * s)
{
    auto it(std::find(m_serializables.begin(), m_serializables.end(), s));
    if(it == m_serializables.end())
        return;
    m_serializables.erase(it);
}

json Serializable::serializeAll()
{
    json j;
    for(auto s : m_serializables)
    {
        json js(s->serialize());
        js["serializableType"] = s->m_serializableType;
        j.push_back(js);
    }
    return j;
}
