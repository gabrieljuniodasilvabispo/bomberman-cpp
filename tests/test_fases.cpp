#include "apoio.hpp"
#include "Partida.hpp"

#include <stdexcept>
#include <string>

namespace {
using bomberman::ConfiguracaoFase;
using bomberman::EstadoPartida;
using bomberman::Partida;
using bomberman::TipoMelhoria;

// Representa terreno, itens escondidos, personagens e porta para comparar reinicios.
std::string descreverMapa(const Mapa& original) {
    Mapa mapa = original;
    std::string descricao;
    for (int linha = 0; linha < mapa.obterLinhas(); ++linha) {
        for (int coluna = 0; coluna < mapa.obterColunas(); ++coluna) {
            const Posicao posicao{linha, coluna};
            char celula = '.';
            if (auto* bloco = mapa.obterBloco(posicao)) {
                celula = bloco->obterTipo() == TipoBloco::Indestrutivel ? '#' : '+';
                if (celula == '+') {
                    bloco->receberExplosao(mapa);
                    if (auto* melhoria = mapa.obterMelhoria(posicao)) {
                        switch (melhoria->getTipo()) {
                        case TipoMelhoria::AumentoAlcance: celula = 'A'; break;
                        case TipoMelhoria::BombaExtra: celula = 'B'; break;
                        case TipoMelhoria::Escudo: celula = 'E'; break;
                        default: celula = '?'; break;
                        }
                    }
                }
            }
            descricao += celula;
        }
    }
    const auto marcar = [&](Posicao posicao, char letra) {
        descricao[posicao.linha * mapa.obterColunas() + posicao.coluna] = letra;
    };
    marcar(mapa.obterInicioJogador(), 'J');
    for (auto posicao : mapa.obterIniciosInimigos()) marcar(posicao, 'I');
    marcar(mapa.obterSaida(), 'S');
    return descricao;
}
} // namespace

TEST_CASE("Jogo padrao tem quatro mapas e comeca com o mapa original") {
    Partida partida;
    CHECK(partida.obterQuantidadeFases() == 4);
    CHECK(partida.obterNumeroFase() == 0);
    partida.iniciarFase();
    CHECK(partida.obterNumeroFase() == 1);
    CHECK(descreverMapa(partida.obterMapa()) ==
        "###########"
        "#J..B..+.I#"
        "#.#.#.#.#.#"
        "#..E.+.+.+#"
        "#A#.#.#.#.#"
        "#+.+.+.+.+#"
        "#.#.#.#.#.#"
        "#I.+.+.+.S#"
        "###########");
    const auto inimigos = partida.obterMapa().obterIniciosInimigos();
    REQUIRE(inimigos.size() == 3);
    conferirPosicao(inimigos[0], {1, 9});
    conferirPosicao(inimigos[1], {7, 1});
    conferirPosicao(inimigos[2], {7, 9});
}

TEST_CASE("Reiniciar e comecar novo jogo repetem o mesmo primeiro mapa fixo") {
    Partida partida;
    partida.reiniciarFase(); // Tambem pode ser usado antes do primeiro inicio.
    const auto original = descreverMapa(partida.obterMapa());
    REQUIRE(partida.colocarBomba());
    REQUIRE(partida.moverJogador({0, 1}));
    partida.reiniciarFase();
    CHECK(partida.obterNumeroFase() == 1);
    CHECK(descreverMapa(partida.obterMapa()) == original);
    CHECK(partida.obterQuantidadeBombas() == 0);
    CHECK(partida.obterQuantidadeExplosoes() == 0);
    CHECK(partida.obterJogador().obterBombasAtivas() == 0);
    conferirPosicao(partida.obterJogador().obterPosicao(), partida.obterMapa().obterInicioJogador());
    CHECK(partida.consumirEventos().empty());
    partida.iniciarFase();
    CHECK(descreverMapa(partida.obterMapa()) == original);
}

