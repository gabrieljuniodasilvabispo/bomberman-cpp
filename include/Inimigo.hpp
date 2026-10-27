#ifndef BOMBERMAN_INIMIGO_HPP
#define BOMBERMAN_INIMIGO_HPP

#include "Posicao.hpp"
#include <SFML/System/Time.hpp>
class Mapa;

namespace bomberman {
class Jogador;
/** @brief Contrato inicial para CRC06 e US04; sem implementacao. */
class Inimigo {
public:
    explicit Inimigo(Posicao posicao);
    Posicao obterPosicao() const;
    bool ativo() const;
    void atualizar(sf::Time deltaTempo, ::Mapa& mapa);
    void receberExplosao();
    void verificarContato(Jogador& jogador);
};
}
#endif
