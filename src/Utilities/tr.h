#ifndef TR_H
#define TR_H

#include <string>
#include <vector>
#include <type_traits>

class Tr
{
    template <typename ... Args>
    friend std::string tr(const std::string & str, Args && ... args);

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

    template <typename T, typename ... Args>
    static std::string combine(const std::string & s, unsigned int index, const T & value, Args && ... args)
    {
        return combine(findAndReplace(s, "{" + std::to_string(index) + "}", str(value)), index + 1, args...);
    }

    static std::string findAndReplace(const std::string & s, const std::string & key, const std::string & value);

    static std::string combine(const std::string & s, unsigned int);


    template <typename T, class = typename std::enable_if<std::is_arithmetic<T>::value>::type>
    static std::string str(const T & v)
    {
        return std::to_string(v);
    }

    static std::string str(const std::string & v);

    static std::string m_filename;
    static std::vector<std::pair<std::string, std::string>> m_texts;
};

template <typename ... Args>
std::string tr(const std::string & str, Args && ... args)
{
    return Tr::combine(Tr::tr(str), 0, args...);
}

std::string tr(const std::string & str);

#endif // TR_H
