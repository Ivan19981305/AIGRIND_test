// Enemy.h
#include <SFML/Graphics.hpp>
#include "Bumpable.h"
#include "Bumping.h"
#include "Bullet.h"
#include "Meteor.h"
#include "Player.h"

class Enemy : public Bumping, public Bumpable
{
public:
    sf::Sprite sprite;
    float speed;
    bool isAlive;

    Enemy(sf::Texture& texture);

    virtual void update(float dt) = 0;

    virtual void bump(Bumpable& object) override;
    virtual void bump(Enemy& enemy);
    virtual void getBumpFrom(const Bumping& ) override;
    virtual void getBumpFrom(const Enemy& );
    virtual void getBumpFrom(const Meteor& );
    virtual void bump(Player& player);

    virtual void run(const Bullet& bullet, float dt) = 0;
};