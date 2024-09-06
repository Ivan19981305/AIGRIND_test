// main.cpp
#include <SFML/Graphics.hpp>
#include <vector>
#include <random>
#include <memory>
#include "Player.h"
#include "Bullet.h"
#include "Enemy.h"
#include "Bird.h"
#include "Bomber.h"
#include "Fighter.h"
#include "Meteor.h"

void checkBump(Enemy& enemy1, Enemy& enemy2);
void checkBump(Bullet& bullet, Enemy& enemy);
void checkBump(Enemy& enemy, Player& player);

int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 600), "2D Scroller");
    window.setFramerateLimit(60);

    // Загрузка текстур (замените на ваши)
    sf::Texture playerTexture;
    if (!playerTexture.loadFromFile("player.png"))
    {
        return EXIT_FAILURE;
    }
    sf::Texture birdTexture;
    if (!birdTexture.loadFromFile("bird.png"))
    {
        return EXIT_FAILURE;
    }
    sf::Texture bomberTexture;
    if (!bomberTexture.loadFromFile("bomber.png"))
    {
        return EXIT_FAILURE;
    }
    sf::Texture fighterTexture;
    if (!fighterTexture.loadFromFile("fighter.png"))
    {
        return EXIT_FAILURE;
    }
    sf::Texture meteorTexture;
    if (!meteorTexture.loadFromFile("meteor.png"))
    {
        return EXIT_FAILURE;
    }
     
    sf::Font font;
    if (!font.loadFromFile("arial.ttf")) 
    {
        return EXIT_FAILURE;
    }

    sf::Text scoreText;
    scoreText.setFont(font);
    scoreText.setCharacterSize(24); 
    scoreText.setFillColor(sf::Color::White); 
    scoreText.setPosition(10, 10);

    sf::Text gameOverText;
    gameOverText.setFont(font);
    gameOverText.setString("Game Over"); 
    gameOverText.setCharacterSize(72); 
    gameOverText.setFillColor(sf::Color::Red); 
    gameOverText.setPosition(200, 200); 

    // Создание объектов игры
    Player player(playerTexture);
    std::vector<Bullet> bullets;
    std::vector<std::unique_ptr<Enemy>> enemies;
    int score = 0;
    bool gameOver = false;

    // Основной цикл игры
    sf::Clock clock;
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
            {
                // Создание нового снаряда при нажатии левой кнопки мыши
                bullets.push_back(Bullet(player.sprite.getPosition() + sf::Vector2f(player.sprite.getGlobalBounds().width, 10)));
            }
        }

        if (gameOver) {
            // Ожидание нажатия на клавишу, чтобы перезапустить
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
                // Reset
                score = 0;
                gameOver = false;
                enemies.clear(); 
                player.isAlive = true;
                player.sprite.setPosition(100, 200);
            } else {
                // Рисуем экран Game Over
                window.clear();
                window.draw(gameOverText);
                window.display();
                continue;
            }
        }

        float dt = clock.restart().asSeconds();

        // Обновление игровых объектов
        player.update(dt, window.getPosition().y);
        for (auto &bullet : bullets)
        {
            bullet.update(dt);
        }
        for (auto &enemy : enemies)
        {
            enemy->update(dt);
        }

        // Создание новых противников
        if (rand() % 100 < 5)
        { // Вероятность появления нового противника
            int enemyType = rand() % 3;
            if (rand() % 10 < 2)
                enemies.push_back(std::make_unique<Meteor>(meteorTexture));
            else if (enemyType == 0)
            {
                enemies.push_back(std::make_unique<Bird>(birdTexture));
            }
            else if (enemyType == 1)
            {
                enemies.push_back(std::make_unique<Bomber>(bomberTexture));
            }
            else
            {
                enemies.push_back(std::make_unique<Fighter>(fighterTexture));
            }
        }

        // Проверка столкновений (TODO: Реализовать)
        for (auto bullet : bullets)
            for (auto &enemy : enemies)
            {
                enemy->run(bullet, dt);
                checkBump(bullet, *enemy);
            }

        for (auto &enemy1 : enemies)
        {
            checkBump(*enemy1, player);
            for (auto &enemy2 : enemies)
                checkBump(*enemy1, *enemy2);
        }

        if (!player.isAlive) {
            gameOver = true;
        } else {
            // Подсчет очков при уничтожении врагов
            for (auto& enemy : enemies) {
                if (!enemy->isAlive) {
                    score++;
                }
            }
            // Обновление текста счета
            scoreText.setString("Score: " + std::to_string(score)); 
        }

        // Удаление снарядов и противников, вышедших за пределы экрана
        bullets.erase(std::remove_if(bullets.begin(), bullets.end(), [](const Bullet &b)
                                     { return b.shape.getPosition().y > 600; }),
                      bullets.end());
        enemies.erase(std::remove_if(enemies.begin(), enemies.end(), [](const std::unique_ptr<Enemy> &e)
                                     { return e->sprite.getPosition().x < -100 || !e->isAlive; }),
                      enemies.end());


        // Отрисовка
        window.clear();
        window.draw(player.sprite);
        for (auto &bullet : bullets)
        {
            window.draw(bullet.shape);
        }
        for (auto &enemy : enemies)
        {
            window.draw(enemy->sprite);
        }
        window.draw(scoreText); // Отрисовка текста счета
        window.display();
    }

    return 0;
}

void checkBump(Enemy& enemy1, Enemy& enemy2)
{
    if (enemy1.sprite.getGlobalBounds().intersects(enemy2.sprite.getGlobalBounds()))
        enemy1.bump(enemy2);
}

void checkBump(Bullet& bullet, Enemy& enemy)
{
    if (bullet.shape.getGlobalBounds().intersects(enemy.sprite.getGlobalBounds()))
        bullet.bump(enemy);
}

void checkBump(Enemy& enemy, Player& player)
{
    if (enemy.sprite.getGlobalBounds().intersects(player.sprite.getGlobalBounds()))
        enemy.bump(player);
}