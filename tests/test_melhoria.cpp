#include "apoio.hpp"
struct TestesMelhoria {
    bomberman::Jogador jogador{{0, 0}};
};
TEST_CASE_FIXTURE(TestesMelhoria, "Melhoria de alcance so aplica o beneficio uma vez") {
    bomberman::Melhoria melhoria({3, 1}, bomberman::TipoMelhoria::AumentoAlcance);
    CHECK(melhoria.getPosicao() == sf::Vector2i(3, 1));
    CHECK(melhoria.getTipo() == bomberman::TipoMelhoria::AumentoAlcance);
    CHECK_FALSE(melhoria.coletada());
    const int antes = jogador.obterAlcance();
    melhoria.aplicarEm(jogador);
    CHECK(melhoria.coletada());
    const int depois = jogador.obterAlcance();
    CHECK(depois > antes);
    melhoria.aplicarEm(jogador);
    CHECK(jogador.obterAlcance() == depois);
}
TEST_CASE_FIXTURE(TestesMelhoria, "Melhoria de bomba aumenta limite simultaneo") {
    bomberman::Melhoria melhoria({0, 0}, bomberman::TipoMelhoria::BombaExtra);
    const int antes = jogador.obterLimiteBombas();
    melhoria.aplicarEm(jogador);
    CHECK(jogador.obterLimiteBombas() > antes);
    CHECK(melhoria.coletada());
}
TEST_CASE_FIXTURE(TestesMelhoria, "Melhoria de escudo protege de uma unica explosao") {
    bomberman::Melhoria melhoria({0, 0}, bomberman::TipoMelhoria::Escudo);
    melhoria.aplicarEm(jogador);
    REQUIRE(jogador.temEscudo());
    jogador.receberExplosao();
    CHECK(jogador.ativo());
    CHECK_FALSE(jogador.temEscudo());
    jogador.receberExplosao();
    CHECK_FALSE(jogador.ativo());
}
