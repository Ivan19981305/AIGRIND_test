#include <SFML/Graphics.hpp>
#include <vector>
#include <random>

class Bumping; // объявление класса
class Enemy;
class Meteor;


class Bumpable
{
public:
    virtual void getBumpFrom(const Bumping &) = 0;
};

class Bumping
{
public:
    virtual void bump(Bumpable &object)
    {
        object.getBumpFrom(*this);
    }
};

// Класс для представления самолета игрока
class Player : public Bumpable
{
public:
    sf::Sprite sprite;
    float velocity;
    bool isAlive;

    Player(sf::Texture &texture)
    {
        sprite.setTexture(texture);
        sprite.setPosition(100, 200);
        sprite.setScale({100 / sprite.getGlobalBounds().width, 100 / sprite.getGlobalBounds().width});
        velocity = 0;
        isAlive = true;
    }

    void update(float dt, int windowPositionY)
    {

        // Обновление позиции самолета с учетом инерции
        velocity += (sf::Mouse::getPosition().y - windowPositionY - sprite.getPosition().y) * 0.05f;
        velocity *= 0.9f;
        sprite.move(0, velocity * dt);

        // sprite.setPosition(sprite.getPosition().x, sf::Mouse::getPosition().y - windowPositionY);

        // Ограничение движения самолета по вертикали
        if (sprite.getPosition().y < 0)
        {
            sprite.setPosition(sprite.getPosition().x, 0);
            velocity = 0;
        }
        if (sprite.getPosition().y > 500)
        {
            sprite.setPosition(sprite.getPosition().x, 500);
            velocity = 0;
        }
    }

    void getBumpFrom(const Bumping &) override final
    {
        // от снарядов не помрёт
    }

    void getBumpFrom(const Enemy &)
    {
        isAlive = false;
    }
};

// Класс для представления снарядов
class Bullet : public Bumping
{
public:
    sf::RectangleShape shape;
    float velocityY;

    Bullet(sf::Vector2f position)
    {
        shape.setSize(sf::Vector2f(5, 5));
        shape.setFillColor(sf::Color::Red);
        shape.setPosition(position);
        velocityY = -100.f; // Начальная скорость снаряда
    }

    void update(float dt)
    {
        // Обновление позиции снаряда с учетом гравитации
        velocityY += 98.f * dt; // Ускорение свободного падения
        shape.move(100.f * dt, velocityY * dt);
    }
};

// Базовый класс для противников
class Enemy : public Bumping, public Bumpable
{
public:
    sf::Sprite sprite;
    float speed;
    bool isAlive;

    Enemy(sf::Texture &texture)
    {
        sprite.setTexture(texture);
        isAlive = true;
    }

    virtual void update(float dt) = 0;

    virtual void bump(Bumpable &object) override
    {
        object.getBumpFrom(*this);
    }

    virtual void bump(Enemy &enemy)
    {
        enemy.getBumpFrom(*this);
    }

    virtual void getBumpFrom(const Bumping &) override
    {
        isAlive = false;
    }

    virtual void getBumpFrom(const Enemy &)
    {
        // чтобы не отлетали друг от друга
    }

    virtual void getBumpFrom(const Meteor &)
    {
        isAlive = false;
    }

    virtual void bump(Player &player)
    {
        player.getBumpFrom(*this);
    }


    virtual void run(const Bullet &, float)
    {
        // по умолчанию бездействуем
    }
};

// Класс для птиц
class Bird : public Enemy
{
public:
    float amplitude;
    float frequency;

    Bird(sf::Texture &texture) : Enemy(texture)
    {
        sprite.setPosition(800, rand() % 100 + 50); // Случайная начальная высота
        sprite.setScale({50 / sprite.getGlobalBounds().width, 50 / sprite.getGlobalBounds().width});
        speed = 100.f;
        amplitude = 20.f;
        frequency = 2.f;
    }

