#include "Libs/json.hpp"
#include "fstream"
#include "patern.h"
#include "map/blocktype.h"
#include <cmath>

Patern::Patern(sf::Vector2u size, Block def)
    : m_blocks(size, def)
    , m_renderInfosName("tileset")
{

}

Patern::Patern(sf::Vector2u size)
    : m_blocks(size)
    , m_renderInfosName("tileset")
{
    generate();
}

sf::Vector2u Patern::getSize() const
{
    return m_blocks.getSize();
}

Block & Patern::operator()(const sf::Vector2u & pos)
{
    return m_blocks(pos);
}

const Block & Patern::operator()(const sf::Vector2u & pos) const
{
    return m_blocks(pos);
}

Patern Patern::transform(Rotation rot, bool flipX, bool flipY)
{
    std::array<int,4> transform={1,0,0,1};

    if(rot == Rotation::ROT_90)
        transform = multiplyMatrice({0,1,-1,0}, transform);
    else if(rot == Rotation::ROT_180)
        transform = multiplyMatrice({-1,0,0,-1}, transform);
    else if(rot == Rotation::ROT_270)
        transform = multiplyMatrice({0,-1,1,0}, transform);

    if(flipX)
        transform = multiplyMatrice({-1,0,0,1}, transform);
    if(flipY)
        transform = multiplyMatrice({1,0,0,-1}, transform);


    sf::Vector2i newSize(transformPos(getSize(), transform, sf::Vector2i(1, 1)));
    Patern newPatern(sf::Vector2u(std::abs(newSize.x), std::abs(newSize.y)));

    for(unsigned int i(0) ; i < getSize().x ; i++)
        for(unsigned int j(0) ; j < getSize().y ; j++)
            {
                Block b(m_blocks(sf::Vector2u(i, j)));
                sf::Vector2u newPos(transformPos(sf::Vector2u(i, j), transform, newSize));
                b.groundOrientation = transformData(b.groundOrientation, rot, flipX, flipY);
                b.wallOrientation = transformData(b.wallOrientation, rot, flipX, flipY);
                b.boxCaracts = transformData(b.boxCaracts, rot, flipX, flipY);
                newPatern(newPos) = b;
            }

    for(const auto & l : m_lights)
    {
        AnimatedLightData newL(l.type());
        for(const auto & f : l)
        {
            LightFrame newF(f.time);
            newF.radius = f.radius;
            newF.color = f.color;
            newF.intensity = f.intensity;
            newF.pitch = f.pitch;
            sf::Vector2f p(transformPos(sf::Vector2f(f.pos.x, f.pos.y), transform, newSize));
            newF.pos = sf::Vector3f(p.x, p.y, f.pos.z);
            newF.yaw = f.yaw;
            if(flipY)
                newF.yaw = -newF.yaw;
            if(flipX)
                newF.yaw = M_PI - newF.yaw;
            if(rot == Rotation::ROT_90)
                newF.yaw += M_PI_2;
            else if(rot == Rotation::ROT_180)
                newF.yaw += M_PI;
            else if(rot == Rotation::ROT_270)
                newF.yaw += 3*M_PI_2;

            newL.add(newF);
        }
        newPatern.m_lights.push_back(newL);
    }

    return newPatern;
}

std::vector<Patern> Patern::load(const std::string & fileName)
{
    using json = nlohmann::json;

    std::ifstream file(fileName);
    if(!file.is_open())
        return std::vector<Patern>();
    json j(json::parse(file));
    file.close();
    if(j.empty() || !j.is_array())
        return std::vector<Patern>();

    std::vector<Patern> paterns;

    for(json & patern : j)
    {
        if(!patern.is_object())
            continue;
        auto sizeX(patern.find("sizeX"));
        auto sizeY(patern.find("sizeY"));
        auto blocks(patern.find("blocks"));
        auto rType(patern.find("type"));
        auto rName(patern.find("name"));
        auto rRarity(patern.find("rarity"));
        auto lights(patern.find("lights"));

        if(sizeX == patern.end() || sizeY == patern.end() || blocks == patern.end()
           || rType == patern.end() || rName == patern.end() || rRarity == patern.end())
            continue;

        sf::Vector2u size(*sizeX, *sizeY);

        if(!blocks->is_array())
            continue;
        if(blocks->size() < size.x*size.y)
            continue;

        Patern p(size);
        for(unsigned int i(0) ; i < size.x ; i++)
            for(unsigned int j(0) ; j < size.y ; j++)
            {
                json & block((*blocks)[i+j*size.x]);
                if(!block.is_array() || block.size() < 5)
                    p(sf::Vector2u(i, j)) = Block();
                else p(sf::Vector2u(i, j)) = Block(block[0], block[1], block[2], block[3], block[4]);
            }

        if(lights != patern.end())
            for(const auto & l : *lights)
                p.m_lights.emplace_back(l);

        p.type = RoomType(rType->get<unsigned int>());
        p.name = *rName;
        p.rarity = *rRarity;
        p.m_renderInfosName = patern["render"].get<std::string>();
        paterns.push_back(p);
    }

    return paterns;
}

