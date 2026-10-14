// Apoio temporario da fase RED; substituido quando src/Bloco.cpp existir.
#include "pendente.hpp"
#include "Bloco.hpp"
Bloco::Bloco(Posicao, TipoBloco, Melhoria*) { pendente("Nao implementado: Bloco::Bloco"); }
Posicao Bloco::obterPosicao() const { pendente("Nao implementado: Posicao Bloco::obterPosicao"); }
TipoBloco Bloco::obterTipo() const { pendente("Nao implementado: TipoBloco Bloco::obterTipo"); }
bool Bloco::estaDestruido() const { pendente("Nao implementado: bool Bloco::estaDestruido"); }
bool Bloco::impedePassagem() const { pendente("Nao implementado: bool Bloco::impedePassagem"); }
bool Bloco::receberExplosao(Mapa&) { pendente("Nao implementado: bool Bloco::receberExplosao"); }
