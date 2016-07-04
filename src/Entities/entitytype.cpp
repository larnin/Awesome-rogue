#include "entitytype.h"

std::vector<float> populationProbability(float dist)
{
    std::vector<float> proba(E_MAX+1, 0.0f);

    proba[E_CIRCLE_MOB] = 1.0f;
    proba[E_CROSS_LASER_MOB] = 1.0f;
    proba[E_PUNCHBALL_MOB] = 1.0f;
    proba[E_SQUARE_MOB] = 1.0f;
    proba[E_TRACKER_MOB] = 1.0f;

    return proba;
}
