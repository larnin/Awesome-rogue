#ifndef CONFIGS_H
#define CONFIGS_H

#include <SFML/System/Vector2.hpp>
#include <string>

class Configs
{
public:
    static const std::string filename;
    static Configs c;

    bool useFullScreen;
    sf::Vector2u screenSize;
    float zoom;
    float musicVolum;
    float soundsVolum;
    std::string lang;


private:
    Configs(const std::string & filename);
    ~Configs();
    void load(const std::string & filename);
    void save(const std::string & filename);
};

#endif // CONFIGS_H
