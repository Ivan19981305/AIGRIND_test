// Bumpable.h
#include <SFML/Graphics.hpp>
#include "Bumping.h"
#include "Enemy.h"
#include "Meteor.h"

class Bumpable
{
public:
    virtual void getBumpFrom(const Bumping&) = 0;
    virtual void getBumpFrom(const Enemy&) = 0;
    virtual void getBumpFrom(const Meteor&) = 0;
};