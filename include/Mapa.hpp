#ifndef BOMBERMAN_MAPA_HPP
#define BOMBERMAN_MAPA_HPP

#include <vector>
#include "Bloco.hpp"
#include "Posicao.hpp"


/** @file Mapa.hpp
 * @brief Contrato de Mapa, baseado no CRC02 e nas historias US01 a US06.
 */

/**
 * @brief Organiza o terreno e consulta a ocupacao das celulas da arena.
 * @details Mantem terreno, ocupacao por bombas e registros de melhorias.
 * Bombas bloqueiam entrada, mas seus objetos e
 * temporizadores pertencem a outras classes. Nao controla vitoria ou desenho.
 */
class Mapa {
public:
    /** @brief Limite de recursos para evitar alocacoes desproporcionais. */
    static constexpr int MAXIMO_CELULAS = 1000000;
    /**
     * @brief Define a configuracao inicial de uma fase.
     * @param linhas Quantidade positiva de linhas.
     * @param colunas Quantidade positiva de colunas.
     * @param blocos Blocos intactos da fase; o mapa deve armazenar copias.
     * @param inicioJogador Celula inicial livre do jogador.
     * @param iniciosInimigos Celulas iniciais livres e distintas dos personagens.
     * @param saida Celula livre da saida da fase.
     * @throws std::invalid_argument Se dimensoes nao forem positivas, houver
     * posicoes fora da grade, blocos sobrepostos ou destruidos, ou posicoes
     * iniciais sobrepostas entre personagens ou bloqueadas, ou saida bloqueada.
     * @throws std::length_error Se a arena exceder MAXIMO_CELULAS.
     * @note Partida define a disposicao segura das fases fixas. Mapa valida
     * posicoes livres e distintas, sem garantir uma rota de fuga em toda configuracao.
     */
    Mapa(int linhas, int colunas, const std::vector<Bloco>& blocos,
         Posicao inicioJogador, const std::vector<Posicao>& iniciosInimigos,
         Posicao saida);

    /** @brief Consulta a altura logica. @return Quantidade de linhas. */
    int obterLinhas() const;

    /** @brief Consulta a largura logica. @return Quantidade de colunas. */
    int obterColunas() const;

    /**
     * @brief Verifica os limites da grade.
     * @param posicao Celula consultada.
     * @return true se linha e coluna pertencem a arena.
     */
    bool dentroDosLimites(Posicao posicao) const;

    /**
     * @brief Verifica se o terreno e as bombas permitem entrar na celula.
     * @param posicao Destino do movimento.
     * @return false fora da arena ou havendo bloco intacto ou bomba.
     * @note Nao verifica contato entre personagens. Jogador consulta o destino,
     * permitindo sair da celula da propria bomba, mas nao voltar enquanto ocupada.
     */
    bool podeEntrar(Posicao posicao) const;

    /**
     * @brief Fornece um bloco para receber impactos.
     * @param posicao Celula consultada.
     * @return Ponteiro emprestado, ou nullptr fora da arena, sem bloco ou apos
     * sua destruicao. Valido ate reiniciar ou destruir o mapa; nao deve ser excluido.
     */
    Bloco* obterBloco(Posicao posicao);

    /**
     * @brief Consulta um bloco sem permitir sua modificacao.
     * @param posicao Celula consultada.
     * @return Ponteiro emprestado constante, com as mesmas regras da outra consulta.
     */
    const Bloco* obterBloco(Posicao posicao) const;

    /**
     * @brief Recebe a notificacao de destruicao de um bloco.
     * @param posicao Celula do bloco destruido.
     * @pre A posicao contem um bloco cujo estado ja e destruido.
     * @post O bloco deixa de bloquear a celula. Uma eventual bomba ainda bloqueia.
     * @note Preserva o objeto ate reiniciar ou destruir o mapa, permitindo que
     * receberExplosao termine com seguranca. Notificacoes repetidas nao tem efeito.
     */
    void liberarBlocoDestruido(Posicao posicao);

    /**
     * @brief Registra a ocupacao de uma bomba colocada por outra classe.
     * @param posicao Celula da bomba.
     * @return true ao registrar; false fora dos limites ou havendo bloco ou bomba.
     * @note Nao cria a bomba nem verifica o limite de bombas do jogador.
     */
    bool registrarBomba(Posicao posicao);

    /**
     * @brief Retira o bloqueio de uma bomba removida da partida.
     * @param posicao Celula anteriormente ocupada.
     * @note Sem efeito se nao houver bomba ou a posicao estiver fora da arena.
     */
    void removerBomba(Posicao posicao);

    /**
     * @brief Consulta a ocupacao por bomba.
     * @param posicao Celula consultada.
     * @return true se houver bomba; false inclusive fora dos limites.
     */
    bool temBomba(Posicao posicao) const;

    /** @brief Consulta o inicio do jogador. @return Celula inicial configurada. */
    Posicao obterInicioJogador() const;

    /** @brief Consulta os inicios dos inimigos. @return Copia das posicoes iniciais. */
    std::vector<Posicao> obterIniciosInimigos() const;

    /** @brief Consulta a saida. @return Celula usada pela Partida ao verificar vitoria. */
    Posicao obterSaida() const;

    /**
     * @brief Registra uma melhoria revelada por um bloco.
     * @param posicao Celula livre de bloco, igual a posicao da melhoria.
     * @param melhoria Objeto pertencente a Partida, que deve permanecer vivo
     * ate removerMelhoria, reiniciar ou destruir o mapa.
     * @pre Celula valida e sem outra melhoria registrada.
     * @throws std::invalid_argument Se a celula for invalida, estiver bloqueada,
     * diferir da posicao do item ou o item ja tiver sido coletado.
     * @throws std::logic_error Se outra melhoria ocupar a celula.
     * @note Repetir o registro do mesmo item na mesma celula nao tem efeito.
     * @note Nao aplica o efeito ao jogador nem transfere propriedade do objeto.
     */
    void registrarMelhoria(Posicao posicao, Melhoria& melhoria);

    /**
     * @brief Localiza uma melhoria revelada.
     * @param posicao Celula consultada.
     * @return Ponteiro emprestado ou nullptr se ausente ou fora dos limites.
     */
    Melhoria* obterMelhoria(Posicao posicao) const;

    /**
     * @brief Remove o registro de uma melhoria coletada ou destruida.
     * @param posicao Celula consultada.
     * @note Nao exclui o objeto. Sem efeito se ausente ou fora dos limites.
     */
    void removerMelhoria(Posicao posicao);

    /**
     * @brief Restaura o terreno e as posicoes da configuracao inicial.
     * @post Blocos intactos, nenhum registro de bomba ou melhoria revelada.
     * @note Invalida ponteiros de blocos. A Partida restaura personagens, bombas
     * e estados das melhorias; os objetos de melhorias referenciados na
     * configuracao inicial devem continuar vivos para reutilizacao.
     * @note Se a alocacao falhar, o estado anterior permanece intacto.
     */
    void reiniciar();

private:
    struct Celula {
        int indiceBloco = -1;
        bool temBomba = false;
        Melhoria* melhoria = nullptr;
    };

    int linhas_;
    int colunas_;
    Posicao inicioJogador_;
    std::vector<Posicao> iniciosInimigos_;
    Posicao saida_;

    // Configuracao original, preservada para reiniciar a fase.
    std::vector<Bloco> blocosIniciais_;
    std::vector<Bloco> blocos_;
    std::vector<std::vector<Celula>> celulas_;
};

#endif // BOMBERMAN_MAPA_HPP
