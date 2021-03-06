#ifndef BOSS1TRANSITION_H
#define BOSS1TRANSITION_H

#include "particule.h"
#include "Utilities/ressource.h"
#include "Utilities/Animations/animation.h"

class Boss1Transition : public Particule
{
public:
    Boss1Transition(const Location & pos, float originalOrientation, float distance, float time);
    Boss1Transition(Boss1Transition &&) = default;
    Boss1Transition & operator =(Boss1Transition &&) = default;
    virtual ~Boss1Transition() = default;

    virtual void update(const sf::Time & elapsedTime) override;
    virtual void draw(sf::RenderTarget & target, sf::RenderStates) const override;

protected:
    Texture m_texture;
    float m_time;
    float m_startTime;
    Animation m_animation;
    float m_startOrientation;
    float m_startDistance;
};

#endif // BOSS1TRANSITION_H
