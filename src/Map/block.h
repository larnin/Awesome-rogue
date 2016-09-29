#ifndef BLOCK_H
#define BLOCK_H

#include "rotation.h"
#include "boxtype.h"

/* chars orientation  et boxCaracts
 *  [0-1] orientation (4 valeurs)
 *  [2] xFlip
 *  [3] yFlip
 *
 * Pour boxCaracts
 *  [4-7] BoxType (16 valeurs)
 * */

/* si wallID ou groundID est egal a 0, aucun rendu est fait */


struct Block
{
    Block();
    Block(unsigned int _groundID, unsigned char _groundOrientation);
    Block(unsigned int _groundID, unsigned char _groundOrientation, unsigned int _wallID, unsigned char _wallOrientation, unsigned char _boxCaracts);
    ~Block() = default;

    unsigned int groundID;
    unsigned char groundOrientation;
    unsigned int wallID;
    unsigned char wallOrientation;
    unsigned char boxCaracts;
};

unsigned char createOrientation(Rotation rot, bool xFlip, bool yFlip);
unsigned char createBoxCaracts(Rotation rot, bool xFlip, bool yFlip, BoxType type);

void setRotation(unsigned char & value, Rotation rot);
Rotation getRotation(unsigned char value);
void setXFlip(unsigned char & value, bool xFlip);
bool getXFlip(unsigned char value);
void setYFlip(unsigned char & value, bool yFlip);
bool getYFlip(unsigned char value);
void setBoxType(unsigned char & value, BoxType type);
BoxType getBoxType(unsigned char value);

unsigned char transformData(unsigned char data, Rotation rot, bool flipX, bool flipY);
unsigned char transformData(unsigned char data, unsigned char transform);

#endif // BLOCK_H
