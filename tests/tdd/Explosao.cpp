// Apoio temporario da fase RED; substituido quando src/Explosao.cpp existir.
#include "pendente.hpp"
#include "Explosao.hpp"
namespace bomberman {
Explosao::Explosao(sf::Vector2i, int, Mapa& mapa) : mapa_(mapa) { pendente("Nao implementado: Explosao::Explosao"); }
void Explosao::propagar() { pendente("Nao implementado: void Explosao::propagar"); }
bool Explosao::ativa() const { pendente("Nao implementado: bool Explosao::ativa"); }
void Explosao::atualizar(sf::Time) { pendente("Nao implementado: void Explosao::atualizar"); }
const std::vector<sf::Vector2i>& Explosao::getCelulasAtingidas() const { pendente("Nao implementado: const std::vector<sf::Vector2i>& Explosao::getCelulasAtingidas"); }
void Explosao::aplicarEm(Jogador&) { pendente("Nao implementado: void Explosao::aplicarEm"); }
void Explosao::aplicarEm(Inimigo&) { pendente("Nao implementado: void Explosao::aplicarEm"); }
}