void Patern::save(const std::string & fileName, const std::vector<Patern> & paterns)
{
    using json = nlohmann::json;

    json datas;
    for(const Patern & p : paterns)
    {
        json j = {
          {"sizeX", p.getSize().x},
          {"sizeY", p.getSize().y},
          {"type", p.type},
          {"name", p.name},
          {"rarity", p.rarity},
          {"render", p.m_renderInfosName}
        };
        json jBlocks;
        for(const Block & b : p.m_blocks)
        {
            json jb{{b.groundID, b.groundOrientation, b.wallID, b.wallOrientation, b.boxCaracts}};
            jBlocks.push_back(jb);
        }
        j["blocks"] = jBlocks;

        json jLights;
        for(const auto & l : p.m_lights)
            jLights.push_back(l.toJson());
        j["lights"] = jLights;

        datas.push_back(j);
    }

    std::ofstream file(fileName);
    if(!file.is_open())
        return;
    file << datas.dump();
    file.close();
}

sf::Vector2i Patern::transformPos(const sf::Vector2u & pos, const std::array<int,4> & transform, const sf::Vector2i & newSize)
{
    sf::Vector2i transformedPos(transform[0]*pos.x+transform[1]*pos.y, transform[2]*pos.x+transform[3]*pos.y);

    if(newSize.x < 0)
        transformedPos.x -= newSize.x+1;
    if(newSize.y < 0)
        transformedPos.y -= newSize.y+1;
    return transformedPos;
}

sf::Vector2f Patern::transformPos(const sf::Vector2f & pos, const std::array<int,4> & transform, const sf::Vector2i & newSize)
{
    sf::Vector2f transformedPos(transform[0]*pos.x+transform[1]*pos.y, transform[2]*pos.x+transform[3]*pos.y);

    if(newSize.x < 0)
        transformedPos.x -= newSize.x+1;
    if(newSize.y < 0)
        transformedPos.y -= newSize.y+1;
    return transformedPos;
}

std::array<int,4> Patern::multiplyMatrice(const std::array<int,4> & left, const std::array<int,4> & right)
{
    return std::array<int, 4>{left[0]*right[0]+left[1]*right[2],
                              left[0]*right[1]+left[1]*right[3],
                              left[2]*right[0]+left[3]*right[2],
                              left[2]*right[1]+left[3]*right[3]};
}

void Patern::generate()
{
    const unsigned int wallCornerID(1);
    const unsigned int wallLineID(2);
    const unsigned int wallLineNoflipID(3);
    const unsigned int groundID(384);

    for(unsigned int i(0) ; i < getSize().x ; i++)
        for(unsigned int j(0) ; j < getSize().y ; j++)
        {
            if(i > 0 && i < getSize().x-1 && j > 0 && j < getSize().y-1)
                m_blocks(sf::Vector2u(i, j)) = Block(groundID, 0, 0, 0, createBoxCaracts(Rotation::ROT_0, false, false, BlockType::boxOf(groundID)));
            else if(i == 0 && j == 0)
                m_blocks(sf::Vector2u(i, j)) = Block(0, 0, wallCornerID, createOrientation(Rotation::ROT_0, false, false)
                                                     , createBoxCaracts(Rotation::ROT_0, false, false, BlockType::boxOf(wallCornerID)));
            else if(i == 0 && j == getSize().y-1)
                m_blocks(sf::Vector2u(i, j)) = Block(0, 0, wallCornerID, createOrientation(Rotation::ROT_90, false, false)
                                                     , createBoxCaracts(Rotation::ROT_90, false, false, BlockType::boxOf(wallCornerID)));
            else if(i == getSize().x-1 && j == getSize().y-1)
                m_blocks(sf::Vector2u(i, j)) = Block(0, 0, wallCornerID, createOrientation(Rotation::ROT_180, false, false)
                                                     , createBoxCaracts(Rotation::ROT_180, false, false, BlockType::boxOf(wallCornerID)));
            else if(i == getSize().x-1 && j == 0)
                m_blocks(sf::Vector2u(i, j)) = Block(0, 0, wallCornerID, createOrientation(Rotation::ROT_270, false, false)
                                                     , createBoxCaracts(Rotation::ROT_270, false, false, BlockType::boxOf(wallCornerID)));
            else if(i == 0 || i == getSize().x-1)
            {
                if(getSize().y%2==0 && j == getSize().y-2)
                    m_blocks(sf::Vector2u(i, j)) = Block(0, 0, wallLineNoflipID, createOrientation(Rotation::ROT_90, true, false)
                                                         , createBoxCaracts(Rotation::ROT_90, true, false, BlockType::boxOf(wallLineNoflipID)));
                else m_blocks(sf::Vector2u(i, j)) = Block(0, 0, wallLineID, createOrientation(Rotation::ROT_90, j%2==0, false)
                                                     , createBoxCaracts(Rotation::ROT_90, j%2==0, false, BlockType::boxOf(wallLineID)));
            }
            else if(j == 0 || j == getSize().y-1)
            {
                if(getSize().x%2==0 && i == getSize().x-2)
                    m_blocks(sf::Vector2u(i, j)) = Block(0, 0, wallLineNoflipID, createOrientation(Rotation::ROT_0, false, true)
                                                         , createBoxCaracts(Rotation::ROT_0, false, true, BlockType::boxOf(wallLineNoflipID)));
                else m_blocks(sf::Vector2u(i, j)) = Block(0, 0, wallLineID, createOrientation(Rotation::ROT_0, i%2==0, false)
                                                     , createBoxCaracts(Rotation::ROT_0, i%2==0, false, BlockType::boxOf(wallLineID)));
            }
        }
}
