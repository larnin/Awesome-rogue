#ifndef CONFIGS_H
#define CONFIGS_H

#include <string>
#include <SFML/System/Vector2.hpp>

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


private:
    Configs(const std::string & filename);
    ~Configs();
    void load(const std::string & filename);
    void save(const std::string & filename);
};

#endif // CONFIGS_H
