#include "Usuario.h"
#include <string.h>
#include <iostream>
#include "Main.h"

Usuario::Usuario(string nickname, string nombre, string apellido, string contra) {
    this->nickname = string(nickname);
    this->nombre = string(nombre);
    this->apellido = string(apellido);
    this->contra = string(contra);
}

Usuario::~Usuario() {

}

string Usuario::getNickname() const {
    return nickname;
}

string Usuario::getNombre() const {
    return nombre;
}

string Usuario::getApellido() const {
    return apellido;
}

string Usuario::getContra() const {
    return contra;
}

void Usuario::addRepoPropio(Repo *repo) {
    reposPropios.push_back(repo);
}

bool Usuario::equals(Usuario *u) const {
    return this->nickname.compare(u->getNickname())== 0;
}

void Usuario::listarReposRelacionados() const {
    cout << "Repositorios en los que participas: " << endl;
    int c = 0;
    for(int i = 0; i < controlador::repos.size(); i++) {
        Repo *r = controlador::repos[i];
        if(this->equals(r->getDuenyo())) {
            c++;
            cout << r->getNombre() << " (de " << r->getDuenyo()->getNickname() << ")" << endl;
        } else {
            bool colab = false;
            for(int j = 0; j < r->getColabs()->size(); j++) {
                //comprobar que es colaborador del repo, y si lo es, imprimirlo y c++
                if(this->equals(r->getColabs()->at(j))) {
                    c++;
                    cout << r->getNombre() << " (de " << r->getDuenyo()->getNickname() << ")" << endl;
                    cout << r->getNombre() << " (de " << r->getDuenyo()->getNickname() << ")" << endl;
                }
            }
        }
    }
    if(c == 0) cout << "No hay repositorios que mostrar" << endl;
}


