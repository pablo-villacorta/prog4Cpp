//
// Created by pablo on 11/04/2019.
//

#ifndef PROG4CPP_USUARIO_H
#define PROG4CPP_USUARIO_H

#include <string.h>
#include <vector>
#include "Repo.h"

using namespace std;

class Repo;

class Usuario {
private:
    vector<Repo*> reposPropios;
    string nickname, nombre, apellido, contra;
public:
    Usuario(string nickname, string nombre, string apellido, string contra);
    ~Usuario();
    string getNickname() const;
    string getNombre() const;
    string getApellido() const;
    string getContra() const;
    void addRepoPropio(Repo *repo);
    bool equals(Usuario *u) const;

    void listarReposRelacionados() const;
};


#endif //PROG4CPP_USUARIO_H
