#ifndef MATRIX
#define MATRIX

#include <SFML/System/Vector2.hpp>
#include <cassert>
#include <vector>

template <typename T>
class Matrix
{
public:
    Matrix(sf::Vector2u size, T defaultValue = T())
        : m_datas(size.x*size.y, defaultValue)
        , m_size(size)
    {}

    typename std::vector<T>::reference operator()(const sf::Vector2u & pos)
    {
        assert(pos.x < m_size.x && pos.y < m_size.y);
        return m_datas[pos.x + pos.y * m_size.x];
    }

    typename std::vector<T>::const_reference operator ()(const sf::Vector2u & pos) const
    {
        assert(pos.x < m_size.x && pos.y < m_size.y);
        return m_datas[pos.x + pos.y * m_size.x];
    }

    sf::Vector2u getSize() const
    {
        return m_size;
    }

    typename std::vector<T>::iterator begin()
    {
        return m_datas.begin();
    }

    typename std::vector<T>::iterator end()
    {
        return m_datas.end();
    }

    typename std::vector<T>::const_iterator begin() const
    {
        return m_datas.begin();
    }

    typename std::vector<T>::const_iterator end() const
    {
        return m_datas.end();
    }

private:
    std::vector<T> m_datas;
    sf::Vector2u m_size;

};

#endif // MATRIX

