#include <SFML/Graphics.hpp>
#include "AudioJogo.hpp"
#include "Bomba.hpp"
#include "Explosao.hpp"
#include "Jogador.hpp"
#include "Mapa.hpp"
#include "Menu.hpp"
#include "Partida.hpp"
#include <algorithm>
#include <array>
#include <iostream>
#include <optional>
#include <stdexcept>
#include <string>
#include <vector>

#ifndef BOMBERMAN_ASSETS_DIR
#define BOMBERMAN_ASSETS_DIR "assets"
#endif

namespace {
using bomberman::EstadoPartida;
using bomberman::Partida;
using Tecla = sf::Keyboard::Key;
enum class Tela { Menu, Instrucoes, Jogo };
const sf::Color FUNDO{20, 20, 40};
const sf::Color DOURADO{255, 200, 80};
const sf::Color VERDE{100, 235, 155};
const sf::Color SUAVE{180, 190, 210};

struct ErroEntrada : std::invalid_argument {
    using std::invalid_argument::invalid_argument;
};

void texto(sf::RenderWindow& janela, const sf::Font& fonte, const std::string& conteudo,
           unsigned tamanho, sf::Vector2f posicao, sf::Color cor = sf::Color::White) {
    sf::Text desenho(fonte, conteudo, tamanho);
    desenho.setPosition(posicao);
    desenho.setFillColor(cor);
    janela.draw(desenho);
}

void retangulo(sf::RenderWindow& janela, sf::Vector2f posicao, sf::Vector2f tamanho, sf::Color cor) {
    sf::RectangleShape desenho(tamanho);
    desenho.setPosition(posicao);
    desenho.setFillColor(cor);
    janela.draw(desenho);
}

void circulo(sf::RenderWindow& janela, sf::Vector2f centro, float raio, sf::Color cor) {
    sf::CircleShape desenho(raio);
    desenho.setPosition(centro - sf::Vector2f{raio, raio});
    desenho.setFillColor(cor);
    janela.draw(desenho);
}

// Texturas dos sprites (pixel art 24x24, ampliadas para o tamanho da celula).
// Se algum arquivo faltar, o elemento correspondente volta ao desenho com formas.
struct Sprites {
    sf::Texture chao;
    sf::Texture chaoSombra;
    sf::Texture bloco;
    std::array<sf::Texture, 3> bomba;
    // Melhorias (o escudo ainda nao tem sprite e continua com o desenho antigo).
    sf::Texture itemFogo;  // AumentoAlcance
    sf::Texture itemBomba; // BombaExtra
    // Partes da explosao: cada sprite ocupa uma posicao especifica no "+" do fogo.
    sf::Texture explosaoCentro;
    sf::Texture explosaoHorizontal;
    sf::Texture explosaoEsquerda; // ponta esquerda do braco horizontal
    sf::Texture explosaoDireita;  // ponta direita do braco horizontal
    sf::Texture explosaoVertical;
    sf::Texture explosaoCima;     // ponta de cima do braco vertical
    sf::Texture explosaoBaixo;    // ponta de baixo do braco vertical
    bool temChao = false;
    bool temChaoSombra = false;
    bool temBloco = false;
    bool temBomba = false;
    bool temItemFogo = false;
    bool temItemBomba = false;
    bool temExplosao = false; // so vale se as 7 partes foram carregadas
};

bool carregarTextura(sf::Texture& textura, const std::string& caminho) {
    if (!textura.loadFromFile(caminho)) {
        std::cerr << "Aviso: sprite nao encontrado em: " << caminho << std::endl;
        return false;
    }
    textura.setSmooth(false); // pixel art: sem suavizacao ao ampliar
    return true;
}

Sprites carregarSprites(const std::string& pasta) {
    Sprites s;
    s.temChao = carregarTextura(s.chao, pasta + "/green-sheet.png");
    s.temChaoSombra = carregarTextura(s.chaoSombra, pasta + "/green-tile-with-shadow.png");
    s.temBloco = carregarTextura(s.bloco, pasta + "/grey-tile.png");
    s.temBomba = true;
    for (std::size_t i = 0; i < s.bomba.size(); ++i) {
        if (!carregarTextura(s.bomba[i], pasta + "/set-bomb-" + std::to_string(i) + ".png")) {
            s.temBomba = false;
        }
    }

    s.temItemFogo = carregarTextura(s.itemFogo, pasta + "/item-fire.png");
    // O arquivo pode se chamar item-bomb.png ou item-bomba.png; aceita os dois.
    s.temItemBomba = carregarTextura(s.itemBomba, pasta + "/item-bomb.png")
        || carregarTextura(s.itemBomba, pasta + "/item-bomba.png");

    // Carrega todas as partes (sem curto-circuito) para avisar de cada arquivo faltando.
    const bool partes[] = {
        carregarTextura(s.explosaoCentro, pasta + "/bomb-explode-ctr.png"),
        carregarTextura(s.explosaoHorizontal, pasta + "/bomb-explode-hrz.png"),
        carregarTextura(s.explosaoEsquerda, pasta + "/bomb-explode-hrz-l.png"),
        carregarTextura(s.explosaoDireita, pasta + "/bomb-explode-hrz-r.png"),
        carregarTextura(s.explosaoVertical, pasta + "/bomb-explode-vrt.png"),
        carregarTextura(s.explosaoCima, pasta + "/bomb-explode-vrt-u.png"),
        carregarTextura(s.explosaoBaixo, pasta + "/bomb-explode-vrt-d.png"),
    };
    s.temExplosao = std::all_of(std::begin(partes), std::end(partes), [](bool ok) { return ok; });
    return s;
}

// Desenha a textura inteira ocupando uma celula quadrada de tamanho "lado".
void sprite(sf::RenderWindow& janela, const sf::Texture& textura, sf::Vector2f posicao, float lado) {
    sf::Sprite desenho(textura);
    const auto tamanho = sf::Vector2f{textura.getSize()};
    desenho.setPosition(posicao);
    desenho.setScale({lado / tamanho.x, lado / tamanho.y});
    janela.draw(desenho);
}

std::optional<Posicao> direcao(Tecla tecla) {
    switch (tecla) {
    case Tecla::W: case Tecla::Up: return Posicao{-1, 0};
    case Tecla::S: case Tecla::Down: return Posicao{1, 0};
    case Tecla::A: case Tecla::Left: return Posicao{0, -1};
    case Tecla::D: case Tecla::Right: return Posicao{0, 1};
    default: return std::nullopt;
    }
}

void desenharMenu(sf::RenderWindow& janela, const sf::Font& fonte, const bomberman::Menu& menu, const sf::Sprite& fundo) {
    janela.draw(fundo);
    const auto& opcoes = menu.getOpcoes();
    for (std::size_t i = 0; i < opcoes.size(); ++i) {
        const bool selecionado = static_cast<int>(i) == menu.getIndiceSelecionado();
        texto(janela, fonte, (selecionado ? "> " : "  ") + opcoes[i], 30,
              {285.f, 250.f + static_cast<float>(i) * 60.f}, selecionado ? VERDE : sf::Color::White);
    }
}

void desenharInstrucoes(sf::RenderWindow& janela, const sf::Font& fonte) {
    texto(janela, fonte, "INSTRUCOES", 34, {270.f, 16.f}, DOURADO);
    float y = 70.f;
    for (const auto& secao : bomberman::Menu::getInstrucoes()) {
        texto(janela, fonte, secao.titulo, 18, {55.f, y}, DOURADO);
        y += 23.f;
        for (const auto& linha : secao.linhas) {
            texto(janela, fonte, linha, 16, {70.f, y});
            y += 19.f;
        }
        y += 7.f;
    }
    texto(janela, fonte, "Esc: voltar ao menu", 17, {320.f, 568.f}, SUAVE);
}

void desenharPartida(sf::RenderWindow& janela, const sf::Font& fonte, const Partida& partida,
                     const Sprites& sprites, sf::Time tempoAnimacao) {
    const auto& mapa = partida.obterMapa();
    const auto& jogador = partida.obterJogador();
    const auto inimigosVivos = std::count_if(partida.obterInimigos().begin(), partida.obterInimigos().end(),
                                            [](const auto& inimigo) { return inimigo.ativo(); });
    texto(janela, fonte, "BOMBERMAN", 26, {30.f, 12.f}, DOURADO);
    texto(janela, fonte, "Fase: " + std::to_string(partida.obterNumeroFase()) + "/"
          + std::to_string(partida.obterQuantidadeFases()), 19, {350.f, 18.f}, DOURADO);
    texto(janela, fonte, "Inimigos: " + std::to_string(inimigosVivos), 19, {625.f, 18.f});
    texto(janela, fonte, "Bombas: " + std::to_string(jogador.obterBombasAtivas()) + "/"
          + std::to_string(jogador.obterLimiteBombas()) + "     Alcance: "
          + std::to_string(jogador.obterAlcance()) + "     Escudo: " + (jogador.temEscudo() ? "sim" : "nao"),
          18, {160.f, 52.f});
    texto(janela, fonte, inimigosVivos == 0 ? "Saida liberada! Entre para concluir a fase."
          : "Elimine os inimigos para liberar a saida.", 16, {245.f, 79.f}, inimigosVivos == 0 ? VERDE : SUAVE);

    const float lado = std::min({48.f, 704.f / mapa.obterColunas(), 432.f / mapa.obterLinhas()});
    const sf::Vector2f origem{(800.f - lado * mapa.obterColunas()) / 2.f, 108.f};
    const auto canto = [&](Posicao p) {
        return origem + sf::Vector2f{p.coluna * lado, p.linha * lado};
    };
    const auto centro = [&](Posicao p) { return canto(p) + sf::Vector2f{lado / 2.f, lado / 2.f}; };
    for (int linha = 0; linha < mapa.obterLinhas(); ++linha) {
        for (int coluna = 0; coluna < mapa.obterColunas(); ++coluna) {
            const Posicao p{linha, coluna};
            const auto pos = canto(p);
            // Chao: a variante sombreada fica logo abaixo de um bloco (a sombra
            // do sprite esta no topo da celula).
            const bool sombra = mapa.obterBloco(Posicao{linha - 1, coluna}) != nullptr;
            if (sombra && sprites.temChaoSombra) {
                sprite(janela, sprites.chaoSombra, pos, lado);
            } else if (sprites.temChao) {
                sprite(janela, sprites.chao, pos, lado);
            } else {
                retangulo(janela, pos, {lado - 1.f, lado - 1.f},
                          (linha + coluna) % 2 == 0 ? sf::Color(35, 46, 54) : sf::Color(40, 52, 59));
            }
            if (const auto* bloco = mapa.obterBloco(p)) {
                const bool parede = bloco->obterTipo() == TipoBloco::Indestrutivel;
                if (!parede && sprites.temBloco) {
                    sprite(janela, sprites.bloco, pos, lado);
                } else {
                    retangulo(janela, pos + sf::Vector2f{2.f, 2.f}, {lado - 5.f, lado - 5.f},
                              parede ? sf::Color(93, 111, 132) : sf::Color(167, 108, 63));
                    retangulo(janela, pos + sf::Vector2f{5.f, 5.f}, {lado - 11.f, 3.f},
                              parede ? sf::Color(132, 152, 173) : sf::Color(220, 158, 99));
                    if (!parede) {
                        retangulo(janela, pos + sf::Vector2f{3.f, lado / 2.f}, {lado - 7.f, 2.f}, FUNDO);
                        retangulo(janela, pos + sf::Vector2f{lado / 2.f, 3.f}, {2.f, lado / 2.f - 3.f}, FUNDO);
                    }
                }
            }
            if (const auto* melhoria = mapa.obterMelhoria(p)) {
                const auto tipoMelhoria = melhoria->getTipo();
                if (tipoMelhoria == bomberman::TipoMelhoria::AumentoAlcance && sprites.temItemFogo) {
                    sprite(janela, sprites.itemFogo, pos, lado);
                } else if (tipoMelhoria == bomberman::TipoMelhoria::BombaExtra && sprites.temItemBomba) {
                    sprite(janela, sprites.itemBomba, pos, lado);
                } else {
                    // Escudo (sem sprite ainda) ou sprite ausente: desenho antigo.
                    std::string letra = "E";
                    sf::Color cor{80, 195, 255};
                    if (tipoMelhoria == bomberman::TipoMelhoria::AumentoAlcance) {
                        letra = "A"; cor = DOURADO;
                    } else if (tipoMelhoria == bomberman::TipoMelhoria::BombaExtra) {
                        letra = "B"; cor = VERDE;
                    }
                    retangulo(janela, pos + sf::Vector2f{10.f, 10.f}, {lado - 20.f, lado - 20.f}, cor);
                    texto(janela, fonte, letra, 21, pos + sf::Vector2f{17.f, 9.f}, FUNDO);
                }
            }
        }
    }
    const auto saida = canto(mapa.obterSaida());
    retangulo(janela, saida + sf::Vector2f{9.f, 6.f}, {lado - 18.f, lado - 12.f},
              inimigosVivos == 0 ? VERDE : sf::Color(110, 115, 128));
    retangulo(janela, saida + sf::Vector2f{14.f, 11.f}, {lado - 28.f, lado - 17.f}, FUNDO);
    circulo(janela, saida + sf::Vector2f{lado - 18.f, lado / 2.f}, 2.f, DOURADO);

    for (std::size_t i = 0; i < partida.obterQuantidadeBombas(); ++i) {
        const auto p = partida.obterBomba(i).getPosicao();
        if (sprites.temBomba) {
            // Pulsa em ciclo 0 -> 1 -> 2 -> 1, trocando de estagio a cada 150 ms.
            static constexpr std::array<std::size_t, 4> CICLO{0, 1, 2, 1};
            const auto passo = static_cast<std::size_t>(tempoAnimacao.asMilliseconds() / 150);
            sprite(janela, sprites.bomba[CICLO[passo % CICLO.size()]], canto({p.y, p.x}), lado);
            continue;
        }
        const auto c = centro({p.y, p.x});
        circulo(janela, c, lado * 0.32f, sf::Color(185, 190, 200));
        circulo(janela, c, lado * 0.27f, sf::Color(15, 16, 24));
        retangulo(janela, c + sf::Vector2f{1.f, -lado * 0.35f}, {4.f, 9.f}, DOURADO);
    }
    for (std::size_t i = 0; i < partida.obterQuantidadeExplosoes(); ++i) {
        const auto& celulas = partida.obterExplosao(i).getCelulasAtingidas();
        if (celulas.empty()) continue;

        // A primeira celula e sempre a origem (centro). As demais ficam na mesma
        // linha ou na mesma coluna dela, entao o deslocamento (dx, dy) em relacao
        // a origem diz de que braco a celula faz parte.
        const sf::Vector2i origem = celulas.front();

        // Tamanho real de cada braco: pode ser menor que o alcance da bomba se um
        // bloco interrompeu o fogo. A ultima celula do braco e a ponta.
        int esquerda = 0, direita = 0, cima = 0, baixo = 0;
        for (const auto& c : celulas) {
            const int dx = c.x - origem.x;
            const int dy = c.y - origem.y;
            if (dy == 0 && dx < 0) esquerda = std::max(esquerda, -dx);
            else if (dy == 0 && dx > 0) direita = std::max(direita, dx);
            else if (dx == 0 && dy < 0) cima = std::max(cima, -dy);
            else if (dx == 0 && dy > 0) baixo = std::max(baixo, dy);
        }

        for (const auto& c : celulas) {
            const auto pos = canto({c.y, c.x});
            if (!sprites.temExplosao) {
                // Sem os sprites: desenho antigo.
                retangulo(janela, pos + sf::Vector2f{3.f, 3.f}, {lado - 6.f, lado - 6.f}, sf::Color(255, 120, 45, 220));
                retangulo(janela, pos + sf::Vector2f{12.f, 12.f}, {lado - 24.f, lado - 24.f}, DOURADO);
                continue;
            }
            const int dx = c.x - origem.x;
            const int dy = c.y - origem.y;
            const sf::Texture* textura = &sprites.explosaoCentro;
            if (dy == 0 && dx < 0) {
                textura = (-dx == esquerda) ? &sprites.explosaoEsquerda : &sprites.explosaoHorizontal;
            } else if (dy == 0 && dx > 0) {
                textura = (dx == direita) ? &sprites.explosaoDireita : &sprites.explosaoHorizontal;
            } else if (dx == 0 && dy < 0) {
                textura = (-dy == cima) ? &sprites.explosaoCima : &sprites.explosaoVertical;
            } else if (dx == 0 && dy > 0) {
                textura = (dy == baixo) ? &sprites.explosaoBaixo : &sprites.explosaoVertical;
            }
            sprite(janela, *textura, pos, lado);
        }
    }
    const auto personagem = [&](Posicao p, sf::Color cor, bool escudo) {
        const auto c = centro(p);
        if (escudo) circulo(janela, c, lado * 0.43f, sf::Color(80, 195, 255));
        circulo(janela, c, lado * 0.32f, cor);
        circulo(janela, c + sf::Vector2f{-6.f, -3.f}, 3.f, FUNDO);
        circulo(janela, c + sf::Vector2f{6.f, -3.f}, 3.f, FUNDO);
    };
    for (const auto& inimigo : partida.obterInimigos()) {
        if (inimigo.ativo()) personagem(inimigo.obterPosicao(), sf::Color(240, 95, 110), false);
    }
    if (jogador.ativo()) personagem(jogador.obterPosicao(), VERDE, jogador.temEscudo());
    texto(janela, fonte, "WASD / setas: mover    Espaco: bomba    R: reiniciar    Esc: menu", 16, {145.f, 548.f});
    texto(janela, fonte, "Itens: A = alcance   B = bomba extra   E = escudo", 15, {230.f, 574.f}, SUAVE);
}

void desenharAviso(sf::RenderWindow& janela, const sf::Font& fonte,
                   const std::string& titulo, const std::string& detalhe, sf::Color cor) {
    retangulo(janela, {0.f, 0.f}, {800.f, 600.f}, sf::Color(10, 12, 22, 195));
    retangulo(janela, {145.f, 205.f}, {510.f, 190.f}, FUNDO);
    texto(janela, fonte, titulo, 38, {270.f, 225.f}, cor);
    texto(janela, fonte, detalhe, 19, {185.f, 292.f});
    texto(janela, fonte, "Esc: voltar ao menu", 17, {315.f, 345.f}, SUAVE);
}

int executar(int argc, char* argv[]) {
    if (argc > 2) throw ErroEntrada("Numero de argumentos invalido. Use --ajuda.");
    if (argc == 2) {
        const std::string argumento = argv[1];
        if (argumento != "--ajuda" && argumento != "--help") {
            throw ErroEntrada("Opcao desconhecida. Use --ajuda.");
        }
        std::cout << "Uso: Bomberman [--ajuda|--help]\n"
                  << "Sem argumentos: abre o menu principal do jogo.\n"
                  << "Menu: Setas/W/S e Enter. Jogo: WASD/setas, Espaco para bomba.\n"
                  << "Sao 4 mapas fixos. A porta liberada leva ao proximo.\n"
                  << "Conclua o quarto mapa para vencer o jogo.\n"
                  << "R repete a fase; Enter reinicia apos derrota; Esc volta ao menu.\n"
                  << "Apos vencer, Enter ou R inicia um novo jogo.\n"
                  << "A partida pausa quando a janela perde o foco.\n";
        return 0;
    }

    sf::Font fonte;
    const std::string caminhoFonte = std::string(BOMBERMAN_ASSETS_DIR) + "/fonts/tuffy.ttf";
    if (!fonte.openFromFile(caminhoFonte)) {
        throw std::runtime_error("Nao foi possivel carregar a fonte: " + caminhoFonte);
    }

    // Carregamento da imagem de fundo
    sf::Texture texturaFundo;
    std::string caminhoImagem = std::string(BOMBERMAN_ASSETS_DIR) + "/img/fundo.png";
    if (!texturaFundo.loadFromFile(caminhoImagem)) {
        std::cerr << "Aviso: Imagem de fundo nao encontrada em: " << caminhoImagem << std::endl;
    }
    sf::Sprite fundo(texturaFundo);

    // Sprites do jogo (chao, blocos e bombas)
    const Sprites sprites = carregarSprites(std::string(BOMBERMAN_ASSETS_DIR) + "/img");
    const sf::Clock relogioAnimacao;

    sf::RenderWindow janela(sf::VideoMode({800, 600}), "Bomberman", sf::Style::Titlebar | sf::Style::Close);
    janela.setFramerateLimit(60);
    janela.setKeyRepeatEnabled(false);
    bomberman::AudioJogo audio(std::string(BOMBERMAN_ASSETS_DIR) + "/sounds");
    bomberman::Menu menu;
    Partida partida;
    Tela tela = Tela::Menu;
    sf::Clock relogio;
    sf::Time esperaMovimento = sf::Time::Zero;
    std::vector<Tecla> teclasMovimento;
    std::vector<Tecla> comandos;
    const auto limparEntrada = [&] {
        teclasMovimento.clear();
        comandos.clear();
        esperaMovimento = sf::Time::Zero;
    };
    const auto iniciar = [&](bool novoJogo) {
        if (novoJogo) partida.iniciarFase();
        else partida.reiniciarFase();
        audio.parar();
        tela = Tela::Jogo;
        limparEntrada();
    };

    while (janela.isOpen()) {
        sf::Time dt = relogio.restart();
        comandos.clear();
        bool mudouContexto = false;
        bool moveuPorEvento = false;
        while (const auto evento = janela.pollEvent()) {
            if (evento->is<sf::Event::Closed>()) {
                janela.close();
                break;
            }
            if (evento->is<sf::Event::FocusLost>() || evento->is<sf::Event::FocusGained>()) {
                limparEntrada();
                mudouContexto = true;
                if (evento->is<sf::Event::FocusLost>() && tela == Tela::Jogo) {
                    audio.pausar();
                } else if (evento->is<sf::Event::FocusGained>()) {
                    audio.retomar();
                }
            }
            if (const auto* solta = evento->getIf<sf::Event::KeyReleased>()) {
                teclasMovimento.erase(std::remove(teclasMovimento.begin(), teclasMovimento.end(), solta->code),
                                      teclasMovimento.end());
            }
            const auto* tecla = evento->getIf<sf::Event::KeyPressed>();
            if (!tecla || !janela.hasFocus()) continue;
            const auto codigo = tecla->code;
            if (tela == Tela::Menu) {
                if (codigo == Tecla::Up || codigo == Tecla::W) menu.navegarAnterior();
                else if (codigo == Tecla::Down || codigo == Tecla::S) menu.navegarProxima();
                else if (codigo == Tecla::Enter) {
                    switch (menu.confirmarSelecao()) {
                    case bomberman::OpcaoMenu::NovoJogo: iniciar(true); break;
                    case bomberman::OpcaoMenu::Instrucoes: tela = Tela::Instrucoes; break;
                    case bomberman::OpcaoMenu::Sair: janela.close(); break;
                    default: break;
                    }
                    mudouContexto = true;
                }
            } else if (codigo == Tecla::Escape) {
                partida.encerrarFase();
                audio.parar();
                tela = Tela::Menu;
                limparEntrada();
                mudouContexto = true;
            } else if (tela == Tela::Jogo) {
                const bool acabou = partida.obterEstado() != EstadoPartida::EmAndamento;
                if (codigo == Tecla::R || (acabou && codigo == Tecla::Enter)) {
                    iniciar(partida.obterEstado() == EstadoPartida::Vitoria);
                    mudouContexto = true;
                } else if (!acabou && codigo == Tecla::Space) {
                    comandos.push_back(codigo);
                } else if (!acabou && direcao(codigo)) {
                    if (std::find(teclasMovimento.begin(), teclasMovimento.end(), codigo) == teclasMovimento.end()) {
                        teclasMovimento.push_back(codigo);
                        comandos.push_back(codigo);
                    }
                }
            }
        }
        if (!janela.isOpen()) break;
        // Nao aplicar o tempo anterior ao inicio/reinicio ou ao retorno de uma pausa.
        if (mudouContexto) dt = sf::Time::Zero;
        if (tela == Tela::Jogo && janela.hasFocus()) {
            // O tempo decorrido pertence ao estado anterior aos novos comandos.
            // Uma bomba criada agora nao pode receber o tempo do quadro passado.
            partida.atualizar(dt);
            for (auto comando : comandos) {
                if (partida.obterEstado() != EstadoPartida::EmAndamento) break;
                if (comando == Tecla::Space) {
                    partida.colocarBomba();
                } else if (auto passo = direcao(comando)) {
                    partida.moverJogador(*passo);
                    esperaMovimento = sf::milliseconds(140);
                    moveuPorEvento = true;
                }
            }
            if (partida.obterEstado() == EstadoPartida::EmAndamento) {
                if (!moveuPorEvento) esperaMovimento -= dt;
                if (!teclasMovimento.empty() && esperaMovimento <= sf::Time::Zero) {
                    partida.moverJogador(*direcao(teclasMovimento.back()));
                    // No maximo um passo por quadro; atrasos nao provocam saltos.
                    esperaMovimento = sf::milliseconds(140);
                }
            } else {
                limparEntrada();
            }
            if (partida.obterEstado() == EstadoPartida::Vitoria && partida.avancarFase()) {
                audio.parar();
                limparEntrada();
                relogio.restart();
            }
            // Eventos registram cada ocorrencia, inclusive explosoes que nasceram
            // e expiraram durante um quadro longo. Consumir evita repetir sons.
            for (auto evento : partida.consumirEventos()) audio.tocar(evento);
        }

        janela.clear(FUNDO);
        if (tela == Tela::Menu) desenharMenu(janela, fonte, menu, fundo);
        else if (tela == Tela::Instrucoes) desenharInstrucoes(janela, fonte);
        else {
            desenharPartida(janela, fonte, partida, sprites, relogioAnimacao.getElapsedTime());
            if (partida.obterEstado() == EstadoPartida::Vitoria) {
                desenharAviso(janela, fonte, "VOCE VENCEU!",
                              "4 fases concluidas! Enter ou R para novo jogo.", VERDE);
            } else if (partida.obterEstado() == EstadoPartida::Derrota) {
                desenharAviso(janela, fonte, "FIM DE JOGO", "Tente outra vez! Enter ou R reinicia a fase.", DOURADO);
            } else if (!janela.hasFocus()) {
                desenharAviso(janela, fonte, "PAUSADO", "Volte para esta janela para continuar.", SUAVE);
            }
        }
        janela.display();
    }
    return 0;
}
} // namespace

int main(int argc, char* argv[]) {
    try { return executar(argc, argv); }
    catch (const ErroEntrada& e) {
        std::cerr << "Erro de entrada: " << e.what() << '\n';
        return 2;
    }
    catch (const std::exception& e) {
        std::cerr << "Erro: " << e.what() << '\n';
        return 1;
    }
    catch (...) {
        std::cerr << "Erro: excecao desconhecida.\n";
        return 1;
    }
}