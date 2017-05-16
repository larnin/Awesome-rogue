#ifndef RENDERDATA_H
#define RENDERDATA_H

#include "Utilities/ressource.h"
#include "blockanimation.h"
#include <cassert>
#include <vector>

class RenderData
{
public:
    RenderData(const std::string & filename);

    bool hasAnimation(unsigned int id) const;
    void addTime(float time);
    BlockFrame frameOf(unsigned int id) const;

    Texture texture;
    Texture secondaryTexture;

private:
    void initAnimation();

    struct PlayedAnimation
    {
        PlayedAnimation(const SingleBlockAnimation & _anim)
            : anim(_anim), time(0) {}

        bool finished() const;
        BlockFrame current() const;

        SingleBlockAnimation anim;
        float time;
    };

    PlayedAnimation create(unsigned int id);

    std::vector<BlockAnimation> m_animations;
    std::vector<std::pair<unsigned int, PlayedAnimation>> m_currentAnimation;
    std::vector<std::pair<unsigned int, BlockFrame>> m_currentFrame;

    static std::default_random_engine m_rand;
};

#endif // RENDERDATA_H
