#include "tr.h"
#include "configs.h"
#include "Libs/json.hpp"
#include "File/serializer.h"
#include <algorithm>
#include <fstream>
#include <iostream>

#ifdef __linux__
#include <stdlib.h>
#endif
#ifdef _WIN32
#ifdef _WIN32_WINNT
#undef _WIN32_WINNT
#endif
#define _WIN32_WINNT 0x0600
#include <windows.h>
#endif

using json = nlohmann::json;

std::vector<std::pair<std::string, std::string>> Tr::m_texts;
std::string Tr::m_filename;
const std::string Tr::langDir("res/lang/");

void Tr::load()
{
    std::string filename(Tr::fileNameFromLang(Configs::c.lang));
    if(filename.empty())
    {
        filename = Tr::fileNameFromLang(Tr::systemLang());
        if(!filename.empty())
            Configs::c.lang = Tr::defaultLang();
    }

    m_texts.clear();

    m_filename = filename;

    try
    {
        std::ifstream file(filename);
        if(!file.is_open())
            return;
        json j(json::parse(file));
        file.close();

        if(!j.is_object())
            return;
        for(auto it(j.begin()) ; it != j.end() ; it++)
            m_texts.emplace_back(it.key(), it.value().get<std::string>());
    }
    catch(const std::exception & e){}
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
    std::cout << "New key : '" << newKey << "'" << std::endl;
    m_texts.emplace_back(newKey, newKey);
    return newKey;
}

std::string Tr::systemLang()
{
#ifdef __linux__
    if(const char* env_p = std::getenv("LANG"))
        return std::string{env_p[0], env_p[1]};
#endif
#ifdef _WIN32
    long unsigned int count(0);
    long unsigned int size(0);
    bool ok(GetUserPreferredUILanguages(MUI_LANGUAGE_NAME, &count, nullptr, &size));
    if(ok)
    {
        wchar_t *cLang = new wchar_t[size];

        ok = GetUserPreferredUILanguages(MUI_LANGUAGE_NAME, &count, cLang, &size);
        if(ok && size >= 2)
            return std::string{static_cast<char>(wctob(cLang[0])), static_cast<char>(wctob(cLang[1]))};
    }
#endif
    return defaultLang();
}

std::string Tr::defaultLang()
{
    return "en";
}

std::vector<std::string> Tr::availableLangs()
{
    auto files(filesIn(langDir, "json"));
    return files;
}

namespace
{
std::string extractFilename(std::string & f)
{
    auto pos(f.find_last_of(("/\\")));
    if(pos != std::string::npos)
        f = f.substr(pos+1);
    pos = f.find_last_of('.');
    if(pos != std::string::npos)
        f = f.substr(0, pos);
    return f;
}
}

std::string Tr::langPartOf(std::string lang)
{
    if(lang.empty())
        return "";
    lang = extractFilename(lang);
    auto pos(lang.find('_'));
    if(pos == std::string::npos)
        return "";
    return lang.substr(0, pos);
}

std::string Tr::namePartOf(std::string lang)
{
    if(lang.empty())
        return "";
    lang = extractFilename(lang);
    auto pos(lang.find('_'));
    if(pos == std::string::npos)
        return "";
    return lang.substr(pos+1);
}

std::string Tr::fileNameFromLang(const std::string & lang)
{
    auto list(availableLangs());
    for(auto l : list)
        if(langPartOf(l) == lang)
            return l;
    return "";
}

std::string Tr::fileNameFromName(const std::string & name)
{
    auto list(availableLangs());
    for(auto l : list)
        if(namePartOf(l) == name)
            return l;
    return "";
}

std::string Tr::combine(const std::string & s, unsigned int)
{
    return s;
}

std::string tr(const std::string & str)
{
    return Tr::tr(str);
}

std::string Tr::findAndReplace(const std::string & s, const std::string & key, const std::string & value)
{
    auto index = s.find(key);
        if(index == std::string::npos)
            return s;
        return findAndReplace(s.substr(0, index) + value + s.substr(index+key.size()), key, value);
}

std::string Tr::str(const std::string & v)
{
    return v;
}


