#include "apoio.hpp"
#include "Bomba.hpp"
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
