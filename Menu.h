#ifndef PROG4CPP_MENU_H
#define PROG4CPP_MENU_H

#include <vector>

namespace menu_ {
    void inicio();

    bool inicioSesion();

    bool registrar();

    void gestionRepos();

    void nuevoRepo();

    void hacerCommit();

    void crearRepoDuplicado();

    void listarColaborador();

    void commitsUsuario();

    void menuMisRepositorios();

    void misRepositorios();

    void historialVersiones();

    void estadisticas();

    void opciones();

    void addColaborador();

    void menuHistorialVersiones();

    void statEvolucionRepo(); //NOUVEAU

    void statEvolucionArchivo(); //NOUVEAU

    void statExtensiones(); //NOUVEAU

    void listarArchivos(std::vector<char*> *a, char *path); //NOUVEAU
}

#endif //PROG4CPP_MENU_H
