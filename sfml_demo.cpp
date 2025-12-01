/*
Compilar (MacOS / Linux):
g++ sfml_demo.cpp -o sfml_demo -std=c++17 \
-I/opt/homebrew/opt/sfml@2/include \
-L/opt/homebrew/opt/sfml@2/lib \
-lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio

Ejecutar:
./sfml_demo

Recursos utilizados en esta demo (puede reemplazarlos con los suyos):

- player.png (imagen pequeña del jugador)
- enemy.png (imagen pequeña del objeto que cae)
- bg.png (fondo)
- beep.wav (efecto de sonido al colisionar)
- PressStart2P-Regular.ttf (Tipo de letra, lo descargue de Google Fonts)

Si no estan las imagenes en el proyecto, el juego se ejecutara, pero va indicar que faltan los recursos en el terminal
*/

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <vector>
#include <random>
#include <string>
#include <sstream>

static std::mt19937 rng((unsigned)std::random_device{}());
int randRange(int a, int b)
{
    std::uniform_int_distribution<int> dist(a, b);
    return dist(rng);
}

int main()
{
    const unsigned WINDOW_W = 800;
    const unsigned WINDOW_H = 600;
    sf::RenderWindow window(sf::VideoMode(WINDOW_W, WINDOW_H), "SFML Demo - Mini Game");
    window.setFramerateLimit(60);

    // Load optional textures
    sf::Texture texPlayer, texEnemy, texBG;
    bool havePlayerTex = false, haveEnemyTex = false, haveBGTex = false;
    if (texPlayer.loadFromFile("player.png"))
        havePlayerTex = true;
    if (texEnemy.loadFromFile("enemy.png"))
        haveEnemyTex = true;
    if (texBG.loadFromFile("bg.png"))
        haveBGTex = true;

    // Load sound
    sf::SoundBuffer hitBuf;
    bool haveSound = false;
    if (hitBuf.loadFromFile("beep.wav"))
        haveSound = true;
    sf::Sound hitSound;
    if (haveSound)
        hitSound.setBuffer(hitBuf);

    // Player setup
    sf::Sprite player;
    sf::CircleShape playerShape(24.f);
    playerShape.setOrigin(24.f, 24.f);
    playerShape.setFillColor(sf::Color(100, 170, 250));
    if (havePlayerTex)
    {
        player.setTexture(texPlayer);
        player.setScale(48.f / texPlayer.getSize().x, 48.f / texPlayer.getSize().y);
        player.setOrigin(24.f, 24.f);
    }
    sf::Vector2f playerPos(WINDOW_W / 2.f, WINDOW_H - 60.f);

    // Background
    sf::Sprite bgSprite;
    if (haveBGTex)
    {
        bgSprite.setTexture(texBG);
        float sx = (float)WINDOW_W / texBG.getSize().x;
        float sy = (float)WINDOW_H / texBG.getSize().y;
        bgSprite.setScale(sx, sy);
    }

    // Enemy (falling) objects
    struct Enemy
    {
        sf::Sprite sprite;
        sf::CircleShape shape;
        sf::Vector2f vel;
    };
    std::vector<Enemy> enemies;
    float spawnTimer = 0.f;
    float spawnInterval = 0.9f; // seconds

    // Score & font
    int score = 0;
    sf::Font font;
    bool haveFont = false;
    if (font.loadFromFile("PressStart2P-Regular.ttf"))
        haveFont = true;
    sf::Text scoreText;
    if (haveFont)
    {
        scoreText.setFont(font);
        scoreText.setCharacterSize(24);
        scoreText.setFillColor(sf::Color::White);
    }
    scoreText.setPosition(10, 10);

    // Game state
    bool running = true;
    sf::Clock clock;

    while (window.isOpen())
    {
        sf::Event ev;
        while (window.pollEvent(ev))
        {
            if (ev.type == sf::Event::Closed)
                window.close();
            if (ev.type == sf::Event::KeyPressed && ev.key.code == sf::Keyboard::Escape)
                window.close();
        }

        float dt = clock.restart().asSeconds();

        // Input
        sf::Vector2f dir(0.f, 0.f);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
            dir.x -= 1.f;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
            dir.x += 1.f;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) || sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
            dir.y -= 1.f;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) || sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
            dir.y += 1.f;
        float speed = 300.f;
        playerPos += dir * speed * dt;
        // clamp
        if (playerPos.x < 24.f)
            playerPos.x = 24.f;
        if (playerPos.x > WINDOW_W - 24.f)
            playerPos.x = WINDOW_W - 24.f;
        if (playerPos.y < 24.f)
            playerPos.y = 24.f;
        if (playerPos.y > WINDOW_H - 24.f)
            playerPos.y = WINDOW_H - 24.f;

        // Spawn enemies
        spawnTimer += dt;
        if (spawnTimer >= spawnInterval)
        {
            spawnTimer = 0.f;
            Enemy e;
            if (haveEnemyTex)
            {
                e.sprite.setTexture(texEnemy);
                float s = 40.f / texEnemy.getSize().x;
                e.sprite.setScale(s, s);
                e.sprite.setOrigin(texEnemy.getSize().x / 2.f, texEnemy.getSize().y / 2.f);
            }
            e.shape.setRadius(18.f);
            e.shape.setOrigin(18.f, 18.f);
            e.shape.setFillColor(sf::Color(250, 100, 100));
            float x = (float)randRange(30, WINDOW_W - 30);
            if (haveEnemyTex)
                e.sprite.setPosition(x, -40.f);
            else
                e.shape.setPosition(x, -40.f);
            float vy = (float)randRange(120, 260);
            float vx = (float)(randRange(-80, 80));
            e.vel = {vx, vy};
            enemies.push_back(std::move(e));
        }

        // Update enemies
        for (auto it = enemies.begin(); it != enemies.end();)
        {
            if (haveEnemyTex)
                it->sprite.move(it->vel * dt);
            else
                it->shape.move(it->vel * dt);
            // remove if off-screen
            sf::Vector2f pos = haveEnemyTex ? it->sprite.getPosition() : it->shape.getPosition();
            if (pos.y > WINDOW_H + 50.f)
            {
                it = enemies.erase(it);
                score += 1;
                continue;
            } // score for dodging
            ++it;
        }

        // Collision detection (simple circle-vs-circle)
        sf::Vector2f playerCenter = playerPos;
        float playerRadius = 24.f;
        bool collided = false;
        for (auto it = enemies.begin(); it != enemies.end();)
        {
            sf::Vector2f ePos = haveEnemyTex ? it->sprite.getPosition() : it->shape.getPosition();
            float exRadius = haveEnemyTex ? 20.f : it->shape.getRadius();
            float dx = ePos.x - playerCenter.x;
            float dy = ePos.y - playerCenter.y;
            float dist2 = dx * dx + dy * dy;
            if (dist2 <= (playerRadius + exRadius) * (playerRadius + exRadius))
            {
                // collision
                collided = true;
                if (haveSound)
                    hitSound.play();
                // simple effect: reset score and clear enemies
                score = 0;
                enemies.clear();
                break;
            }
            ++it;
        }

        // Draw
        window.clear(sf::Color(30, 30, 40));
        if (haveBGTex)
            window.draw(bgSprite);

        // draw player (texture or shape)
        if (havePlayerTex)
        {
            player.setPosition(playerPos);
            window.draw(player);
        }
        else
        {
            playerShape.setPosition(playerPos);
            window.draw(playerShape);
        }

        // draw enemies
        for (auto &e : enemies)
        {
            if (haveEnemyTex)
                window.draw(e.sprite);
            else
                window.draw(e.shape);
        }

        // HUD
        std::ostringstream ss;
        ss << "Score: " << score;
        scoreText.setString(ss.str());
        if (!haveFont)
        { // fallback display using simple rectangle + no font
            sf::RectangleShape rect(sf::Vector2f(140, 30));
            rect.setPosition(10, 10);
            rect.setFillColor(sf::Color(0, 0, 0, 140));
            window.draw(rect);
            // draw a simple text using shapes (very crude)
        }
        else
        {
            window.draw(scoreText);
        }

        // small instruction
        if (haveFont)
        {
            sf::Text t("Move: WASD / Arrow keys. Esc to quit.", font, 14);
            t.setPosition(10, WINDOW_H - 24);
            window.draw(t);
        }

        window.display();
    }

    return 0;
}
