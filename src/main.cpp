#include <SFML/Graphics.hpp>
#include <iostream>
#include <new>
#include <optional>
#include <stdexcept>
#include <string>

namespace {

int executar(int argc, char* argv[]) {
    if (argc > 2) {
        throw std::invalid_argument("Numero de argumentos invalido. Use --ajuda.");
    }
    if (argc == 2) {
        const std::string argumento = argv[1];
        if (argumento != "--ajuda" && argumento != "--help") {
            throw std::invalid_argument("Opcao desconhecida. Use --ajuda.");
        }
        std::cout << "Uso: Bomberman [--ajuda|--help]\n"
                  << "Sem argumentos: abre o prototipo grafico de movimento.\n"
                  << "Comandos: WASD ou setas; feche a janela para sair.\n"
                  << "A partida completa ainda nao esta integrada.\n"
                  << "Nao sao necessarios arquivos externos de inicializacao.\n";
        return 0;
    }

    // Cria a janela de exibição
    sf::RenderWindow window(sf::VideoMode({800, 600}), "Movimentacao Base para teste");
    if (!window.isOpen()) {
        throw std::runtime_error("Nao foi possivel abrir a janela grafica.");
    }
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
            if (event->is<sf::Event::Closed>()) {
                window.close();
                break;
            }
        }
        if (!window.isOpen()) {
            break;
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

} // namespace

int main(int argc, char* argv[]) {
    try {
        return executar(argc, argv);
    } catch (const std::invalid_argument& erro) {
        std::cerr << "Erro de entrada: " << erro.what() << '\n';
        return 2;
    } catch (const std::bad_alloc&) {
        std::cerr << "Erro: memoria insuficiente para executar o aplicativo.\n";
        return 1;
    } catch (const std::exception& erro) {
        std::cerr << "Erro ao executar o aplicativo: " << erro.what() << '\n';
        return 1;
    }
}
