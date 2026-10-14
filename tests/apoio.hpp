#ifndef BOMBERMAN_TESTES_APOIO_HPP
#define BOMBERMAN_TESTES_APOIO_HPP
#include <doctest/doctest.h>
#include "Mapa.hpp"
#include "Player.hpp"
#include "Enemy.hpp"
#include "Melhoria.hpp"

inline void conferirPosicao(Posicao atual, Posicao esperada) {
    CHECK(atual.linha == esperada.linha);
    CHECK(atual.coluna == esperada.coluna);
}

struct TestesMapa {
    Mapa mapa{5, 5, {Bloco({1, 1}, TipoBloco::Destrutivel),
                     Bloco({2, 2}, TipoBloco::Indestrutivel)},
              {0, 0}, {{4, 0}}, {4, 4}};
};
struct TestesBloco : TestesMapa {};
struct TestesJogador : TestesMapa {
    bomberman::Jogador jogador{{0, 0}};
};
struct TestesInimigo : TestesJogador {
    bomberman::Inimigo inimigo{{4, 0}};
};

#endif // BOMBERMAN_TESTES_APOIO_HPP
