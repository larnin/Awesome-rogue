#include "tr.h"
#include "Libs/json.hpp"
#include <algorithm>
#include <fstream>

std::vector<std::pair<std::string, std::string>> Tr::m_texts;
std::string Tr::m_filename;
using json = nlohmann::json;

void Tr::load(const std::string & filename)
{
    m_texts.clear();

    std::ifstream file(filename);
    if(!file.is_open())
        return;
    json j(json::parse(file));
    file.close();

    if(!j.is_object())
        return;
    for(auto it(j.begin()) ; it != j.end() ; it++)
        m_texts.emplace_back(it.key(), it.value().get<std::string>());
    m_filename = filename;
}

void Tr::save()
{
    if(m_filename.empty())
        return;

    json datas;
    for(const auto & s : m_texts)
        datas[s.first] = s.second;

    std::ofstream file(m_filename);
    if(!file.is_open())
        return;
    file << datas.dump();
    file.close();
}

std::string Tr::tr(const std::string & str)
{
    auto it(std::find_if(m_texts.begin(), m_texts.end(), [str](const auto & t){ return t.first == str; }));
    if(it == m_texts.end())
        return addKey(str);
    return it->second;
}

std::string Tr::addKey(const std::string & newKey)
{
    //const std::string newKeyText("## Unimplemented text ##");
    m_texts.emplace_back(newKey, newKey);
    return newKey;
}

std::string tr(const std::string & s)
{
    return Tr::tr(s);
}
