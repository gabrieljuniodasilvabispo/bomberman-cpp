// Apoio temporario da fase RED; substituido quando src/Enemy.cpp existir.
#include "pendente.hpp"
#include "Enemy.hpp"
namespace bomberman {
Inimigo::Inimigo(Posicao) { pendente("Nao implementado: Inimigo::Inimigo"); }
Posicao Inimigo::obterPosicao() const { pendente("Nao implementado: Posicao Inimigo::obterPosicao"); }
bool Inimigo::ativo() const { pendente("Nao implementado: bool Inimigo::ativo"); }
void Inimigo::atualizar(sf::Time, ::Mapa&) { pendente("Nao implementado: void Inimigo::atualizar"); }
void Inimigo::receberExplosao() { pendente("Nao implementado: void Inimigo::receberExplosao"); }
void Inimigo::verificarContato(Jogador&) { pendente("Nao implementado: void Inimigo::verificarContato"); }
}
