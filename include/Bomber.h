// Bomber.h
#ifndef BOMBER_H
#define BOMBER_H

#include "Enemy.h"

class Bomber : public Enemy
{
public:
    Bomber(sf::Texture &texture);
    void update(float dt) override;
};

#endif // BOMBER_H