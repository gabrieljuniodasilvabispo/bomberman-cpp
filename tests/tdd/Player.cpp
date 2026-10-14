// Apoio temporario da fase RED; substituido quando src/Player.cpp existir.
#include "pendente.hpp"
#include "Player.hpp"
namespace bomberman {
Jogador::Jogador(Posicao) { pendente("Nao implementado: Jogador::Jogador"); }
Posicao Jogador::obterPosicao() const { pendente("Nao implementado: Posicao Jogador::obterPosicao"); }
bool Jogador::ativo() const { pendente("Nao implementado: bool Jogador::ativo"); }
bool Jogador::mover(Posicao, ::Mapa&) { pendente("Nao implementado: bool Jogador::mover"); }
int Jogador::obterAlcance() const { pendente("Nao implementado: int Jogador::obterAlcance"); }
int Jogador::obterLimiteBombas() const { pendente("Nao implementado: int Jogador::obterLimiteBombas"); }
bool Jogador::temEscudo() const { pendente("Nao implementado: bool Jogador::temEscudo"); }
void Jogador::receberExplosao() { pendente("Nao implementado: void Jogador::receberExplosao"); }
void Jogador::receberContatoInimigo() { pendente("Nao implementado: void Jogador::receberContatoInimigo"); }
}
