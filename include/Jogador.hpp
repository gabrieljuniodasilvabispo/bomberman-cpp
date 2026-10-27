#ifndef BOMBERMAN_JOGADOR_HPP
#define BOMBERMAN_JOGADOR_HPP

#include "Posicao.hpp"
class Mapa;

namespace bomberman {
/** @brief Contrato inicial para CRC03, US02, US03 e US05; sem implementacao. */
class Jogador {
public:
    explicit Jogador(Posicao posicao);
    Posicao obterPosicao() const;
    bool ativo() const;
    /** @brief Tenta um passo ortogonal unitario; retorna false se bloqueado. */
    bool mover(Posicao deslocamento, ::Mapa& mapa);
    int obterAlcance() const;
    int obterLimiteBombas() const;
    bool temEscudo() const;
    /** @brief Consome o escudo, se presente; caso contrario derrota o jogador. */
    void receberExplosao();
    /** @brief Contato com inimigo derrota mesmo um jogador com escudo. */
    void receberContatoInimigo();
};
}
#endif
