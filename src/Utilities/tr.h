#ifndef TR_H
#define TR_H

#include <string>
#include <vector>
#include <type_traits>

class Tr
{
public:
    static void load();
    static void save();
    static std::string tr(const std::string & str);

    static const std::string langDir;

    static std::string systemLang();
    static std::string defaultLang();
    static std::vector<std::string> availableLangs();
    static std::string langPartOf(std::string lang); // en_english
    static std::string namePartOf(std::string lang);
    static std::string fileNameFromLang(const std::string & lang);
    static std::string fileNameFromName(const std::string & name);

private:
    Tr() = delete;
    static std::string addKey(const std::string & newKey);
    static std::string m_filename;

    static std::vector<std::pair<std::string, std::string>> m_texts;
};

namespace
{
    std::string trPrivate(const std::string & s)
    {
        return s;
    }

    std::string str(const std::string & v)
    {
        return v;
    }

    template <typename T, class = typename std::enable_if<std::is_arithmetic<T>::value>::type>
    std::string str(const T & v)
    {
        return std::to_string(v);
    }

    template<typename T, typename... Args>
    std::string trPrivate(const std::string & s, const T & t, Args ... args)
    {
        auto index(s.find('$'));
        if(index == std::string::npos)
            return s;
        return trPrivate(s.substr(0, index) + str(t) + s.substr(index+1), args...);
        //return trPrivate(s + " " + str(t), args ...);
    }
}

std::string tr(const std::string & s);

template <typename T, typename... Args>
std::string tr(const std::string & s, const T & v, const Args ... args)
{
    return trPrivate(Tr::tr(s), v, args...);
}

#endif // TR_H
