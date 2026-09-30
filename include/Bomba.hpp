#ifndef BOMBERMAN_BOMBA_HPP
#define BOMBERMAN_BOMBA_HPP

#include <SFML/System/Time.hpp>
#include <SFML/System/Vector2.hpp>

namespace bomberman {

class Jogador;
class Mapa;

/**
 * @class Bomba
 * @brief Controla o tempo até a detonação e o alcance definido pelo jogador responsável.
 */
class Bomba {
public:
    /**
     * @brief Cria uma bomba em uma posição da arena.
     * @param posicao Célula onde a bomba foi colocada.
     * @param alcance Distância, em células, atingida pela explosão.
     * @param responsavel Jogador que posicionou a bomba.
     */
    Bomba(sf::Vector2i posicao, int alcance, Jogador& responsavel);

    /** @brief Retorna a posição da bomba na arena. */
    sf::Vector2i getPosicao() const;

    /** @brief Retorna o alcance da explosão que a bomba irá gerar. */
    int getAlcance() const;

    /** @brief Indica se o tempo da bomba terminou e ela está pronta para detonar. */
    bool prontaParaExplodir() const;

    /**
     * @brief Avança o temporizador da bomba a cada ciclo da partida.
     * @param deltaTempo Tempo decorrido desde a última atualização.
     */
    void atualizar(sf::Time deltaTempo);

    /** @brief Antecipa a detonação (chamado ao ser atingida por outra explosão). */
    void anteciparDetonacao();

    /**
     * @brief Detona a bomba, gerando os efeitos da explosão na arena.
     * @param mapa Mapa onde a explosão ocorre.
     */
    void detonar(Mapa& mapa);

private:
    sf::Vector2i posicao_;
    int alcance_;
    Jogador& responsavel_;
    sf::Time tempoRestante_;
    bool detonada_ = false;
};

} // namespace bomberman

#endif