#ifndef TRANSITION_H
#define TRANSITION_H

#include "Libs/json.hpp"
#include "Conditions/condition.h"
#include <SFML/System/Time.hpp>
#include <memory>

using json = nlohmann::json;

class Animator;

class Transition
{
public:
    Transition(const json & j);
    Transition(unsigned int targetAnimationID, std::unique_ptr<Condition> && condition);
    Transition(Transition &&) = default;
    Transition & operator =(Transition &&) = default;

    bool update(const Animator & animator, const sf::Time & elapsedTime);
    void reset();
    inline unsigned int getNextAnimation() const { return m_targetAnimationID; }
private:
    unsigned int m_targetAnimationID;
    std::unique_ptr<Condition> m_condition;
};

#endif // TRANSITION_H
