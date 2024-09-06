// Enemy.cpp
#include "Enemy.h"
#include <SFML/Graphics.hpp>
#include <random>

Enemy::Enemy(sf::Texture& texture)
{
    sprite.setTexture(texture);
    isAlive = true;
}

void Enemy::bump(Bumpable& object) override
{
    object.getBumpFrom(*this);
}

void Enemy::bump(Enemy& enemy)
{
    enemy.getBumpFrom(*this);
}

void Enemy::getBumpFrom(const Bumping& ) override
{
    isAlive = false;
}

void Enemy::getBumpFrom(const Enemy& )
{
    // чтобы не отлетали друг от друга
}

void Enemy::getBumpFrom(const Meteor& )
{
    isAlive = false;
}

void Enemy::bump(Player& player)
{
    player.getBumpFrom(*this);
}

void Enemy::run(const Bullet&, float)
{
    // по умолчанию бездействуем
}