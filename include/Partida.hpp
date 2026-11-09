#ifndef BOMBERMAN_PARTIDA_HPP
#define BOMBERMAN_PARTIDA_HPP

#include "Bloco.hpp"
#include "Inimigo.hpp"
#include "Melhoria.hpp"
#include "Posicao.hpp"
#include <SFML/System/Time.hpp>
#include <cstddef>
#include <memory>
#include <optional>
#include <vector>

class Mapa;

namespace bomberman {

using ::Mapa;
class Jogador;
class Bomba;
class Explosao;

/** @brief Descricao de um bloco inicial, sem referencias a objetos externos. */
struct BlocoInicial {
    Posicao posicao;
    TipoBloco tipo;
    std::optional<TipoMelhoria> melhoria = std::nullopt;
};

/** @brief Dados copiados pela partida e reutilizados em cada reinicio. */
struct ConfiguracaoFase {
    int linhas = 9;
    int colunas = 11;
    std::vector<BlocoInicial> blocos;
    Posicao inicioJogador{1, 1};
    std::vector<Posicao> iniciosInimigos;
    Posicao saida{7, 9};
};

enum class EstadoPartida {
    NaoIniciada,
    EmAndamento,
    Vitoria,
    Derrota,
    Encerrada ///< Encerramento manual, por exemplo ao voltar para o menu.
};

/** @brief Acontecimentos consumidos pela interface, sem reproduzir audio nas regras. */
enum class EventoPartida {
    PassoHorizontal,
    PassoVertical,
    BombaColocada,
    BombaExplodiu,
    MelhoriaColetada,
    Derrota
};

/**
 * @brief Coordena as regras da fase, sem ler teclado ou desenhar a janela.
 * @note Possui mapa, personagens, melhorias, bombas e explosoes. Consultas
 * emprestadas deixam de valer ao iniciar/reiniciar ou destruir a partida;
 * bombas e explosoes tambem podem desaparecer durante a atualizacao.
 */
class Partida {
public:
    /** @brief Prepara a configuracao de uma arena fixa com as tres melhorias. */
    Partida();
    /** @brief Guarda uma configuracao propria; a validacao ocorre ao iniciar. */
    explicit Partida(ConfiguracaoFase configuracao);
    ~Partida();
    Partida(const Partida&) = delete;
    Partida& operator=(const Partida&) = delete;
    Partida(Partida&&) = delete;
    Partida& operator=(Partida&&) = delete;

    /**
     * @brief Cria todos os elementos em seus estados iniciais.
     * @throws std::invalid_argument Para configuracao invalida ou melhoria
     * de velocidade (ainda nao suportada). Aplicam-se tambem os limites de Mapa.
     * @note Prepara a fase antes de substituir a anterior, preservando-a em falha.
     */
    void iniciarFase();
    /**
     * @brief Atualiza temporizadores, inimigos, dano, cadeias, coleta e resultado.
     * @note Usa passos de no maximo 10 ms para verificar contatos entre movimentos.
     * Tempos nao positivos ou maiores que 5 s sao descartados (pausa longa).
     * Fases encerradas nao avancam. Derrota tem precedencia sobre vitoria.
     */
    void atualizar(sf::Time deltaTempo);
    /** @brief Tenta um passo unitario e resolve dano, coleta e resultado imediatamente. */
    bool moverJogador(Posicao deslocamento);
    /**
     * @brief Coloca uma bomba na celula do jogador e registra sua ocupacao.
     * @return false fora da partida, sem vaga ou com celula ocupada.
     * @note Uma bomba colocada em fogo ativo detona no mesmo comando.
     */
    bool colocarBomba();
    /** @brief Retorna false antes do inicio; depois consulta os inimigos reais. */
    bool todosInimigosEliminados() const;
    /** @brief Interrompe uma fase em andamento, preservando o estado para consulta. */
    void encerrarFase();
    /** @brief Recria a fase, inclusive itens coletados, atributos e terreno. */
    void reiniciarFase();

    /**
     * @brief Retira os acontecimentos acumulados, na ordem em que ocorreram.
     * @note Cada evento e entregue uma unica vez. Inicio, reinicio e encerramento
     * descartam eventos antigos. A interface deve consumir a fila a cada quadro.
     */
    std::vector<EventoPartida> consumirEventos();

    EstadoPartida obterEstado() const;
    /** @throws std::logic_error Se a fase ainda nao foi iniciada. */
    const Mapa& obterMapa() const;
    /** @throws std::logic_error Se a fase ainda nao foi iniciada. */
    const Jogador& obterJogador() const;
    const std::vector<Inimigo>& obterInimigos() const;
    std::size_t obterQuantidadeBombas() const;
    std::size_t obterQuantidadeExplosoes() const;
    /** @throws std::out_of_range Para indice inexistente, inclusive antes do inicio. */
    const Bomba& obterBomba(std::size_t indice) const;
    /** @throws std::out_of_range Para indice inexistente, inclusive antes do inicio. */
    const Explosao& obterExplosao(std::size_t indice) const;

private:
    struct ElementosFase;
    ConfiguracaoFase configuracao_;
    std::unique_ptr<ElementosFase> fase_;
    EstadoPartida estado_ = EstadoPartida::NaoIniciada;
    std::vector<EventoPartida> eventos_;

    void processarExplosoes();
    void resolverContatosEColeta();
    void verificarResultado();
};

} // namespace bomberman

#endif
