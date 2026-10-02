#ifndef BOMBERMAN_MELHORIA_HPP
#define BOMBERMAN_MELHORIA_HPP

#include <SFML/System/Time.hpp>
#include <SFML/System/Vector2.hpp>

namespace bomberman {

class Jogador;
class Mapa;

/// @brief Tipos de melhorias (Power-ups) disponíveis no jogo.
enum class TipoMelhoria {
    AumentoAlcance,
    BombaExtra,
    AumentoVelocidade
};

/**
 * @class Melhoria
 * @brief Representa um item coletável no mapa que altera os atributos do jogador.
 */
class Melhoria {
public:
    /**
     * @brief Constrói uma melhoria em uma posição do mapa.
     * @param posicao Célula (x, y) onde a melhoria será gerada.
     * @param tipo O tipo de benefício concedido por este item.
     */
    Melhoria(sf::Vector2i posicao, TipoMelhoria tipo);

    /**
     * @brief Aplica o efeito do item diretamente sobre o jogador.
     * @param jogador Referência ao jogador que coletou a melhoria.
     */
    void aplicarEm(Jogador& jogador);

    /** @brief Indica se o item já foi coletado por algum personagem. */
    bool coletada() const;

    /**
     * @brief Avança a animação ou tempo de permanência do item no mapa.
     * @param deltaTempo Tempo decorrido desde o último quadro.
     */
    void atualizar(sf::Time deltaTempo);

    /** @brief Retorna a posição da célula onde o item se encontra. */
    sf::Vector2i getPosicao() const;

    /** @brief Retorna o tipo de melhoria que este item representa. */
    TipoMelhoria getTipo() const;

private:
    sf::Vector2i posicao_;
    TipoMelhoria tipo_;
    bool coletada_;
};

} // namespace bomberman

#endif // BOMBERMAN_MELHORIA_HPP
