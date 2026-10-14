// Apoio temporario da fase RED; substituido quando src/Partida.cpp existir.
#include "pendente.hpp"
#include "Partida.hpp"
namespace bomberman {
Partida::Partida() { pendente("Nao implementado: Partida::Partida"); }
void Partida::iniciarFase() { pendente("Nao implementado: void Partida::iniciarFase"); }
void Partida::atualizar(sf::Time) { pendente("Nao implementado: void Partida::atualizar"); }
bool Partida::todosInimigosEliminados() const { pendente("Nao implementado: bool Partida::todosInimigosEliminados"); }
void Partida::encerrarFase() { pendente("Nao implementado: void Partida::encerrarFase"); }
void Partida::reiniciarFase() { pendente("Nao implementado: void Partida::reiniciarFase"); }
}
