#include "animator.h"
#include <fstream>

Animator::Animator(const std::string & filename)
    : m_currentAnimationID(0)
{
    std::ifstream file(filename);
    if(!file.is_open())
        return;
    json j(json::parse(file));
    file.close();

    m_texture = Texture(j["texture"].get<std::string>());

    auto itAnims(j.find("animations"));
    if(itAnims == j.end() || !itAnims->is_array())
        return;

    for(auto & anim : *itAnims)
    {
        AnimationState state;
        state.animation = Animation(anim["animation"]);
        auto itTransition(j.find("transitions"));
        if(itTransition != anim.end() && itTransition->is_array())
            for(auto & transition : *itTransition)
                state.transitions.push_back(Transition(transition));
        m_animations.push_back(std::move(state));
    }
}

Frame Animator::getCurrent() const
{
    return m_animations[m_currentAnimationID].animation.getCurrent();
}

void Animator::update(const sf::Time & elapsedTime)
{
    m_animations[m_currentAnimationID].animation.addTime(elapsedTime.asSeconds());
    for(auto & t : m_animations[m_currentAnimationID].transitions)
    {
        if(t.update(*this, elapsedTime))
        {
            changeState(t.getNextAnimation());
            break;
        }
    }
}

void Animator::setProperty(const std::string & id, unsigned int value)
{
    m_proprieties[id] = value;
}

int Animator::getProperty(const std::string & id) const
{
    auto it = m_proprieties.find(id);
    if(it == m_proprieties.end())
        return 0;
    return it->second;
}

Animation const & Animator::getCurrentAnimation() const
{
    return m_animations[m_currentAnimationID].animation;
}

void Animator::changeState(unsigned int animationID)
{
    m_currentAnimationID = animationID;
    for(auto & t : m_animations[m_currentAnimationID].transitions)
        t.reset();
}
