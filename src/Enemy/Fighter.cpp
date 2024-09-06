// Fighter.cpp
#include "Fighter.h"
#include <SFML/Graphics.hpp>
#include <cmath>
#include <random>

Fighter::Fighter(sf::Texture& texture) : Enemy(texture)
{
    sprite.setPosition(800, rand() % 600); // Случайная начальная высота
    sprite.setScale({100 / sprite.getGlobalBounds().width, 100 / sprite.getGlobalBounds().width});
    speed = 150 + rand() % 50; // Случайная скорость
}

void Fighter::update(float dt)
{
    sprite.move(-speed * dt, 0);
}

void Fighter::run(const Bullet& bullet, float dt)
{
    sf::Vector2f bulletDirection = bullet.shape.getPosition() - sprite.getPosition();
    float angle = atan2f(bulletDirection.y, bulletDirection.x);

    float speedFactor = 1.f + sqrt(pow(bulletDirection.x, 2) + pow(bulletDirection.y, 2)) * 0.01f; 

    sprite.move(-speed * cos(angle) * speedFactor * dt * 0.05f, -speed * sin(angle) * speedFactor * dt * 0.05f);
}