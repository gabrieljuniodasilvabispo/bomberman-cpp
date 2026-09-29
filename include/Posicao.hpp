#ifndef BOMBERMAN_POSICAO_HPP
#define BOMBERMAN_POSICAO_HPP

/** @file Posicao.hpp
 * @brief Coordenadas logicas compartilhadas pelas classes da arena.
 */

/** @brief Celula da grade, com indices a partir de zero; nao representa pixels. */
struct Posicao {
    int linha;  ///< Indice da linha, crescente para baixo.
    int coluna; ///< Indice da coluna, crescente para a direita.
};

#endif // BOMBERMAN_POSICAO_HPP
