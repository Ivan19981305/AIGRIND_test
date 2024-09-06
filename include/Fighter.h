// Fighter.h
#ifndef FIGHTER_H
#define FIGHTER_H

#include "Enemy.h"

class Fighter : public Enemy
{
public:
    Fighter(sf::Texture &texture);
    void update(float dt) override;
    void run(const Bullet &bullet, float dt) override final;
};

#endif // FIGHTER_H