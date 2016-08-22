#ifndef EVENTEXPERIENCECHANGED_H
#define EVENTEXPERIENCECHANGED_H

struct EventExperienceChanged
{
    EventExperienceChanged(unsigned int newValue)
        : value(newValue) {}

    unsigned int value;
};

#endif // EVENTEXPERIENCECHANGED_H
