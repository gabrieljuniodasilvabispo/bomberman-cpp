// Apoio temporario da fase RED; substituido quando src/Melhoria.cpp existir.
#include "pendente.hpp"
#include "Melhoria.hpp"
namespace bomberman {
Melhoria::Melhoria(sf::Vector2i, TipoMelhoria) { pendente("Nao implementado: Melhoria::Melhoria"); }
void Melhoria::aplicarEm(Jogador&) { pendente("Nao implementado: void Melhoria::aplicarEm"); }
bool Melhoria::coletada() const { pendente("Nao implementado: bool Melhoria::coletada"); }
void Melhoria::atualizar(sf::Time) { pendente("Nao implementado: void Melhoria::atualizar"); }
sf::Vector2i Melhoria::getPosicao() const { pendente("Nao implementado: sf::Vector2i Melhoria::getPosicao"); }
TipoMelhoria Melhoria::getTipo() const { pendente("Nao implementado: TipoMelhoria Melhoria::getTipo"); }
}
