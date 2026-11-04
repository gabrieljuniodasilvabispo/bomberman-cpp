#include "apoio.hpp"
#include <limits>
#include <stdexcept>
TEST_CASE_FIXTURE(TestesJogador, "Jogador inicia ativo na posicao informada") {
    CHECK(jogador.ativo());
    conferirPosicao(jogador.obterPosicao(), {0, 0});
    CHECK(jogador.obterAlcance() > 0);
    CHECK(jogador.obterLimiteBombas() > 0);
    CHECK_FALSE(jogador.temEscudo());
}
TEST_CASE_FIXTURE(TestesJogador, "Jogador movimenta nas quatro direcoes em celulas livres") {
    for (Posicao passo : {Posicao{0, 1}, {0, -1}, {1, 0}, {-1, 0}})
        CHECK(jogador.mover(passo, mapa));
    conferirPosicao(jogador.obterPosicao(), {0, 0});
}
TEST_CASE_FIXTURE(TestesJogador, "Jogador nao atravessa limites blocos ou bombas") {
    CHECK_FALSE(jogador.mover({-1, 0}, mapa));
    REQUIRE(jogador.mover({0, 1}, mapa));
    CHECK_FALSE(jogador.mover({1, 0}, mapa));
    REQUIRE(mapa.registrarBomba({0, 2}));
    CHECK_FALSE(jogador.mover({0, 1}, mapa));
    conferirPosicao(jogador.obterPosicao(), {0, 1});
}
TEST_CASE_FIXTURE(TestesJogador, "Jogador sem escudo e derrotado por explosao") {
    jogador.receberExplosao();
    CHECK_FALSE(jogador.ativo());
}

TEST_CASE_FIXTURE(TestesJogador, "Jogador rejeita diagonais saltos e deslocamento nulo") {
    CHECK_FALSE(jogador.mover({0, 0}, mapa));
    CHECK_FALSE(jogador.mover({2, 2}, mapa));
    CHECK_FALSE(jogador.mover({1, 2}, mapa));
    CHECK_FALSE(jogador.mover({2, 0}, mapa));
    conferirPosicao(jogador.obterPosicao(), {0, 0});
    REQUIRE(jogador.mover({0, 1}, mapa));
    CHECK_FALSE(jogador.mover({2, 0}, mapa)); // Nao pula o bloco em {1, 1}.
    conferirPosicao(jogador.obterPosicao(), {0, 1});
    jogador.receberExplosao();
    CHECK_FALSE(jogador.mover({0, 1}, mapa));
}

TEST_CASE("Jogador rejeita coordenadas negativas na criacao") {
    CHECK_THROWS_AS((bomberman::Jogador({-1, 0})), std::invalid_argument);
    CHECK_THROWS_AS((bomberman::Jogador({0, -1})), std::invalid_argument);
}

TEST_CASE("Jogador fora do mapa nao soma deslocamento a coordenada extrema") {
    const int maximo = std::numeric_limits<int>::max();
    bomberman::Jogador jogador({maximo, maximo});
    Mapa mapa(2, 2, {}, {0, 0}, {}, {1, 1});
    CHECK_FALSE(jogador.mover({1, 0}, mapa));
    CHECK_FALSE(jogador.mover({0, 1}, mapa));
    conferirPosicao(jogador.obterPosicao(), {maximo, maximo});
}
