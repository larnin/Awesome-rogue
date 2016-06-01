#ifndef NONCOPIABLE_H
#define NONCOPIABLE_H

class NonCopiable
{
public:
    NonCopiable() = default;

    NonCopiable(const NonCopiable &) = delete;
    NonCopiable & operator= (const NonCopiable &) = delete;

    NonCopiable(NonCopiable &&) = default;
    NonCopiable & operator= (NonCopiable &&) = default;
};

#endif // NONCOPIABLE_H
