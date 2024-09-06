// Enemy.h
#ifndef ENEMY_H
#define ENEMY_H

#include <SFML/Graphics.hpp>
#include "Bumping.h"
#include "Bumpable.h"
#include "Bullet.h"
#include "Player.h"

class Meteor;

class Enemy : public Bumping, public Bumpable
{
public:
    sf::Sprite sprite;
    float speed;
    bool isAlive;

    Enemy(sf::Texture &texture);
    virtual void update(float dt) = 0;

    virtual void bump(Bumpable &object) override;
    virtual void bump(Enemy &enemy);
    virtual void bump(Player &player);

    virtual void getBumpFrom(const Bumping &) override;
    virtual void getBumpFrom(const Enemy &);
    virtual void getBumpFrom(const Meteor &);

    virtual void run(const Bullet&, float);
};

#endif // ENEMY_H