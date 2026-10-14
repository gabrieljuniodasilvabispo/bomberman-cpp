#include "apoio.hpp"
struct TestesPosicao {};
TEST_CASE_FIXTURE(TestesPosicao, "Posicao preserva linha e coluna independentes") {
    Posicao original{2, 7};
    Posicao copia = original;
    copia.linha = 4;
    conferirPosicao(original, {2, 7});
    conferirPosicao(copia, {4, 7});
}
