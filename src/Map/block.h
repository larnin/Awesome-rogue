#ifndef BLOCK_H
#define BLOCK_H

enum BoxType
{
    EMPTY = 0,
    FULL = 1,
    TRIANGLE = 2,
    HALF = 3,
    QUARTER = 4,
    THREE_QUARTER = 5,
    HALF_CENTRED = 6,
    CROSS = 7,
    T_SHARP = 8,
    CORNER_CENTRED = 9
};

enum Rotation
{
    ROT_0 = 0,
    ROT_90 = 1,
    ROT_180 = 2,
    ROT_270 = 3
};

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


#endif // BLOCK_H
