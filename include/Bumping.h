// Bumping.h
#include <SFML/Graphics.hpp>
#include "Bumpable.h"

class Bumping
{
public:
    virtual void bump(Bumpable& object)
    {
        object.getBumpFrom(*this);
    }
};