// Bird.h
#include "Enemy.h"

class Bird : public Enemy
{
public:
    float amplitude;
    float frequency;

    Bird(sf::Texture& texture);

    void update(float dt) override;
    void getBumpFrom(const Bumping& ) override final;
    void getBumpFrom(const Enemy& ) override final;
    void getBumpFrom(const Meteor& ) override final;
};