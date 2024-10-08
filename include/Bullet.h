// Bullet.h
#ifndef BULLET_H
#define BULLET_H

#include <SFML/Graphics.hpp>
#include "Bumping.h"

class Bullet : public Bumping
{
public:
    sf::RectangleShape shape;
    float velocityY;

    Bullet(sf::Vector2f position);
    void update(float dt);
};

#endif // BULLET_H