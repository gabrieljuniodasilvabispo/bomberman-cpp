#ifndef BOMBERMAN_TESTES_TDD_PENDENTE_HPP
#define BOMBERMAN_TESTES_TDD_PENDENTE_HPP
#include <stdexcept>
// Exclusivo do executavel de testes. Nenhuma regra de jogo e implementada aqui.
[[noreturn]] inline void pendente(const char* metodo) {
    throw std::runtime_error(metodo);
}

#endif // BOMBERMAN_TESTES_TDD_PENDENTE_HPP
