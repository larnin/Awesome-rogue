#include "block.h"

Block::Block()
    : groundID(0)
    , groundOrientation(0)
    , wallID(0)
    , wallOrientation(0)
    , boxCaracts(0)
{

}

Block::Block(unsigned int _groundID, unsigned char _groundOrientation)
    : Block(_groundID, _groundOrientation, 0, 0, 0)
{

}

Block::Block(unsigned int _groundID, unsigned char _groundOrientation, unsigned int _wallID, unsigned char _wallOrientation, unsigned char _boxCaracts)
    : groundID(_groundID)
    , groundOrientation(_groundOrientation)
    , wallID(_wallID)
    , wallOrientation(_wallOrientation)
    , boxCaracts(_boxCaracts)
{

}

const unsigned char maskRot(0x03);
const unsigned char maskXFlip(0x04);
const unsigned char maskYFlip(0x08);
const unsigned char maskBoxType(0xF0);
const unsigned int moveBoxType(4);

unsigned char createOrientation(Rotation rot, bool xFlip, bool yFlip)
{
    unsigned char c(0);
    setRotation(c, rot);
    setXFlip(c, xFlip);
    setYFlip(c, yFlip);
    return c;
}

unsigned char createBoxCaracts(Rotation rot, bool xFlip, bool yFlip, BoxType type)
{
    unsigned char c(createOrientation(rot, xFlip, yFlip));
    setBoxType(c, type);
    return c;
}

void setRotation(unsigned char & value, Rotation rot)
{
    unsigned char mask(value - (value & maskRot));
    value = mask + rot;
}

Rotation getRotation(unsigned char value)
{
    return Rotation(value & maskRot);
}

void setXFlip(unsigned char & value, bool xFlip)
{
    unsigned char mask(value - (value & maskXFlip));
    value = mask + maskXFlip*xFlip;
}

bool getXFlip(unsigned char value)
{
    return value & maskXFlip;
}

void setYFlip(unsigned char & value, bool yFlip)
{
    unsigned char mask(value - (value & maskYFlip));
    value = mask + maskYFlip*yFlip;
}

bool getYFlip(unsigned char value)
{
    return value & maskYFlip;
}

void setBoxType(unsigned char & value, BoxType type)
{
    unsigned char mask(value - (value & maskBoxType));
    value = mask + (type << moveBoxType);
}

BoxType getBoxType(unsigned char value)
{
    return BoxType((value & maskBoxType) >> moveBoxType);
}

unsigned char transformData(unsigned char data, Rotation rot, bool flipX, bool flipY)
{
    unsigned int newRot(getRotation(data));
    if(newRot < rot)
        newRot += 4;
    newRot -= rot;

    bool newFlipX(!((flipX && getXFlip(data)) || (!flipX && !getXFlip(data))));
    bool newFlipY(!((flipY && getYFlip(data)) || (!flipY && !getYFlip(data))));
    setRotation(data, Rotation(newRot));
    setXFlip(data, newFlipX);
    setYFlip(data, newFlipY);
    return data;
}
