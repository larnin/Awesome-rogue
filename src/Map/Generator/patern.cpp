#include <cmath>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonParseError>
#include <QJsonArray>

#include "patern.h"
#include "map/blocktype.h"
Patern::Patern(sf::Vector2u size, Block def)
    : m_blocks(size, def)
{

}

Patern::Patern(sf::Vector2u size)
    : m_blocks(size)
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
    return newPatern;
}

std::vector<Patern> Patern::load(const std::string & fileName)
{
    QFile file(QString::fromStdString(fileName));
    if(!file.exists())
        return std::vector<Patern>();
    if(!file.open(QIODevice::ReadOnly))
        return std::vector<Patern>();

    QJsonParseError error;
    QJsonDocument doc(QJsonDocument::fromJson(file.readAll(), &error));
    file.close();
    if(error.error != QJsonParseError::NoError)
        return std::vector<Patern>();
    if(!doc.isArray())
        return std::vector<Patern>();

    QJsonArray docArray(doc.array());
    std::vector<Patern> paterns;
    for(const auto & jsonPatern : docArray)
    {
        if(!jsonPatern.isObject())
            continue;
        QJsonObject jsonPaternObject(jsonPatern.toObject());
        sf::Vector2u size(jsonPaternObject.value("sizeX").toInt(), jsonPaternObject.value("sizeY").toInt());
        if(size.x == 0 || size.y == 0)
            continue;
        QJsonArray blocks(jsonPaternObject.value("blocks").toArray());
        if((unsigned int)(blocks.size()) != size.x*size.y)
            continue;
        Patern p(size);
        for(unsigned int i(0) ; i < (unsigned int)(blocks.size()) ; i++)
        {
            sf::Vector2u pos(i%size.x, i/size.x);
            QJsonArray block(blocks[i].toArray());
            if(block.size() != 5)
            {
                p(pos) = Block();
                continue;
            }
            Block b;
            b.groundID = block[0].toInt();
            b.groundOrientation = block[1].toInt();
            b.wallID = block[2].toInt();
            b.wallOrientation = block[3].toInt();
            b.boxCaracts = block[4].toInt();
            p(pos) = b;
        }
        p.type = RoomType(jsonPaternObject.value("type").toInt());
        p.name = jsonPaternObject.value("name").toString().toStdString();
        p.rarity = jsonPaternObject.value("rarity").toInt();
        paterns.push_back(p);
    }
    return paterns;
}

void Patern::save(const std::string & fileName, const std::vector<Patern> & paterns)
{
    QJsonArray jsonPaterns;
    for(const auto & p : paterns)
    {
        QJsonObject jsonPatern;
        jsonPatern.insert("sizeX", int(p.getSize().x));
        jsonPatern.insert("sizeY", int(p.getSize().y));
        jsonPatern.insert("type", p.type);
        jsonPatern.insert("name", QString::fromStdString(p.name));
        jsonPatern.insert("rarity", int(p.rarity));

        QJsonArray jsonBlocks;
        for(const Block & b : p.m_blocks)
        {
            QJsonArray jsonBlock;
            jsonBlock.append(int(b.groundID));
            jsonBlock.append(b.groundOrientation);
            jsonBlock.append(int(b.wallID));
            jsonBlock.append(b.wallOrientation);
            jsonBlock.append(b.boxCaracts);
            jsonBlocks.append(jsonBlock);
        }
        jsonPatern.insert("blocks", jsonBlocks);
        jsonPaterns.append(jsonPatern);
    }
    QFile file(QString::fromStdString(fileName));
    if(!file.open(QIODevice::WriteOnly))
        return;
    file.write(QJsonDocument(jsonPaterns).toJson(QJsonDocument::Compact));
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
