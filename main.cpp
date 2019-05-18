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

extern "C"{
    #include "manejo_archivos.h"
    #include "graficos.h"
}

using namespace bbdd;

namespace controlador {
    Usuario *usuarioActual;
    vector<Usuario*> usuarios;
    vector<Repo*> repos;
    Repo *repoActual;
    Commit *commitActual;
}

using namespace controlador;

int main() {
    cargarBD();
    cout << "Bienvenido/a" << endl;
    cout << "Para el correcto funcionamiento del programa, no se pueden incluir espacios ni comas en los nombres de carpetas, " << endl;
    cout << "archivos y cualquier tipo de informacion que se recoja por teclado en el programa." << endl;
    cout << "Para introducir las rutas, utiliza el caracter '/' para separar los directorios." << endl;
    cout << endl;
    menu_::inicio();
    liberarMemoria();
    return 0;
}

void liberarMemoria() {
    for(int i = 0; i < repos.size(); i++) {
        Repo *r = repos[i];
        for(int j = 0; j < r->getCommits()->size(); j++) {
            delete r->getCommits()->at(j);
        }
        delete r;
    }

    for(int i = 0; i < usuarios.size(); i++) {
        delete usuarios[i];
    }
}

Repo* crearNuevoRepo(string nombre, string descripcion, string ruta) {
    Repo *r = new Repo(nombre, descripcion, ruta, usuarioActual);
    repos.push_back(r);
    usuarioActual->addRepoPropio(r);
    r->crearCarpetaCommits();
    bbdd::registrarRepositorio(nombre, descripcion, ruta, usuarioActual);
    return r;
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

            strcpy(pathDest, directorioDestino);
            strcat(pathDest, "/");
            strcat(pathDest, dp->d_name);
            DIR *dir2 = opendir(path);

            if (!dir2) {
                copiarArchivo(path, pathDest);
            } else {
                crearCarpeta(pathDest);
                copiaCarpeta(path, pathDest, root + 1);
            }
        }
    }
    closedir(dir);

    return 0;
}
