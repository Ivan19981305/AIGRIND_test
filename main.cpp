#include <SFML/Graphics.hpp>
#include <vector>
#include <random>

// Класс для представления самолета игрока
class Player {
public:
    sf::Sprite sprite;
    float velocity;

    Player(sf::Texture& texture) {
        sprite.setTexture(texture);
        sprite.setPosition(100, 200);
        velocity = 0;
    }

    void update(float dt) {
        // Обновление позиции самолета с учетом инерции
        velocity += (sf::Mouse::getPosition().y - sprite.getPosition().y) * 0.01f;
        velocity *= 0.9f; 
        sprite.move(0, velocity * dt);

        // Ограничение движения самолета по вертикали
        if (sprite.getPosition().y < 0) {
            sprite.setPosition(sprite.getPosition().x, 0);
            velocity = 0;
        }
        if (sprite.getPosition().y > 400) {
            sprite.setPosition(sprite.getPosition().x, 400);
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
        shape.setSize(sf::Vector2f(5, 10));
        shape.setFillColor(sf::Color::Red);
        shape.setPosition(position);
        velocityY = -500.f; // Начальная скорость снаряда
    }

    void update(float dt) {
        // Обновление позиции снаряда с учетом гравитации
        velocityY += 9.8f * dt; // Ускорение свободного падения
        shape.move(200.f * dt, velocityY * dt); 
    }
};

// Базовый класс для противников
class Enemy {
public:
    sf::Sprite sprite;
    float speed;

    Enemy(sf::Texture& texture) {
        sprite.setTexture(texture);
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
        speed = rand() % 100 + 150; // Случайная скорость
    }

    void update(float dt) override {
        // TODO: Реализовать уклонение от снарядов
        sprite.move(-speed * dt, 0); 
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
                bullets.push_back(Bullet(player.sprite.getPosition() + sf::Vector2f(player.sprite.getLocalBounds().width, 10)));
            }
        }

        float dt = clock.restart().asSeconds();

        // Обновление игровых объектов
        player.update(dt);
        for (auto& bullet : bullets) {
            bullet.update(dt);
        }
        for (auto& enemy : enemies) {
            enemy->update(dt);
        }

        // Создание новых противников 
        if (rand() % 100 < 5) { // Вероятность появления нового противника
            int enemyType = rand() % 3;
            if (enemyType == 0) {
                enemies.push_back(std::make_unique<Bird>(birdTexture));
            } else if (enemyType == 1) {
                enemies.push_back(std::make_unique<Bomber>(bomberTexture));
            } else {
                enemies.push_back(std::make_unique<Fighter>(fighterTexture));
            }
        }

        // Удаление снарядов и противников, вышедших за пределы экрана
        bullets.erase(std::remove_if(bullets.begin(), bullets.end(), [](const Bullet& b) {
            return b.shape.getPosition().x > 800;
        }), bullets.end());
        enemies.erase(std::remove_if(enemies.begin(), enemies.end(), [](const std::unique_ptr<Enemy>& e) {
            return e->sprite.getPosition().x < -e->sprite.getLocalBounds().width;
        }), enemies.end());

        // Проверка столкновений (TODO: Реализовать)


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