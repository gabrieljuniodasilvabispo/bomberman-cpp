#include "apoio.hpp"
TEST_CASE_FIXTURE(TestesBloco, "Bloco destrutivel libera celula e so interrompe o primeiro impacto") {
    auto* bloco = mapa.obterBloco({1, 1});
    REQUIRE(bloco != nullptr);
    conferirPosicao(bloco->obterPosicao(), {1, 1});
    CHECK(bloco->obterTipo() == TipoBloco::Destrutivel);
    CHECK_FALSE(bloco->estaDestruido());
    CHECK(bloco->impedePassagem());
    CHECK(bloco->receberExplosao(mapa));
    CHECK(bloco->estaDestruido());
    CHECK_FALSE(bloco->impedePassagem());
    CHECK(mapa.podeEntrar({1, 1}));
    CHECK(mapa.obterBloco({1, 1}) == nullptr);
    CHECK_FALSE(bloco->receberExplosao(mapa));
}
TEST_CASE_FIXTURE(TestesBloco, "Bloco indestrutivel resiste a impactos repetidos") {
    auto* bloco = mapa.obterBloco({2, 2});
    REQUIRE(bloco != nullptr);
    CHECK(bloco->receberExplosao(mapa));
    CHECK(bloco->receberExplosao(mapa));
    CHECK_FALSE(bloco->estaDestruido());
    CHECK(bloco->impedePassagem());
}
struct TestesRevelacaoBloco {};
TEST_CASE_FIXTURE(TestesRevelacaoBloco, "Bloco revela melhoria uma unica vez") {
    bomberman::Melhoria melhoria({1, 1}, bomberman::TipoMelhoria::Escudo);
    Mapa arena(3, 3, {Bloco({1, 1}, TipoBloco::Destrutivel, &melhoria)},
               {0, 0}, {}, {2, 2});
    CHECK(arena.obterMelhoria({1, 1}) == nullptr);
    auto* bloco = arena.obterBloco({1, 1});
    REQUIRE(bloco != nullptr);
    bloco->receberExplosao(arena);
    CHECK(arena.obterMelhoria({1, 1}) == &melhoria);
    arena.removerMelhoria({1, 1});
    bloco->receberExplosao(arena);
    CHECK(arena.obterMelhoria({1, 1}) == nullptr);
}
