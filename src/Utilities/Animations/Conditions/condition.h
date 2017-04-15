#ifndef CONDITION_H
#define CONDITION_H

#include "Libs/json.hpp"
#include <SFML/System/Time.hpp>
#include <memory>

using json = nlohmann::json;

class Animator;

class Condition
{
public:
    virtual ~Condition() = default;

    virtual bool check(const Animator & animator, const sf::Time & elapsedTime) = 0;
    virtual void reset();

    static std::unique_ptr<Condition> load(const json & j);

private:
    enum ConditionType
    {
        AND_CONDITION = 0,
        OR_CONDITION = 1,
        NOT_CONDITION = 2,
        ANIMATION_FINISHED_CONDITION = 10,
        WAIT_CONDITION = 20,
        WAIT_AFTER_ACTIVE_CONDITION = 21,
        PROPERTY_EQUAL_CONDITION = 30,
        PROPERTY_NOT_EQUAL_CONDITION = 31,
        PROPERTY_INFERIOR_CONDITION = 32,
        PROPERTY_INFERIOR_OR_EQUAL_CONDITION = 33,
        PROPERTY_SUPERIOR_CONDITION = 34,
        PROPERTY_SUPERIOR_OR_EQUAL_CONDITION = 35
    };
};

#endif // CONDITION_H
