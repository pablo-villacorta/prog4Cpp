#ifndef PROG4CPP_MAIN_H
#define PROG4CPP_MAIN_H

#include "Usuario.h"
#include "Commit.h"
#include <vector>

namespace controlador {
    extern Usuario *usuarioActual;
    extern vector<Usuario*> usuarios;
    extern vector<Repo*> repos;
}

void menu();

int copiaCarpeta(char *directorioOrigen, char *directorioDestino, const int root);

//para que este metodo funcione primero hay que reservar
//strlen(origen)+1 elementos para destino
void copiarString(char *destino, const char *origen);

#endif //PROG4CPP_MAIN_H
