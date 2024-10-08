// Player.cpp
#include "Player.h"
#include <SFML/Window/Mouse.hpp>

Player::Player(sf::Texture &texture)
{
    sprite.setTexture(texture);
    sprite.setPosition(100, 200);
    sprite.setScale({100 / sprite.getGlobalBounds().width, 100 / sprite.getGlobalBounds().width});
    velocity = 0;
    isAlive = true;
}

void Player::update(float dt, int windowPositionY)
{

    // Обновление позиции самолета с учетом инерции
    velocity += (sf::Mouse::getPosition().y - windowPositionY - sprite.getPosition().y) * 0.05f;
    velocity *= 0.9f;
    sprite.move(0, velocity * dt);

    // sprite.setPosition(sprite.getPosition().x, sf::Mouse::getPosition().y - windowPositionY);

    // Ограничение движения самолета по вертикали
    if (sprite.getPosition().y < 0)
    {
        sprite.setPosition(sprite.getPosition().x, 0);
        velocity = 0;
    }
    if (sprite.getPosition().y > 500)
    {
        isAlive = false;
        sprite.setPosition(sprite.getPosition().x, 500);
        velocity = 0;
    }
}

void Player::getBumpFrom(const Bumping &)
{
    // от снарядов не помрёт
}

void Player::getBumpFrom(const Enemy &)
{
    isAlive = false;
}