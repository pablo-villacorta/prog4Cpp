#ifndef PROG4CPP_BD_H
#define PROG4CPP_BD_H

#include <string>
#include "Usuario.h"
#include "Repo.h"

namespace bbdd {
    void cargarBD();

    int registrarUsuario(string nick, string nombre, string apellido, string contra); //ok
    //int existeUsuario(char *nick); //ok
    int iniciarSesion(string nick, string contra); //ok

    void cargarUsuarios(); //ok
    Usuario* getUsuario(string nick);

    int registrarRepositorio(string nombre, string desc, string ruta, Usuario *duenyo); //ok
    /*int existeRepositorio(char *nombre); //ok */
    void cargarRepos(); //ok
    Repo* getRepo(string repo);

    int registrarCommit(string desc, int fecha, Usuario *autor, Repo *repo); //ok
    void cargarCommits(); //ok

    int registrarColaborador(Usuario *autor, Repo *repo); //ok
    void cargarColaboradores(); //ok

    bool existeRepo(string nombre);
}

#endif //PROG4CPP_BD_H
