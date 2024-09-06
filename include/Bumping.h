// Bumping.h
#ifndef BUMPING_H
#define BUMPING_H

#include "Bumpable.h"

class Bumping
{
public:
    virtual void bump(Bumpable &object)
    {
        object.getBumpFrom(*this);
    }
};

#endif // BUMPING_H