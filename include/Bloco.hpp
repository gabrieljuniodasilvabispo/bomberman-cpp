#ifndef BOMBERMAN_BLOCO_HPP
#define BOMBERMAN_BLOCO_HPP

#include "Posicao.hpp"

class Mapa;
class Melhoria;

/** @file Bloco.hpp
 * @brief Contrato de Bloco, baseado no CRC08 e nas historias US02, US04 e US05.
 */

/** @brief Tipos de obstaculo previstos na modelagem. */
enum class TipoBloco {
    Destrutivel,   ///< E destruido ao receber uma explosao.
    Indestrutivel  ///< Permanece intacto ao receber uma explosao.
};

/**
 * @brief Obstaculo que conhece sua posicao e trata impactos de explosoes.
 * @details Interface da etapa C6. Atributos privados e corpos dos metodos
 * serao definidos na implementacao. Nao representa o chao nem uma bomba.
 */
class Bloco {
public:
    /**
     * @brief Define um bloco inicialmente intacto.
     * @param posicao Celula do bloco; deve pertencer ao mapa que o recebera.
     * @param tipo Comportamento diante de uma explosao.
     * @param melhoria Melhoria escondida, ou nullptr quando nao houver.
     * @pre Uma melhoria so pode estar escondida em um bloco destrutivel.
     * @note A melhoria pertence a Partida e deve permanecer viva enquanto
     * referenciada pelo bloco ou mapa. O bloco nao a exclui.
     */
    Bloco(Posicao posicao, TipoBloco tipo, Melhoria* melhoria = nullptr);

    /** @brief Consulta a celula ocupada. @return Posicao logica do bloco. */
    Posicao obterPosicao() const;

    /** @brief Consulta o tipo. @return Tipo definido na criacao. */
    TipoBloco obterTipo() const;

    /** @brief Consulta o estado. @return true se o bloco foi destruido. */
    bool estaDestruido() const;

    /** @brief Consulta o bloqueio. @return true enquanto estiver intacto. */
    bool impedePassagem() const;

    /**
     * @brief Trata o impacto e notifica o mapa se houver destruicao.
     * @param mapa Mapa que contem este bloco.
     * @return true se o bloco estava intacto e interrompe esta propagacao,
     * inclusive quando acaba de ser destruido; false se ja estava destruido.
     * @pre Este bloco pertence ao mapa informado.
     * @post Um bloco indestrutivel permanece intacto. Um destrutivel fica
     * destruido, notifica liberarBlocoDestruido e revela sua melhoria, se houver,
     * por registrarMelhoria. Impactos repetidos nao repetem a revelacao.
     * @note O mapa preserva o objeto durante a notificacao.
     */
    bool receberExplosao(Mapa& mapa);
};

#endif // BOMBERMAN_BLOCO_HPP
