#include <SFML/Graphics.hpp>
#include <vector>
#include <random>
#include <fstream>

// Класс для представления самолета игрока
class Player {
public:
    sf::Sprite sprite;
    float velocity;

    Player(sf::Texture& texture) {
        sprite.setTexture(texture);
        sprite.setPosition(100, 200);
        sprite.setScale({100 / sprite.getGlobalBounds().width, 100 / sprite.getGlobalBounds().width});
        velocity = 0;
    }

    void update(float dt, int windowPositionY) {
        std::ofstream log_file;
        log_file.open("log.txt", std::ios::app);
        log_file << sf::Mouse::getPosition().y << ' ' << 
                    sprite.getPosition().y << ' ' << 
                    sf::Mouse::getPosition().y - sprite.getPosition().y << std::endl;
        log_file.close();


        // Обновление позиции самолета с учетом инерции
        velocity += (sf::Mouse::getPosition().y - sprite.getPosition().y) * 0.01f;
        velocity *= 0.9f; 
        sprite.move(0, velocity * dt);

        sprite.setPosition(sprite.getPosition().x, sf::Mouse::getPosition().y - windowPositionY);

        // Ограничение движения самолета по вертикали
        if (sprite.getPosition().y < 0) {
            sprite.setPosition(sprite.getPosition().x, 0);
            velocity = 0;
        }
        if (sprite.getPosition().y > 500) {
            sprite.setPosition(sprite.getPosition().x, 500);
            velocity = 0;
        }
    }
};

// Класс для представления снарядов
class Bullet {
public:
    sf::RectangleShape shape;
    float velocityY;

    Bullet(sf::Vector2f position) {
        shape.setSize(sf::Vector2f(5, 5));
        shape.setFillColor(sf::Color::Red);
        shape.setPosition(position);
        velocityY = -100.f; // Начальная скорость снаряда
    }

    void update(float dt) {
        // Обновление позиции снаряда с учетом гравитации
        velocityY += 98.f * dt; // Ускорение свободного падения
        shape.move(100.f * dt, velocityY * dt); 
    }
};

// Базовый класс для противников
class Enemy {
public:
    sf::Sprite sprite;
    float speed;
    bool isAlive;

    Enemy(sf::Texture& texture) {
        sprite.setTexture(texture);
        isAlive = true;
    }

    virtual void update(float dt) = 0;
};

// Класс для птиц
class Bird : public Enemy {
public:
    float amplitude;
    float frequency;

    Bird(sf::Texture& texture) : Enemy(texture) {
        sprite.setPosition(800, rand() % 100 + 50); // Случайная начальная высота
        sprite.setScale({50 / sprite.getGlobalBounds().width, 50 / sprite.getGlobalBounds().width});
        speed = 100.f;
        amplitude = 20.f;
        frequency = 2.f;
    }

    void update(float dt) override {
        // Движение птицы с колебаниями по высоте
        sprite.move(-speed * dt, amplitude * sin(frequency * sprite.getPosition().x * 0.01f) * dt);
    }
};

// Класс для бомбардировщиков
class Bomber : public Enemy {
public:
    Bomber(sf::Texture& texture) : Enemy(texture) {
        sprite.setPosition(800, rand() % 200 + 150); // Случайная начальная высота
        sprite.setScale({150 / sprite.getGlobalBounds().width, 150 / sprite.getGlobalBounds().width});
        speed = rand() % 50 + 100; // Случайная скорость
    }

    void update(float dt) override {
        sprite.move(-speed * dt, 0);
    }
};

// Класс для истребителей
class Fighter : public Enemy {
public:
    Fighter(sf::Texture& texture) : Enemy(texture) {
        sprite.setPosition(800, rand() % 200 + 150); // Случайная начальная высота
        sprite.setScale({100 / sprite.getGlobalBounds().width, 100 / sprite.getGlobalBounds().width});
        speed = rand() % 100 + 150; // Случайная скорость
    }

    void update(float dt) override {
        // TODO: Реализовать уклонение от снарядов
        sprite.move(-speed * dt, 0); 
    }
};

// Класс для метеоров
class Meteor : public Enemy {
public:
    float velocityX;

