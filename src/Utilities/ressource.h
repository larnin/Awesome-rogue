#ifndef RESSOURCE_H
#define RESSOURCE_H

#include <string>
#include <memory>
#include <unordered_map>
#include <stdexcept>

template <typename T>
class Ressource
{
public:
    Ressource() : m_fileName("") , m_ressource(std::shared_ptr<T>()) {}
    Ressource(std::string fileName)
        : Ressource()
    {
        load(fileName);
    }

    ~Ressource()
    {
        if(m_ressource)
            m_ressource.reset();
        auto it(m_ressources.find(m_fileName));
        if(it != m_ressources.end())
            if(it->second.expired())
                m_ressources.erase(it);
    }

    void load(const std::string & fileName)
    {
        m_fileName = fileName;
        auto it(m_ressources.find(m_fileName));
        if(it != m_ressources.end())
        {
            m_ressource = it->second.lock();
            return;
        }

        m_ressource = std::make_shared<T>(T());
        if(!m_ressource->loadFromFile(fileName))
            throw std::invalid_argument("File \"" + fileName + "\" not found !");
        m_ressources.insert(std::pair<std::string, std::weak_ptr<T>>(fileName, std::weak_ptr<T>(m_ressource)));
    }

    T* operator->()
    {
        return m_ressource.get();
    }

    const T* operator->() const
    {
        return m_ressource.get();
    }

    T& operator*()
    {
        return *m_ressource;
    }

    const T& operator*() const
    {
        return *m_ressource;
    }

    T* operator()() const
    {
        return m_ressource.get();
    }

    bool isValid() const
    {
        if(m_ressource)
            return true;
        return false;
    }

private:
    std::string m_fileName;
    std::shared_ptr<T> m_ressource;
    static std::unordered_map<std::string, std::weak_ptr<T>> m_ressources;
};

template <typename T>
std::unordered_map<std::string, std::weak_ptr<T>> Ressource<T>::m_ressources;

#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Font.hpp>

typedef Ressource<sf::Texture> Texture;
typedef Ressource<sf::Font> Font;

#endif // RESSOURCE_H
