#ifndef PATERN_H
#define PATERN_H

#include "Utilities/matrix.h"
#include "Map/block.h"
#include "Map/roomtype.h"
#include <vector>
#include <array>

class Patern
{
    friend class Room;
public:

    Patern(sf::Vector2u size);
    ~Patern() = default;
    sf::Vector2u getSize() const;
    Block & operator()(const sf::Vector2u & pos);
    const Block & operator()(const sf::Vector2u & pos) const;
    Patern transform(Rotation rot, bool flipX, bool flipY);

    static std::vector<Patern> load(const std::string & fileName);
    static void save(const std::string & fileName, const std::vector<Patern> & paterns);

    RoomType type;
    std::string name;
    unsigned int rarity;

private:
    Patern(sf::Vector2u size, Block def);
    void generate();

    static sf::Vector2i transformPos(const sf::Vector2u & pos, const std::array<int,4> & transform, const sf::Vector2i & newSize);
    static std::array<int,4> multiplyMatrice(const std::array<int,4> & left, const std::array<int,4> & right);

    Matrix<Block> m_blocks;
    std::string m_renderInfosName;
};

#endif // PATERN_H
