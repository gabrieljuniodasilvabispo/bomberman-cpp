#include <SFML/Graphics.hpp>

int main() {
    // Cria uma janela 640x480 com título
    sf::RenderWindow window(sf::VideoMode(640, 480), "Bomberman - Teste SFML");
    window.setFramerateLimit(60);

    // Circulo verde para testar renderização de formas
    sf::CircleShape shape(50.f);
    shape.setFillColor(sf::Color::Green);
    shape.setPosition(270.f, 190.f);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear(sf::Color::Black);
        window.draw(shape);
        window.display();
    }

    return 0;
}