#ifndef BOMBERMAN_PARTIDA_HPP
#define BOMBERMAN_PARTIDA_HPP

#include <SFML/System/Time.hpp>

namespace bomberman {

class Mapa;
class Jogador;
class Inimigo;
class Bomba;
class Menu;

/**
 * @class Partida
 * @brief Controla o estado atual da partida, gerenciando as fases, elementos ativos e ciclos do jogo.
 */
class Partida {
public:
    /**
     * @brief Cria o gerenciador da partida.
     */
    Partida();

    /**
     * @brief Inicia uma nova fase, criando o jogador e os inimigos nas posições iniciais.
     */
    void iniciarFase();

    /**
     * @brief Atualiza os elementos ativos a cada ciclo do jogo.
     * @param deltaTempo Tempo decorrido desde a última atualização.
     */
    void atualizar(sf::Time deltaTempo);

    /**
     * @brief Verifica se todos os inimigos foram eliminados.
     * @return true se não houver inimigos ativos, false caso contrário.
     */
    bool todosInimigosEliminados() const;

    /**
     * @brief Encerra a fase quando ocorrer uma condição de vitória ou derrota.
     */
    void encerrarFase();

    /**
     * @brief Permitir que o jogador reinicie a fase, restaurando os elementos para seus estados iniciais.
     */
    void reiniciarFase();

private:
    bool partidaEmAndamento_ = false;
    
    // Ponteiros/referências para as colaborações listadas no CRC01
    // Mapa* mapa_;
    // Jogador* jogador_;
    // Menu* menu_;
};

} // namespace bomberman

#endif
