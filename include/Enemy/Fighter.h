// Fighter.h
#include "Enemy.h"
#include "Bullet.h"

class Fighter : public Enemy
{
public:
    Fighter(sf::Texture& texture);

    void update(float dt) override;
    void run(const Bullet& bullet, float dt) override;
};