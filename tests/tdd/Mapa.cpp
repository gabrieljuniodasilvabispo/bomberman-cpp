// Apoio temporario da fase RED; substituido quando src/Mapa.cpp existir.
#include "pendente.hpp"
#include "Mapa.hpp"
Mapa::Mapa(int, int, const std::vector<Bloco>&, Posicao, const std::vector<Posicao>&, Posicao) { pendente("Nao implementado: Mapa::Mapa"); }
int Mapa::obterLinhas() const { pendente("Nao implementado: int Mapa::obterLinhas"); }
int Mapa::obterColunas() const { pendente("Nao implementado: int Mapa::obterColunas"); }
bool Mapa::dentroDosLimites(Posicao) const { pendente("Nao implementado: bool Mapa::dentroDosLimites"); }
bool Mapa::podeEntrar(Posicao) const { pendente("Nao implementado: bool Mapa::podeEntrar"); }
Bloco* Mapa::obterBloco(Posicao) { pendente("Nao implementado: Bloco* Mapa::obterBloco"); }
const Bloco* Mapa::obterBloco(Posicao) const { pendente("Nao implementado: const Bloco* Mapa::obterBloco"); }
void Mapa::liberarBlocoDestruido(Posicao) { pendente("Nao implementado: void Mapa::liberarBlocoDestruido"); }
bool Mapa::registrarBomba(Posicao) { pendente("Nao implementado: bool Mapa::registrarBomba"); }
void Mapa::removerBomba(Posicao) { pendente("Nao implementado: void Mapa::removerBomba"); }
bool Mapa::temBomba(Posicao) const { pendente("Nao implementado: bool Mapa::temBomba"); }
Posicao Mapa::obterInicioJogador() const { pendente("Nao implementado: Posicao Mapa::obterInicioJogador"); }
std::vector<Posicao> Mapa::obterIniciosInimigos() const { pendente("Nao implementado: std::vector<Posicao> Mapa::obterIniciosInimigos"); }
Posicao Mapa::obterSaida() const { pendente("Nao implementado: Posicao Mapa::obterSaida"); }
void Mapa::registrarMelhoria(Posicao, Melhoria&) { pendente("Nao implementado: void Mapa::registrarMelhoria"); }
Melhoria* Mapa::obterMelhoria(Posicao) const { pendente("Nao implementado: Melhoria* Mapa::obterMelhoria"); }
void Mapa::removerMelhoria(Posicao) { pendente("Nao implementado: void Mapa::removerMelhoria"); }
void Mapa::reiniciar() { pendente("Nao implementado: void Mapa::reiniciar"); }