    void update(float dt) override
    {
        // Движение птицы с колебаниями по высоте
        sprite.move(-speed * dt, amplitude * sin(frequency * sprite.getPosition().x * 0.01f) * dt);
    }

    void getBumpFrom(const Bumping &) override final
    {
        // Птицы неубиваемы
    }
    void getBumpFrom(const Enemy &) override final
    {
        // Даже от самолётов
    }
    void getBumpFrom(const Meteor &) override final
    {
        // Даже от метеоров
    }
};

// Класс для бомбардировщиков
class Bomber : public Enemy
{
public:
    Bomber(sf::Texture &texture) : Enemy(texture)
    {
        sprite.setPosition(800, rand() % 200 + 150); // Случайная начальная высота
        sprite.setScale({150 / sprite.getGlobalBounds().width, 150 / sprite.getGlobalBounds().width});
        speed = rand() % 50 + 100; // Случайная скорость
    }

    void update(float dt) override
    {
        sprite.move(-speed * dt, 0);
    }
};

// Класс для истребителей
class Fighter : public Enemy
{
public:
    Fighter(sf::Texture &texture) : Enemy(texture)
    {
        sprite.setPosition(800, rand() % 200 + 150); // Случайная начальная высота
        sprite.setScale({100 / sprite.getGlobalBounds().width, 100 / sprite.getGlobalBounds().width});
        speed = rand() % 100 + 150; // Случайная скорость
    }

    void update(float dt) override
    {
        // TODO: Реализовать уклонение от снарядов
        sprite.move(-speed * dt, 0);
    }

    void run(const Bullet &bullet, float dt) override final
    {
        // Вычисляем направление вектора от истребителя к пуле
        sf::Vector2f bulletDirection = bullet.shape.getPosition() - sprite.getPosition();
        float angle = atan2f(bulletDirection.y, bulletDirection.x);

        // Увеличиваем скорость, если пуля близко, чтобы быстрее убегать
        float speedFactor = 1.f + sqrt(pow(bulletDirection.x, 2) + pow(bulletDirection.y, 2)) * 0.01f; // Коэффициент для увеличения скорости

        // Двигаемся в противоположную от пули сторону
        sprite.move(-speed * cos(angle) * speedFactor * dt * 0.2f, -speed * sin(angle) * speedFactor * dt * 0.2f);
    }
};

// Класс для метеоров
class Meteor : public Enemy
{
public:
    float velocityX, velocityY;

    Meteor(sf::Texture &texture) : Enemy(texture)
    {
        sprite.setPosition(rand() % 800, -100); // Случайная начальная позиция сверху экрана
        sprite.setScale({30 / sprite.getGlobalBounds().width, 30 / sprite.getGlobalBounds().width});
        velocityX = rand() % 100 - 50; // Случайная скорость
        velocityY = 0;
    }

    void update(float dt) override
    {
        velocityY += 98.f * dt;
        sprite.move(velocityX * dt, velocityY * dt);
    }

    void getBumpFrom(const Meteor &) override final
    {
        // Против самострела
    }

    void bump(Enemy &enemy) override final
    {
        enemy.getBumpFrom(*this);
    }
};

bool checkDistance(const sf::Vector2f &object1, const sf::Vector2f &object2)
{
    return pow(object1.x - object2.x, 2) + pow(object1.y - object2.y, 2) < 10000;
}

void checkBump(Enemy &enemy1, Enemy &enemy2)
{
    if (checkDistance(enemy1.sprite.getPosition(), enemy2.sprite.getPosition()))
        enemy1.bump(enemy2);
}

void checkBump(Bullet &bullet, Enemy &enemy)
{
    if (checkDistance(bullet.shape.getPosition(), enemy.sprite.getPosition()))
        bullet.bump(enemy);
}

void checkBump(Enemy &enemy, Player &player)
{
    if (checkDistance(enemy.sprite.getPosition(), player.sprite.getPosition()))
        enemy.bump(player);
}

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
    gameOverText.setPosition(100, 200); 

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