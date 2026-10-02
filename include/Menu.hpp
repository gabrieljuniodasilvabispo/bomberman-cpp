#ifndef BOMBERMAN_MENU_HPP
#define BOMBERMAN_MENU_HPP

#include <SFML/System/Time.hpp>
#include <string>
#include <vector>

namespace bomberman {

/// @brief Opções possíveis a serem selecionadas pelo usuário no menu.
enum class OpcaoMenu {
    NovoJogo,
    Instrucoes,
    Sair,
    Nenhuma
};

/**
 * @class Menu
 * @brief Gerencia a navegação, seleção de opções e estado da interface do menu.
 */
class Menu {
public:
    /** @brief Constrói e inicializa as opções textuais do menu. */
    Menu();

    /** @brief Move a seleção do menu para a opção anterior. */
    void navegarAnterior();

    /** @brief Move a seleção do menu para a próxima opção. */
    void navegarProxima();

    /**
     * @brief Confirma a escolha atual do usuário.
     * @return A opção do menu selecionada no momento.
     */
    OpcaoMenu confirmarSelecao();

    /**
     * @brief Atualiza animações ou elementos temporais da interface do menu.
     * @param deltaTempo Tempo decorrido desde a última atualização.
     */
    void atualizar(sf::Time deltaTempo);

    /** @brief Indica se a tela de menu ainda está ativa. */
    bool ativo() const;

    /** @brief Retorna o índice inteiro da opção atualmente em foco. */
    int getIndiceSelecionado() const;

    /** @brief Retorna a lista de textos de todas as opções do menu. */
    const std::vector<std::string>& getOpcoes() const;

private:
    std::vector<std::string> opcoes_;
    int indiceSelecionado_;
    bool ativo_;
};

} // namespace bomberman

#endif // BOMBERMAN_MENU_HPP
