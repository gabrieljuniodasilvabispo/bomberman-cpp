#include "apoio.hpp"
#include "Bomba.hpp"
#include "Explosao.hpp"
#include <stdexcept>
struct TestesBomba {
    bomberman::Jogador jogador{{0, 0}};
    bomberman::Bomba bomba{{2, 1}, 3, jogador};
};
TEST_CASE_FIXTURE(TestesBomba, "Bomba preserva posicao e alcance do responsavel") {
    CHECK(bomba.getPosicao() == sf::Vector2i(2, 1));
    CHECK(bomba.getAlcance() == 3);
    CHECK_FALSE(bomba.prontaParaExplodir());
}
TEST_CASE_FIXTURE(TestesBomba, "Bomba nao avanca temporizador com delta zero") {
    bomba.atualizar(sf::Time::Zero);
    CHECK_FALSE(bomba.prontaParaExplodir());
}
TEST_CASE_FIXTURE(TestesBomba, "Bomba permite antecipar detonacao de forma idempotente") {
    bomba.anteciparDetonacao();
    CHECK(bomba.prontaParaExplodir());
    bomba.anteciparDetonacao();
    CHECK(bomba.prontaParaExplodir());
}

TEST_CASE_FIXTURE(TestesBomba, "Bomba detona apos tres segundos sem retroceder o relogio") {
    bomba.atualizar(sf::milliseconds(2900));
    CHECK_FALSE(bomba.prontaParaExplodir());
    bomba.atualizar(sf::seconds(-1));
    bomba.atualizar(sf::milliseconds(100));
    CHECK(bomba.prontaParaExplodir());
}

TEST_CASE_FIXTURE(TestesBomba, "Detonar remove ocupacao e devolve exatamente uma vaga") {
    Mapa mapa(3, 5, {}, {0, 0}, {}, {2, 4});
    REQUIRE(mapa.registrarBomba({1, 2}));
    CHECK(bomba.detonar(mapa) == nullptr);
    CHECK(mapa.temBomba({1, 2}));
    CHECK(jogador.obterBombasAtivas() == 1);
    CHECK_THROWS_AS((bomberman::Bomba({0, 0}, 1, jogador)), std::logic_error);

    bomba.anteciparDetonacao();
    auto explosao = bomba.detonar(mapa);
    REQUIRE(explosao != nullptr);
    CHECK_FALSE(mapa.temBomba({1, 2}));
    CHECK(jogador.obterBombasAtivas() == 0);
    CHECK(jogador.obterLimiteBombas() == 1);

    bomberman::Bomba outra({0, 0}, 1, jogador);
    bomba.anteciparDetonacao();
    CHECK_FALSE(bomba.prontaParaExplodir());
    CHECK(bomba.detonar(mapa) == nullptr);
    CHECK(jogador.obterBombasAtivas() == 1);
    CHECK_THROWS_AS((bomberman::Bomba({1, 0}, 1, jogador)), std::logic_error);
}

TEST_CASE("Destruir bomba sem detonar devolve sua reserva") {
    bomberman::Jogador jogador({0, 0});
    {
        bomberman::Bomba bomba({0, 0}, 1, jogador);
        CHECK(jogador.obterBombasAtivas() == 1);
    }
    CHECK(jogador.obterBombasAtivas() == 0);
    CHECK_THROWS_AS((bomberman::Bomba({0, 0}, 0, jogador)), std::invalid_argument);
    CHECK(jogador.obterBombasAtivas() == 0);
}

TEST_CASE("Falha ao detonar fora do mapa preserva reserva e permite limpeza") {
    bomberman::Jogador jogador({0, 0});
    Mapa mapa(2, 3, {}, {0, 0}, {}, {1, 2});
    {
        bomberman::Bomba bomba({3, 0}, 1, jogador);
        bomba.anteciparDetonacao();
        CHECK_THROWS_AS(bomba.detonar(mapa), std::invalid_argument);
        CHECK(jogador.obterBombasAtivas() == 1);
    }
    CHECK(jogador.obterBombasAtivas() == 0);
}
