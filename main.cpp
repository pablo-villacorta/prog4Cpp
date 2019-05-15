#include <iostream>
#include <string>
#include "BD.h"
#include "Usuario.h"
#include "main.h"
#include <vector>
#include "Repo.h"
#include <string.h>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include "Menu.h"

#define URL "C:/Users/pablo/Desktop/prog4_bd/hola"

extern "C"{
    #include "manejo_archivos.h"
    #include "graficos.h"
}

using namespace bbdd;

namespace controlador {
    Usuario *usuarioActual;
    vector<Usuario*> usuarios;
    vector<Repo*> repos;
}

using namespace controlador;

int main() {
    Usuario *u = new Usuario("juan", "dos", "santos", "a");
    Repo *r = new Repo("miRepo", "descripcion", URL, u);
    r->commit();



    return 0;
}




void aChar(char *dest, string origen) {
    int len = origen.size();
    //char *d = new char[len+1];
    for(int i = 0; i < len; i++) {
        dest[i] = origen[i];
    }
    dest[len] = '\0';
}

int getPosUltimaBarra(char *ruta) {
    int pos = 0;
    for(int i = strlen(ruta); i >= 0; i--) {
        if(ruta[i] == '/') {
            pos = i;
            break;
        }
    }
    return pos;
}

void getDirectorioPadre(char *dest, char *ruta) {
    int pos = getPosUltimaBarra(ruta);
    for(int i = 0; i < pos; i++) {
        dest[i] = ruta[i];
    }
    dest[pos] = '\0';
}

int copiaCarpeta(char *directorioOrigen, char *directorioDestino, const int root) {
    long int tamanyo = 0;
    int i;
    char path[1000];
    char pathDest[1000];
    struct dirent *dp;
    DIR *dir = opendir(directorioOrigen);

    if (!dir) {
        return -1; //error
    }

    while ((dp = readdir(dir)) != NULL) {

        if (strcmp(dp->d_name, ".") != 0 && strcmp(dp->d_name, "..") != 0) {

            strcpy(path, directorioOrigen);
            strcat(path, "/");
            strcat(path, dp->d_name);

            for (int i = 0; i < 1000; i++) {
                pathDest[i] = NULL;
            }

            //strcat(pathDest, directorioOrigen);
            //strcat(pathDest, "_commit");
            //crearCarpeta(pathDest);
            strcpy(pathDest, directorioDestino);
            strcat(pathDest, "/");
            strcat(pathDest, dp->d_name);
            DIR *dir2 = opendir(path);

            if (!dir2) {
                copiarArchivo(path, pathDest);
            } else {

                cout << path << endl;
                cout << pathDest << endl;
                crearCarpeta(pathDest);
                copiaCarpeta(path, pathDest, root + 1);
            }
        }
    }
    closedir(dir);

    return 0;
}