    Meteor(sf::Texture& texture) : Enemy(texture) {
        sprite.setPosition(rand() % 800, 0); // Случайная начальная позиция сверху экрана
        sprite.setScale({70 / sprite.getGlobalBounds().width, 70 / sprite.getGlobalBounds().width});
        velocityX = rand() % 150 + 50; // Случайная скорость 
        std::ofstream log_file;
        log_file.open("log.txt", std::ios::app);
        log_file << sprite.getPosition().x << ' ' << 
                    sprite.getPosition().y << std::endl;
        log_file.close();
    }

    void update(float dt) override {
        std::ofstream log_file;
        log_file.open("log.txt", std::ios::app);
        log_file << sprite.getPosition().x << ' ' << 
                    sprite.getPosition().y << ' ' << 
                    velocityX * dt << ' ' << 98.f * dt << std::endl;
        log_file.close();
        sprite.move(velocityX * dt, 98.f * dt * dt);
    }
};

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "2D Scroller");
    window.setFramerateLimit(60);

    // Загрузка текстур (замените на ваши)
    sf::Texture playerTexture;
    if (!playerTexture.loadFromFile("player.png")) {
        return EXIT_FAILURE;
    }
    sf::Texture birdTexture;
    if (!birdTexture.loadFromFile("bird.png")) {
        return EXIT_FAILURE;
    }
    sf::Texture bomberTexture;
    if (!bomberTexture.loadFromFile("bomber.png")) {
        return EXIT_FAILURE;
    }
    sf::Texture fighterTexture;
    if (!fighterTexture.loadFromFile("fighter.png")) {
        return EXIT_FAILURE;
    }
    sf::Texture meteorTexture;
    if (!meteorTexture.loadFromFile("meteor.png")) {
        return EXIT_FAILURE;
    }

    // Создание объектов игры
    Player player(playerTexture);
    std::vector<Bullet> bullets;
    std::vector<std::unique_ptr<Enemy>> enemies;

    // Основной цикл игры
    sf::Clock clock;
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                // Создание нового снаряда при нажатии левой кнопки мыши
                bullets.push_back(Bullet(player.sprite.getPosition() + sf::Vector2f(player.sprite.getGlobalBounds().width, 10)));
            }
        }

        float dt = clock.restart().asSeconds();

        // Обновление игровых объектов
        player.update(dt, window.getPosition().y);
        for (auto& bullet : bullets) {
            bullet.update(dt);
        }
        for (auto& enemy : enemies) {
            enemy->update(dt);
        }

        // Создание новых противников 
        if (rand() % 100 < 5) { // Вероятность появления нового противника
            int enemyType = rand() % 3;
            if (rand() % 10 < 9)
                enemies.push_back(std::make_unique<Meteor>(meteorTexture));
            else if (enemyType == 0) {
                enemies.push_back(std::make_unique<Bird>(birdTexture));
            } else if (enemyType == 1) {
                enemies.push_back(std::make_unique<Bomber>(bomberTexture));
            } else {
                enemies.push_back(std::make_unique<Fighter>(fighterTexture));
            }

        }

        // Проверка столкновений (TODO: Реализовать)
        for(auto bullet: bullets)
            for(auto& enemy : enemies)
                if (std::pow(bullet.shape.getPosition().x - enemy->sprite.getPosition().x, 2) + 
                    std::pow(bullet.shape.getPosition().y - enemy->sprite.getPosition().y, 2) < 10000)
                {
                    enemy->isAlive = false;
                }

        // Удаление снарядов и противников, вышедших за пределы экрана
        bullets.erase(std::remove_if(bullets.begin(), bullets.end(), [](const Bullet& b) {
            return b.shape.getPosition().y > 600;
        }), bullets.end());
        enemies.erase(std::remove_if(enemies.begin(), enemies.end(), [](const std::unique_ptr<Enemy>& e) {
            return e->sprite.getPosition().x < 0 || !e->isAlive;
        }), enemies.end());



        // Отрисовка
        window.clear();
        window.draw(player.sprite);
        for (auto& bullet : bullets) {
            window.draw(bullet.shape);
        }
        for (auto& enemy : enemies) {
            window.draw(enemy->sprite);
        }
        window.display();
    }

    return 0;
}