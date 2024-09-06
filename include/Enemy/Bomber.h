// Bomber.h
#include "Enemy.h"

class Bomber : public Enemy
{
public:
    Bomber(sf::Texture& texture);

    void update(float dt) override;
};