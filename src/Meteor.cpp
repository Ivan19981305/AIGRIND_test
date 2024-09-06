// Meteor.cpp
#include "Meteor.h"
#include <cstdlib>


Meteor::Meteor(sf::Texture &texture) : Enemy(texture)
{
    sprite.setPosition(rand() % 600 + 200, -100); // Случайная начальная позиция сверху экрана
    sprite.setScale({30 / sprite.getGlobalBounds().width, 30 / sprite.getGlobalBounds().width});
    velocityX = rand() % 100 - 50; // Случайная скорость
    velocityY = 0;
}

void Meteor::update(float dt)
{
    velocityY += 98.f * dt;
    sprite.move(velocityX * dt, velocityY * dt);
}

void Meteor::getBumpFrom(const Meteor &)
{
    // Против самострела
}

void Meteor::bump(Enemy &enemy)
{
    enemy.getBumpFrom(*this);
}