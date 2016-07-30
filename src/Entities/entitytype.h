#ifndef ENTITYTYPE_H
#define ENTITYTYPE_H

#include <vector>

enum EntityType
{
    E_CIRCLE_MOB = 0,
    E_SMALL_CIRCLE_MOB = 1,
    E_CROSS_LASER_MOB = 2,
    E_PUNCHBALL_MOB = 3,
    E_SQUARE_MOB = 4,
    E_TRACKER_MOB = 5,
    E_BOSS1_PARTS = 6,
    E_BOSS1_FINAL = 7,
    E_MAX = 7
};

std::vector<float> populationProbability(float dist);

#endif // ENTITYTYPE_H
