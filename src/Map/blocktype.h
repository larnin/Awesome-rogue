#ifndef BLOCKTYPE_H
#define BLOCKTYPE_H

#include "block.h"
#include "Collisions/hitbox.h"

class BlockType
{
public:
    static bool isGround(unsigned int id);
    static bool isWall(unsigned int id);
    static BoxType boxOf(unsigned int id);
    static HitBox createBox(unsigned char boxCaracts);

    static const unsigned int tileSize;
    static const unsigned int nbTile;
    
private:
    BlockType() = delete;
    ~BlockType() = delete;

};

#endif // BLOCKTYPE_H
