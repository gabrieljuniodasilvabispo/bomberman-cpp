#include "apoio.hpp"
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
