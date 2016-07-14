#ifndef BLOCKTYPE_H
#define BLOCKTYPE_H

#include "block.h"
#include "Collisions/hitbox.h"

enum BlockInteractionType
{
    BI_NO_INTERACTION,
    BI_CHEST,
    BI_PORTAL,
    BI_SAVEPOINT,
    BI_START_BOSS1
};

class BlockType
{
public:
    static bool isGround(unsigned int id);
    static bool isWall(unsigned int id);
    static BoxType boxOf(unsigned int id);
    static HitBox createBox(unsigned char boxCaracts);

    static BlockInteractionType getInteraction(const Block & b);
    static std::string interactionName(BlockInteractionType b);

    static const unsigned int tileSize;
    static const unsigned int nbTile;
    
private:
    static BlockInteractionType getInteraction(unsigned int id);

    BlockType() = delete;
    ~BlockType() = delete;

};

#endif // BLOCKTYPE_H
