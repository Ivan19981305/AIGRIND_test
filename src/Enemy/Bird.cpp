// Bird.cpp
#include "Bird.h"
#include <SFML/Graphics.hpp>
#include <random>

Bird::Bird(sf::Texture& texture) : Enemy(texture)
{
    sprite.setPosition(800, rand() % 100 + 400); // Случайная начальная высота
    sprite.setScale({50 / sprite.getGlobalBounds().width, 50 / sprite.getGlobalBounds().width});
    speed = 100.f;
    amplitude = 20.f;
    frequency = 2.f;
}

void Bird::update(float dt)
{
    sprite.move(-speed * dt, amplitude * sin(frequency * sprite.getPosition().x * 0.01f) * dt);
}

void Bird::getBumpFrom(const Bumping& ) override final
{
    // Птицы неубиваемы
}
void Bird::getBumpFrom(const Enemy& ) override final
{
    // Даже от самолётов
}
void Bird::getBumpFrom(const Meteor& ) override final
{
    // Даже от метеоров
}