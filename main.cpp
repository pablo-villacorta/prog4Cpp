#include <iostream>
#include <string>
#include "BD.h"
#include "Usuario.h"
#include "main.h"
#include <vector>
#include "Repo.h"

using namespace bbdd;

namespace controlador {
    Usuario *usuarioActual;
    vector<Usuario*> usuarios;
    vector<Repo*> repos;
}

using namespace controlador;

int main() {
    cargarBD();

    return 0;
}

void copiarString(char *destino, const char *origen) {
    int len = strlen(origen)+1;
    for(int i = 0; i < len; i++) {
        destino[i] = origen[i];
    }
}
