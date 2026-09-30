#ifndef BOMBERMAN_EXPLOSAO_HPP
#define BOMBERMAN_EXPLOSAO_HPP

#include <SFML/System/Time.hpp>
#include <SFML/System/Vector2.hpp>

#include <vector>

namespace bomberman {

class Mapa;
class Jogador;
class Inimigo;

/**
 * @class Explosao
 * @brief Calcula as células atingidas a partir de uma bomba detonada e
 *        aplica seus efeitos sobre o mapa e os personagens.
 */
class Explosao {
public:
    /**
     * @brief Cria uma explosão a partir de uma origem e um alcance.
     * @param origem Célula onde a bomba detonou.
     * @param alcance Distância máxima, em células, atingida em cada direção.
     * @param mapa Mapa onde a explosão se propaga.
     */
    Explosao(sf::Vector2i origem, int alcance, Mapa& mapa);

    /** @brief Calcula as células atingidas nas quatro direções a partir da origem. */
    void propagar();

    /** @brief Indica se a explosão ainda está ativa (visível/com efeito). */
    bool ativa() const;

    /**
     * @brief Avança o tempo de duração do efeito da explosão.
     * @param deltaTempo Tempo decorrido desde a última atualização.
     */
    void atualizar(sf::Time deltaTempo);

    /** @brief Retorna as células atingidas pela explosão. */
    const std::vector<sf::Vector2i>& getCelulasAtingidas() const;

    /**
     * @brief Aplica o efeito da explosão sobre o jogador, caso ele esteja em uma célula atingida.
     * @param jogador Jogador a ser verificado.
     */
    void aplicarEm(Jogador& jogador);

    /**
     * @brief Aplica o efeito da explosão sobre um inimigo, caso ele esteja em uma célula atingida.
     * @param inimigo Inimigo a ser verificado.
     */
    void aplicarEm(Inimigo& inimigo);

private:
    sf::Vector2i origem_;
    int alcance_;
    Mapa& mapa_;
    std::vector<sf::Vector2i> celulasAtingidas_;
    sf::Time tempoRestante_;
};

} // namespace bomberman

#endif