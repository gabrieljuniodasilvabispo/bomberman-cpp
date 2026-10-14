// Apoio temporario da fase RED; substituido quando src/Bomba.cpp existir.
#include "pendente.hpp"
#include "Bomba.hpp"
namespace bomberman {
Bomba::Bomba(sf::Vector2i, int, Jogador& jogador) : responsavel_(jogador) { pendente("Nao implementado: Bomba::Bomba"); }
sf::Vector2i Bomba::getPosicao() const { pendente("Nao implementado: sf::Vector2i Bomba::getPosicao"); }
int Bomba::getAlcance() const { pendente("Nao implementado: int Bomba::getAlcance"); }
bool Bomba::prontaParaExplodir() const { pendente("Nao implementado: bool Bomba::prontaParaExplodir"); }
void Bomba::atualizar(sf::Time) { pendente("Nao implementado: void Bomba::atualizar"); }
void Bomba::anteciparDetonacao() { pendente("Nao implementado: void Bomba::anteciparDetonacao"); }
void Bomba::detonar(Mapa&) { pendente("Nao implementado: void Bomba::detonar"); }
}
