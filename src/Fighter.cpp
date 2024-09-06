// Fighter.cpp
#include "Fighter.h"
#include <cstdlib>
#include <cmath>

Fighter::Fighter(sf::Texture &texture) : Enemy(texture)
{
    sprite.setPosition(800, rand() % 600); // Случайная начальная высота
    sprite.setScale({100 / sprite.getGlobalBounds().width, 100 / sprite.getGlobalBounds().width});
    speed = 150 + rand() % 50; // Случайная скорость
}

void Fighter::update(float dt)
{
    // TODO: Реализовать уклонение от снарядов
    sprite.move(-speed * dt, 0);
}

void Fighter::run(const Bullet &bullet, float dt)
{
    // Вычисляем направление вектора от истребителя к пуле
    sf::Vector2f bulletDirection = bullet.shape.getPosition() - sprite.getPosition();
    float angle = atan2f(bulletDirection.y, bulletDirection.x);

    // Увеличиваем скорость, если пуля близко, чтобы быстрее убегать
    float speedFactor = 1.f + sqrt(pow(bulletDirection.x, 2) + pow(bulletDirection.y, 2)) * 0.01f; // Коэффициент для увеличения скорости

    // Двигаемся в противоположную от пули сторону
    sprite.move(-speed * cos(angle) * speedFactor * dt * 0.05f, -speed * sin(angle) * speedFactor * dt * 0.05f);
}