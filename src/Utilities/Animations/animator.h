#ifndef ANIMATOR_H
#define ANIMATOR_H

#include "Systemes/updatable.h"
#include "animation.h"
#include "transition.h"
#include "Utilities/ressource.h"
#include <vector>
#include <unordered_map>

struct AnimationState
{
    std::vector<Transition> transitions;
    Animation animation;
};

class Animator : public Updatable
{
public:
    Animator(const std::string & filename);
    virtual ~Animator() = default;

    Frame getCurrent() const;
    Animation const & getCurrentAnimation() const;
    void setProperty(const std::string & id, unsigned int value);
    int getProperty(const std::string & id) const;

    void update(const sf::Time & elapsedTime);
    void changeState(unsigned int animationID);

private:
    std::vector<AnimationState> m_animations;
    unsigned int m_currentAnimationID;
    std::unordered_map<std::string, int> m_proprieties;
    Texture m_texture;
};

#endif // ANIMATOR_H
