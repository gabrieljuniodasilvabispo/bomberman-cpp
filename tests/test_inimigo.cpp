#include "apoio.hpp"
TEST_CASE_FIXTURE(TestesInimigo, "Inimigo inicia ativo e e eliminado por explosao") {
    conferirPosicao(inimigo.obterPosicao(), {4, 0});
    CHECK(inimigo.ativo());
    inimigo.receberExplosao();
    CHECK_FALSE(inimigo.ativo());
    inimigo.receberExplosao();
    CHECK_FALSE(inimigo.ativo());
}
TEST_CASE_FIXTURE(TestesInimigo, "Inimigo com delta zero permanece na mesma celula") {
    inimigo.atualizar(sf::Time::Zero, mapa);
    conferirPosicao(inimigo.obterPosicao(), {4, 0});
}
TEST_CASE_FIXTURE(TestesInimigo, "Inimigo derrota jogador somente ao ocupar a mesma celula") {
    inimigo.verificarContato(jogador);
    CHECK(jogador.ativo());
    bomberman::Jogador proximo({4, 0});
    inimigo.verificarContato(proximo);
    CHECK_FALSE(proximo.ativo());
}
