// Bumpable.h
#ifndef BUMPABLE_H
#define BUMPABLE_H

class Bumping; // объявление класса

class Bumpable
{
public:
    virtual void getBumpFrom(const Bumping &) = 0;
};

#endif // BUMPABLE_H