#include "condition.h"
#include "andcondition.h"
#include "orcondition.h"
#include "notcondition.h"
#include "animationfinishedcondition.h"
#include "waitcondition.h"
#include "waitafteractivecondition.h"
#include "propertyequalcondition.h"
#include "propertynotequalcondition.h"
#include "propertyinferiorcondition.h"
#include "propertyinferiororequalcondition.h"
#include "propertysuperiorcondition.h"
#include "propertysuperiororequalcondition.h"
#include <utility>
#include <string>

void Condition::reset()
{

}

std::pair<std::string, int> getProperty(const json & j)
{
    return std::make_pair<std::string, int>(j["property"], j["value"]);
}

std::vector<std::unique_ptr<Condition>> getConditionList(const json & j)
{
    auto it(j.find("conditions"));
    if(it == j.end() || !it->is_array())
        return std::vector<std::unique_ptr<Condition>>();

    std::vector<std::unique_ptr<Condition>> conditions;

    for(auto jItem : *it)
        conditions.push_back(std::move(Condition::load(jItem)));
    return conditions;
}

std::unique_ptr<Condition> Condition::load(const json & j)
{
    auto itType(j.find("type"));
    if(itType == j.end())
        return std::unique_ptr<Condition>();

    switch (itType->get<int>())
    {
    case AND_CONDITION:
    {
        auto condition = std::make_unique<AndCondition>();
        for(auto & c : getConditionList(j))
            condition->add(c);
        return condition;
    }
    case OR_CONDITION:
    {
        auto condition = std::make_unique<OrCondition>();
        for(auto & c : getConditionList(j))
            condition->add(c);
        return condition;
    }
    case NOT_CONDITION:
    {
        auto itCondition(j.find("condition"));
        if(itCondition == j.end())
            return std::unique_ptr<Condition>();
        return std::make_unique<NotCondition>(load(*itCondition));
    }
    case ANIMATION_FINISHED_CONDITION:
    {
        return std::make_unique<AnimationFinishedCondition>();
    }
    case WAIT_CONDITION:
    {
        return std::make_unique<WaitCondition>(j["time"].get<float>());
    }
    case WAIT_AFTER_ACTIVE_CONDITION:
    {
        auto itCondition(j.find("condition"));
        if(itCondition == j.end())
            return std::unique_ptr<Condition>();
        return std::make_unique<WaitAfterActiveCondition>(load(*itCondition), j["time"].get<float>());
    }
    case PROPERTY_EQUAL_CONDITION:
    {
        auto property = getProperty(j);
        return std::make_unique<PropertyEqualCondition>(property.first, property.second);
    }
    case PROPERTY_NOT_EQUAL_CONDITION:
    {
        auto property = getProperty(j);
        return std::make_unique<PropertyNotEqualCondition>(property.first, property.second);
    }
    case PROPERTY_INFERIOR_CONDITION:
    {
        auto property = getProperty(j);
        return std::make_unique<PropertyInferiorCondition>(property.first, property.second);
    }
    case PROPERTY_INFERIOR_OR_EQUAL_CONDITION:
    {
        auto property = getProperty(j);
        return std::make_unique<PropertyInferiorOrEqualCondition>(property.first, property.second);
    }
    case PROPERTY_SUPERIOR_CONDITION:
    {
        auto property = getProperty(j);
        return std::make_unique<PropertySuperiorCondition>(property.first, property.second);
    }
    case PROPERTY_SUPERIOR_OR_EQUAL_CONDITION:
    {
        auto property = getProperty(j);
        return std::make_unique<PropertySuperiorOrEqualCondition>(property.first, property.second);
    }
    default:
        return std::unique_ptr<Condition>();
    }
}
