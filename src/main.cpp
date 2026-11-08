#include <SFML/Graphics.hpp>
#include "Menu.hpp"
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
                  << "Sem argumentos: abre o menu principal do jogo.\n"
                  << "Comandos no menu: Setas cima/baixo para navegar, Enter para selecionar.\n";
        return 0;
    }

    sf::RenderWindow window(sf::VideoMode({800, 600}), "Bomberman - Menu Principal");
    window.setFramerateLimit(60);

    sf::Font font;
    bool fonteCarregada = font.openFromFile("C:/Windows/Fonts/arial.ttf");
    if (!fonteCarregada) {
        std::cout << "ERRO: A fonte arial nao pôde ser carregada!\n";
    }

    bomberman::Menu menu;
    enum EstadoTela { MenuPrincipal, Jogando, Instrucoes };
    EstadoTela estadoAtual = MenuPrincipal;

    sf::CircleShape shape(30.f);
    shape.setFillColor(sf::Color::Green);
    shape.setPosition({370.f, 270.f});
    float speed = 300.f;
    sf::Clock clock;

    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) window.close();
            
            if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
                if (estadoAtual == MenuPrincipal) {
                    if (keyPressed->code == sf::Keyboard::Key::Up || keyPressed->code == sf::Keyboard::Key::W) {
                        menu.navegarAnterior();
                    } else if (keyPressed->code == sf::Keyboard::Key::Down || keyPressed->code == sf::Keyboard::Key::S) {
                        menu.navegarProxima();
                    } else if (keyPressed->code == sf::Keyboard::Key::Enter) {
                        auto escolha = menu.confirmarSelecao();
                        if (escolha == bomberman::OpcaoMenu::NovoJogo) { 
                            estadoAtual = Jogando; 
                            clock.restart(); 
                        } else if (escolha == bomberman::OpcaoMenu::Instrucoes) {
                            estadoAtual = Instrucoes;
                        } else if (escolha == bomberman::OpcaoMenu::Sair) {
                            window.close();
                        }
                    }
                } else if (keyPressed->code == sf::Keyboard::Key::Escape) {
                    estadoAtual = MenuPrincipal;
                }
            }
        }

        window.clear(sf::Color(20, 20, 40));

        // 1. Renderização do Menu Principal
        if (estadoAtual == MenuPrincipal && fonteCarregada) {
            sf::Text titulo(font, "BOMBERMAN", 50); 
            titulo.setFillColor(sf::Color::Yellow); 
            titulo.setPosition({240.f, 100.f});
            window.draw(titulo);
            
            const auto& opcoes = menu.getOpcoes();
            for (size_t i = 0; i < opcoes.size(); ++i) {
                sf::Text item(font, opcoes[i], 30);
                item.setFillColor(static_cast<int>(i) == menu.getIndiceSelecionado() ? sf::Color::Green : sf::Color::White);
                item.setPosition({300.f, 250.f + (i * 60)});
                window.draw(item);
            }
        } 
        // 2. Renderização da Tela de Instruções
        else if (estadoAtual == Instrucoes && fonteCarregada) {
            sf::Text titulo(font, "INSTRUCOES", 38);
            titulo.setFillColor(sf::Color::Yellow);
            titulo.setPosition({250.f, 20.f});
            window.draw(titulo);

            float y = 95.f;
            for (const auto& secao : bomberman::Menu::getInstrucoes()) {
                sf::Text tituloSecao(font, secao.titulo, 20);
                tituloSecao.setFillColor(sf::Color(255, 200, 80));
                tituloSecao.setPosition({60.f, y});
                window.draw(tituloSecao);
                y += 26.f;
                
                for (const auto& linha : secao.linhas) {
                    sf::Text texto(font, linha, 16);
                    texto.setFillColor(sf::Color::White);
                    texto.setPosition({80.f, y});
                    window.draw(texto);
                    y += 20.f;
                }
                y += 6.f;
            }

            sf::Text voltar(font, "Pressione ESC para voltar ao menu", 16);
            voltar.setFillColor(sf::Color(150, 150, 150));
            voltar.setPosition({230.f, 560.f});
            window.draw(voltar);
        } 
        // 3. Renderização do Jogo
        else if (estadoAtual == Jogando) {
            sf::Time dt = clock.restart();
            float m = speed * dt.asSeconds();
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)) shape.move({0.f, -m});
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) shape.move({0.f, m});
            window.clear(sf::Color::Black);
            window.draw(shape);
        }

        window.display();
    }
    return 0;
}

} // namespace

int main(int argc, char* argv[]) {
    try { return executar(argc, argv); }
    catch (...) { return 1; }
}
