// Bomber.cpp
#include "Bomber.h"
#include <SFML/Graphics.hpp>
#include <random>

Bomber::Bomber(sf::Texture& texture) : Enemy(texture)
{
    if (rand() % 100 > 50){
        sprite.setPosition(800, rand() % 150); // Случайная начальная высота
        speed = 200; // высокая скорость
    }
    else
    {
        sprite.setPosition(800, 225 +  rand() % 150); // Случайная начальная высота
        speed = 150; // средняя скорость
    }
    sprite.setScale({150 / sprite.getGlobalBounds().width, 150 / sprite.getGlobalBounds().width});
}

void Bomber::update(float dt)
{
    sprite.move(-speed * dt, 0);
}