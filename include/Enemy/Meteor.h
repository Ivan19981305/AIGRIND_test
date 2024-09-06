// Meteor.h
#include "Enemy.h"

class Meteor : public Enemy
{
public:
    float velocityX, velocityY;

    Meteor(sf::Texture& texture);

    void update(float dt) override;
    void getBumpFrom(const Meteor& ) override final;
    void bump(Enemy& enemy) override final;
};