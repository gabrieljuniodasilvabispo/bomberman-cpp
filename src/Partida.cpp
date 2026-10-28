#include "Partida.hpp"

namespace bomberman {

Partida::Partida() 
    : partidaEmAndamento_(false) 
{
}

void Partida::iniciarFase() {
    partidaEmAndamento_ = true;
}

void Partida::atualizar(sf::Time deltaTempo) {
    if (!partidaEmAndamento_ || deltaTempo <= sf::Time::Zero) {
        return;
    }
}

bool Partida::todosInimigosEliminados() const {
    return false; 
}

void Partida::encerrarFase() {
    partidaEmAndamento_ = false;
}

void Partida::reiniciarFase() {
    encerrarFase();
    iniciarFase();
}

}
