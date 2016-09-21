#ifndef RENDERDATA_H
#define RENDERDATA_H

#include "Utilities/ressource.h"
#include <cassert>
#include <vector>

class RenderData
{
public:
    RenderData(const std::string & filename);

    bool hasAnimation(unsigned int id) const;
    unsigned int frameOf(unsigned int id, float time) const;

    Texture texture;

private:
    struct BlockFrame
    {
        BlockFrame(unsigned int _id, float _time)
            : time(_time), id(_id) {}

        float time;
        unsigned int id;
    };

    struct BlockAnimation
    {
        unsigned int id() const
        {
            assert(!frames.empty());
            return frames.front().id;
        }
        std::vector<BlockFrame> frames;
    };

    std::vector<BlockAnimation> anims;
};

#endif // RENDERDATA_H
