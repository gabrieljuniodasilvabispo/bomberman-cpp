// Apoio temporario da fase RED; substituido quando src/Menu.cpp existir.
#include "pendente.hpp"
#include "Menu.hpp"
namespace bomberman {
Menu::Menu() { pendente("Nao implementado: Menu::Menu"); }
void Menu::navegarAnterior() { pendente("Nao implementado: void Menu::navegarAnterior"); }
void Menu::navegarProxima() { pendente("Nao implementado: void Menu::navegarProxima"); }
OpcaoMenu Menu::confirmarSelecao() { pendente("Nao implementado: OpcaoMenu Menu::confirmarSelecao"); }
void Menu::atualizar(sf::Time) { pendente("Nao implementado: void Menu::atualizar"); }
bool Menu::ativo() const { pendente("Nao implementado: bool Menu::ativo"); }
int Menu::getIndiceSelecionado() const { pendente("Nao implementado: int Menu::getIndiceSelecionado"); }
const std::vector<std::string>& Menu::getOpcoes() const { pendente("Nao implementado: const std::vector<std::string>& Menu::getOpcoes"); }
}