TEST_CASE("Portas seguem a ordem das quatro fases e terminam sem criar uma quinta") {
    std::vector<ConfiguracaoFase> fases(4);
    for (std::size_t i = 0; i < fases.size(); ++i) {
        fases[i].linhas = 3;
        fases[i].colunas = 5 + static_cast<int>(i);
        fases[i].inicioJogador = {0, 0};
        fases[i].saida = {0, 1};
    }
    Partida partida(fases);
    partida.iniciarFase();
    for (std::size_t numero = 1; numero <= 4; ++numero) {
        CAPTURE(numero);
        CHECK(partida.obterNumeroFase() == numero);
        CHECK(partida.obterMapa().obterColunas() == 4 + static_cast<int>(numero));
        CHECK(partida.obterEstado() == EstadoPartida::EmAndamento);
        CHECK_FALSE(partida.avancarFase());
        REQUIRE(partida.colocarBomba());
        REQUIRE(partida.moverJogador({0, 1}));
        REQUIRE(partida.obterEstado() == EstadoPartida::Vitoria);
        if (numero < 4) {
            REQUIRE(partida.avancarFase());
            CHECK(partida.obterQuantidadeBombas() == 0);
            CHECK(partida.obterJogador().obterBombasAtivas() == 0);
            CHECK(partida.consumirEventos().empty());
            const auto mapa = descreverMapa(partida.obterMapa());
            partida.reiniciarFase();
            CHECK(partida.obterNumeroFase() == numero + 1);
            CHECK(descreverMapa(partida.obterMapa()) == mapa);
        }
    }
    CHECK_FALSE(partida.avancarFase());
    CHECK(partida.obterNumeroFase() == 4);
    CHECK(partida.obterEstado() == EstadoPartida::Vitoria);
    partida.atualizar(sf::seconds(4));
    CHECK(partida.obterJogador().ativo());
    CHECK_FALSE(partida.colocarBomba());
    partida.iniciarFase();
    CHECK(partida.obterNumeroFase() == 1);
    CHECK(partida.obterMapa().obterColunas() == 5);
    CHECK(partida.obterEstado() == EstadoPartida::EmAndamento);
}

TEST_CASE("Avanco recusa partida nao iniciada derrota encerramento e vitoria sem proxima fase") {
    ConfiguracaoFase fase;
    fase.linhas = 3;
    fase.colunas = 4;
    fase.inicioJogador = {1, 1};
    fase.saida = {1, 2};
    Partida partida(fase);
    CHECK_FALSE(partida.avancarFase());
    CHECK(partida.obterNumeroFase() == 0);
    partida.iniciarFase();
    SUBCASE("em andamento") {}
    SUBCASE("encerrada") { partida.encerrarFase(); }
    SUBCASE("derrota") {
        REQUIRE(partida.colocarBomba());
        partida.atualizar(sf::seconds(3));
        REQUIRE(partida.obterEstado() == EstadoPartida::Derrota);
    }
    SUBCASE("fase unica concluida") {
        REQUIRE(partida.moverJogador({0, 1}));
        REQUIRE(partida.obterEstado() == EstadoPartida::Vitoria);
    }
    const auto estado = partida.obterEstado();
    const auto mapa = descreverMapa(partida.obterMapa());
    CHECK_FALSE(partida.avancarFase());
    CHECK(partida.obterEstado() == estado);
    CHECK(partida.obterNumeroFase() == 1);
    CHECK(descreverMapa(partida.obterMapa()) == mapa);
}

TEST_CASE("Sequencia vazia e rejeitada e fase invalida nao substitui a anterior") {
    CHECK_THROWS_AS(Partida(std::vector<ConfiguracaoFase>{}), std::invalid_argument);
    ConfiguracaoFase primeira;
    primeira.inicioJogador = primeira.saida;
    auto invalida = primeira;
    invalida.linhas = 0;
    Partida partida(std::vector<ConfiguracaoFase>{primeira, invalida});
    partida.iniciarFase();
    const auto mapa = descreverMapa(partida.obterMapa());
    CHECK_THROWS_AS(partida.avancarFase(), std::invalid_argument);
    CHECK(partida.obterNumeroFase() == 1);
    CHECK(partida.obterEstado() == EstadoPartida::Vitoria);
    CHECK(descreverMapa(partida.obterMapa()) == mapa);
}
