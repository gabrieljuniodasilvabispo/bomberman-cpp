#include "apoio.hpp"
#include <stdexcept>

TEST_CASE_FIXTURE(TestesMapa, "Mapa preserva dimensoes e configuracao inicial") {
    CHECK(mapa.obterLinhas() == 5);
    CHECK(mapa.obterColunas() == 5);
    conferirPosicao(mapa.obterInicioJogador(), {0, 0});
    conferirPosicao(mapa.obterSaida(), {4, 4});
    auto inimigos = mapa.obterIniciosInimigos();
    REQUIRE(inimigos.size() == 1);
    conferirPosicao(inimigos.front(), {4, 0});
}
TEST_CASE_FIXTURE(TestesMapa, "Mapa rejeita entrada fora dos limites e em blocos") {
    for (Posicao p : {Posicao{-1, 0}, {0, -1}, {5, 0}, {0, 5}}) {
        CHECK_FALSE(mapa.dentroDosLimites(p));
        CHECK_FALSE(mapa.podeEntrar(p));
        CHECK(mapa.obterBloco(p) == nullptr);
    }
    CHECK(mapa.dentroDosLimites({4, 4}));
    CHECK(mapa.podeEntrar({0, 0}));
    CHECK_FALSE(mapa.podeEntrar({1, 1}));
    CHECK_FALSE(mapa.podeEntrar({2, 2}));
    const Mapa& consulta = mapa;
    REQUIRE(consulta.obterBloco({2, 2}) != nullptr);
    CHECK(consulta.obterBloco({2, 2})->obterTipo() == TipoBloco::Indestrutivel);
}
TEST_CASE_FIXTURE(TestesMapa, "Mapa registra e remove ocupacao por bomba") {
    CHECK(mapa.registrarBomba({0, 1}));
    CHECK(mapa.temBomba({0, 1}));
    CHECK_FALSE(mapa.podeEntrar({0, 1}));
    CHECK_FALSE(mapa.registrarBomba({0, 1}));
    CHECK_FALSE(mapa.registrarBomba({1, 1}));
    CHECK_FALSE(mapa.registrarBomba({-1, 0}));
    mapa.removerBomba({0, 1});
    mapa.removerBomba({0, 1});
    mapa.removerBomba({-1, 0});
    CHECK_FALSE(mapa.temBomba({0, 1}));
    CHECK(mapa.podeEntrar({0, 1}));
}
TEST_CASE_FIXTURE(TestesMapa, "Mapa registra melhoria sem assumir propriedade") {
    bomberman::Melhoria melhoria({1, 0}, bomberman::TipoMelhoria::BombaExtra);
    mapa.registrarMelhoria({0, 1}, melhoria); // SFML x=coluna, y=linha.
    CHECK(mapa.obterMelhoria({0, 1}) == &melhoria);
    CHECK(mapa.podeEntrar({0, 1}));
    mapa.removerMelhoria({0, 1});
    CHECK(mapa.obterMelhoria({0, 1}) == nullptr);
    CHECK_FALSE(melhoria.coletada());
}
TEST_CASE_FIXTURE(TestesMapa, "Mapa reinicia terreno bombas e melhorias") {
    auto* bloco = mapa.obterBloco({1, 1});
    REQUIRE(bloco != nullptr);
    bloco->receberExplosao(mapa);
    REQUIRE(mapa.registrarBomba({0, 1}));
    bomberman::Melhoria melhoria({3, 0}, bomberman::TipoMelhoria::BombaExtra);
    mapa.registrarMelhoria({0, 3}, melhoria);
    mapa.reiniciar();
    CHECK_FALSE(mapa.temBomba({0, 1}));
    CHECK(mapa.obterMelhoria({0, 3}) == nullptr);
    REQUIRE(mapa.obterBloco({1, 1}) != nullptr);
    CHECK_FALSE(mapa.obterBloco({1, 1})->estaDestruido());
    CHECK_FALSE(mapa.podeEntrar({1, 1}));
}
struct TestesValidacaoMapa {};
TEST_CASE_FIXTURE(TestesValidacaoMapa, "Mapa rejeita dimensoes invalidas") {
    CHECK_THROWS_AS((Mapa(0, 5, {}, {0, 0}, {}, {0, 1})), std::invalid_argument);
    CHECK_THROWS_AS((Mapa(5, -1, {}, {0, 0}, {}, {0, 1})), std::invalid_argument);
}
TEST_CASE_FIXTURE(TestesValidacaoMapa, "Mapa rejeita personagens sobrepostos ou fora da arena") {
    CHECK_THROWS_AS((Mapa(5, 5, {}, {0, 0}, {{0, 0}}, {4, 4})), std::invalid_argument);
    CHECK_THROWS_AS((Mapa(5, 5, {}, {-1, 0}, {}, {4, 4})), std::invalid_argument);
    CHECK_THROWS_AS((Mapa(5, 5, {}, {0, 0}, {}, {5, 4})), std::invalid_argument);
}
