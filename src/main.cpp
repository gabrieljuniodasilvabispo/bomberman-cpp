#include <SFML/Graphics.hpp>
#include <optional>

int main() {
    // Cria a janela de exibição
    sf::RenderWindow window(sf::VideoMode({800, 600}), "Movimentacao Base para teste");
    window.setFramerateLimit(60);

    // Cria o círculo verde com raio de 30 pixels
    sf::CircleShape shape(30.f);
    shape.setFillColor(sf::Color::Green);
    // Posiciona o círculo no centro da janela
    shape.setPosition({370.f, 270.f});

    // Velocidade de movimento em pixels por segundo
    float speed = 300.f;
    sf::Clock clock;

    while (window.isOpen()) {
        // Processamento de eventos (fechamento da janela)
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>())
                window.close();
        }

        // Cálculo do tempo decorrido por frame (Delta Time)
        sf::Time dt = clock.restart();
        float movement = speed * dt.asSeconds();

        // Movimentação contínua baseada em tempo real (teclado)
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up))
            shape.move({0.f, -movement});
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down))
            shape.move({0.f, movement});
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left))
            shape.move({-movement, 0.f});
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right))
            shape.move({movement, 0.f});

        // Renderização
        window.clear();
        window.draw(shape);
        window.display();
    }

    return 0;
}