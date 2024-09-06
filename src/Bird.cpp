// Bird.cpp
#include "Bird.h"
#include <cstdlib>
#include <cmath>

Bird::Bird(sf::Texture &texture) : Enemy(texture)
{
    sprite.setPosition(800, rand() % 100 + 400); // Случайная начальная высота
    sprite.setScale({50 / sprite.getGlobalBounds().width, 50 / sprite.getGlobalBounds().width});
    speed = 100.f;
    amplitude = 20.f;
    frequency = 2.f;
}

void Bird::update(float dt)
{
    // Движение птицы с колебаниями по высоте
    sprite.move(-speed * dt, amplitude * sin(frequency * sprite.getPosition().x * 0.01f) * dt);
}

void Bird::getBumpFrom(const Bumping &)
{
    // Птицы неубиваемы
}
void Bird::getBumpFrom(const Enemy &)
{
    // Даже от самолётов
}

void Bird::getBumpFrom(const Meteor &)
{
    // Даже от метеоров
}