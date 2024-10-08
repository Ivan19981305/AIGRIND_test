// Bullet.cpp
#include "Bullet.h"

Bullet::Bullet(sf::Vector2f position)
{
    shape.setSize(sf::Vector2f(5, 5));
    shape.setFillColor(sf::Color::Red);
    shape.setPosition(position);
    velocityY = -100.f; // Начальная скорость снаряда
}

void Bullet::update(float dt)
{
    // Обновление позиции снаряда с учетом гравитации
    velocityY += 98.f * dt; // Ускорение свободного падения
    shape.move(100.f * dt, velocityY * dt);
}
