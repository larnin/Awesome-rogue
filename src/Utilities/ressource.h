#ifndef RESSOURCE_H
#define RESSOURCE_H

#include <string>
#include <memory>
#include <map>
#include <stdexcept>

template <typename T>
class Ressource
{
public:
    Ressource() : m_fileName(""), m_ressource(std::shared_ptr<T>())
    {}

    Ressource(std::string fileName)
        : Ressource()
    {
        load(fileName);
    }

    Ressource(const T & value)
        : m_fileName("")
        , m_ressource(std::make_shared<T>(value))
    {

    }

    Ressource(const Ressource & r)
        : m_fileName(r.m_fileName)
        , m_ressource(r.m_ressource)
    {

    }

    Ressource & operator=(const Ressource & r)
    {
        if(&r == this)
            return *this;

        freeMyRessource();

        m_fileName = r.m_fileName;
        m_ressource = r.m_ressource;
        return *this;
    }

    ~Ressource()
    {
        freeMyRessource();
    }

    Ressource clone(const Ressource & r)
    {
        return Ressource(*m_ressource);
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

    void freeMyRessource()
    {
        if(m_ressource)
            m_ressource.reset();
        auto it(m_ressources.find(m_fileName));
        if(it != m_ressources.end())
            if(it->second.expired())
                m_ressources.erase(it);
    }

    std::string m_fileName;
    std::shared_ptr<T> m_ressource;
    static std::map<std::string, std::weak_ptr<T>> m_ressources;
};

template <typename T>
std::map<std::string, std::weak_ptr<T>> Ressource<T>::m_ressources;

#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Font.hpp>

typedef Ressource<sf::Texture> Texture;
typedef Ressource<sf::Font> Font;

#endif // RESSOURCE_H
