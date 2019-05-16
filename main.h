#ifndef PROG4CPP_MAIN_H
#define PROG4CPP_MAIN_H

#include "Usuario.h"
#include "Commit.h"
#include <vector>

namespace controlador {
    extern Usuario *usuarioActual;
    extern vector<Usuario*> usuarios;
    extern vector<Repo*> repos;
    extern Repo *repoActual;
}

void menu();

int copiaCarpeta(char *directorioOrigen, char *directorioDestino, const int root);

//para que este metodo funcione primero hay que reservar
//strlen(origen)+1 elementos para destino
void aChar(char *dest, string origen);
int getPosUltimaBarra(char *ruta);
void getDirectorioPadre(char *dest, char *ruta);

void crearNuevoRepo(string nombre, string descripcion, string ruta);

#endif //PROG4CPP_MAIN_H
