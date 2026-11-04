#ifndef BOMBERMAN_INIMIGO_HPP
#define BOMBERMAN_INIMIGO_HPP

#include "Posicao.hpp"
#include <SFML/System/Time.hpp>

class Mapa;

namespace bomberman {

class Jogador;

class Inimigo {
public:
    /** @throws std::invalid_argument Se linha ou coluna for negativa. */
    explicit Inimigo(Posicao posicao);
    Posicao obterPosicao() const;
    bool ativo() const;
    /**
     * @brief Tenta um passo a cada 500 ms, preservando o tempo entre chamadas.
     * @note Inimigos inativos e tempos nao positivos nao alteram o estado.
     * @throws std::invalid_argument Se o inimigo ativo estiver fora do mapa
     * ou deltaTempo superar 5 segundos. A falha preserva posicao e acumulador.
     * @note O limite impede processamento excessivo apos uma pausa longa;
     * o controlador deve descartar esse tempo ao retomar a partida.
     */
    void atualizar(sf::Time deltaTempo, ::Mapa& mapa);
    void receberExplosao();
    void verificarContato(Jogador& jogador);

private:
    Posicao posicao_;
    bool ativo_ = true;
    sf::Time tempoAcumulado_ = sf::Time::Zero;
};

} // namespace bomberman

#endif
