#include <SFML/Graphics.hpp>

int main()
{
    sf::RenderWindow window(sf::VideoMode({800u, 600u}), "SFML 3.0.2 - Green Circle");
    window.setFramerateLimit(60);

    sf::CircleShape circle(30.f);
    circle.setFillColor(sf::Color::Green);
    circle.setPosition({385.f, 285.f});

    const float speed = 4.f;

    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();
        }

        // Movement (WASD or arrow keys)
        sf::Vector2f move(0.f, 0.f);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up))
            move.y -= speed;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down))
            move.y += speed;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left))
            move.x -= speed;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right))
            move.x += speed;

        circle.move(move);

        window.clear(sf::Color::Black);
        window.draw(circle);
        window.display();
    }

    return 0;
}