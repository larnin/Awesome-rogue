#ifndef BLOCKANIMATION_H
#define BLOCKANIMATION_H

#include "Map/rotation.h"
#include <vector>

struct BlockFrame
{
    BlockFrame(float _minTime, float _maxTime, unsigned int _id, Rotation _rot, bool _flipX, bool _flipY)
        : minTime(_minTime)
        , maxTime(_maxTime)
        , id(_id)
        , rot(_rot)
        , flipX(_flipX)
        , flipY(_flipY)
    {

    }

    BlockFrame()
        : minTime(0.1)
        , maxTime(0.1)
        , id(0)
        , rot(ROT_0)
        , flipX(false)
        , flipY(false)
    {

    }

    float minTime;
    float maxTime;
    unsigned int id;
    Rotation rot;
    bool flipX;
    bool flipY;
};

struct SingleBlockAnimation
{
    SingleBlockAnimation(float _weight)
        : weight(_weight) {}

    float weight;
    std::vector<BlockFrame> frames;
};

struct BlockAnimation
{
    BlockAnimation(unsigned int _id)
        : id(_id) {}

    unsigned int id;
    std::vector<SingleBlockAnimation> anims;
};



#endif // BLOCKANIMATION_H
