#ifndef BOMBERMAN_JOGADOR_HPP
#define BOMBERMAN_JOGADOR_HPP

#include "Posicao.hpp"
class Mapa;

namespace bomberman {
/**
 * @brief Personagem controlavel, com atributos e reservas de bombas.
 * @note Possui identidade estavel: destrua suas bombas e explosoes antes de
 * destruir/recriar o jogador no reinicio. Elas mantem referencias emprestadas.
 */
class Jogador {
public:
    explicit Jogador(Posicao posicao);
    Jogador(const Jogador&) = delete;
    Jogador& operator=(const Jogador&) = delete;
    Jogador(Jogador&&) = delete;
    Jogador& operator=(Jogador&&) = delete;
    Posicao obterPosicao() const;
    bool ativo() const;
    /** @brief Tenta um passo ortogonal unitario; retorna false se bloqueado. */
    bool mover(Posicao deslocamento, ::Mapa& mapa);
    int obterAlcance() const;
    int obterLimiteBombas() const;
    int obterBombasAtivas() const;
    /** @brief Reserva uma vaga, se o jogador estiver ativo e houver capacidade. */
    bool reservarVagaBomba();
    /** @brief Devolve uma vaga ocupada, sem alterar o limite de bombas. */
    void liberarVagaBomba();
    bool temEscudo() const;
    /** @brief Aplica o aumento de alcance de uma melhoria. */
    void aumentarAlcance();
    /** @brief Aplica o aumento de bombas simultaneas de uma melhoria. */
    void aumentarLimiteBombas();
    /** @brief Concede protecao contra uma explosao. */
    void ativarEscudo();
    /** @brief Consome o escudo, se presente; caso contrario derrota o jogador. */
    void receberExplosao();
    /** @brief Contato com inimigo derrota mesmo um jogador com escudo. */
    void receberContatoInimigo();

private:
    Posicao posicao_;
    bool ativo_ = true;
    bool escudo_ = false;
    int alcance_ = 2;
    int limiteBombas_ = 1;
    int bombasAtivas_ = 0;
};
}
#endif
