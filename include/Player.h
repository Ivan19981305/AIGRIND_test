// Player.h
#include <SFML/Graphics.hpp>
#include "Bumpable.h"
#include "Bumping.h"
#include "Enemy.h"

class Player : public Bumpable
{
public:
    sf::Sprite sprite;
    float velocity;
    bool isAlive;

    Player(sf::Texture& texture);

    void update(float dt, int windowPositionY);

    void getBumpFrom(const Bumping&) override final;
    void getBumpFrom(const Enemy&);
};