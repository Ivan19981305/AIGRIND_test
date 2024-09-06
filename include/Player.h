// Player.h
#ifndef PLAYER_H
#define PLAYER_H

#include <SFML/Graphics.hpp>
#include "Bumpable.h"

class Enemy;

class Player : public Bumpable
{
public:
    sf::Sprite sprite;
    float velocity;
    bool isAlive;

    Player(sf::Texture &texture);
    void update(float dt, int windowPositionY);
    void getBumpFrom(const Bumping &) override final;
    void getBumpFrom(const Enemy &);
};

#endif // PLAYER_H