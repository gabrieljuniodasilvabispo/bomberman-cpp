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

TEST_CASE("Inimigo acumula tempo independentemente da divisao em quadros") {
    // Apenas um destino livre por passo: nao depende da escolha aleatoria.
    Mapa corredor(1, 2, {}, {0, 0}, {{0, 1}}, {0, 0});
    bomberman::Inimigo inteiro({0, 1}), fracionado({0, 1});
    fracionado.atualizar(sf::milliseconds(250), corredor);
    conferirPosicao(fracionado.obterPosicao(), {0, 1});
    fracionado.atualizar(sf::seconds(-1), corredor);
    fracionado.atualizar(sf::milliseconds(250), corredor);
    inteiro.atualizar(sf::milliseconds(500), corredor);
    conferirPosicao(fracionado.obterPosicao(), {0, 0});
    conferirPosicao(inteiro.obterPosicao(), fracionado.obterPosicao());

    fracionado.atualizar(sf::milliseconds(750), corredor);
    conferirPosicao(fracionado.obterPosicao(), {0, 1});
    fracionado.atualizar(sf::milliseconds(250), corredor);
    conferirPosicao(fracionado.obterPosicao(), {0, 0});
    inteiro.atualizar(sf::milliseconds(1000), corredor);
    conferirPosicao(inteiro.obterPosicao(), fracionado.obterPosicao());
}

TEST_CASE("Inimigo respeita obstaculos com tempo positivo e para apos ser eliminado") {
    Mapa corredor(1, 2, {}, {0, 0}, {{0, 1}}, {0, 0});
    bomberman::Inimigo inimigo({0, 1});
    REQUIRE(corredor.registrarBomba({0, 0}));
    inimigo.atualizar(sf::seconds(1), corredor);
    conferirPosicao(inimigo.obterPosicao(), {0, 1});
    corredor.removerBomba({0, 0});
    inimigo.atualizar(sf::milliseconds(500), corredor);
    conferirPosicao(inimigo.obterPosicao(), {0, 0});
    inimigo.receberExplosao();
    inimigo.atualizar(sf::seconds(1), corredor);
    conferirPosicao(inimigo.obterPosicao(), {0, 0});
}
