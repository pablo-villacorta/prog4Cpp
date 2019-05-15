#include "Repo.h"
#include <string.h>
#include <iostream>
#include "Usuario.h"
#include "Commit.h"
#include "Main.h"
#include "BD.h"

using namespace std;

Repo::Repo(string nombre, string desc, string ruta, Usuario *duenyo) {
    this->nombre = string(nombre);
    this->descripcion= string(desc);
    this->ruta = string(ruta);
    this->duenyo = duenyo;
    this->duenyo->addRepoPropio(this);
}

Repo::~Repo() {
    delete duenyo;
}

string Repo::getNombre() const {
    return this->nombre;
}

string Repo::getDescripcion() const {
    return this->descripcion;
}

string Repo::getRuta() const {
    return this->ruta;
}

Usuario* Repo::getDuenyo() const {
    return this->duenyo;
}

void Repo::addColaborador(Usuario *usuario) {
    this->colabs.push_back(usuario);
}

void Repo::addCommit(Commit *commit) {
    this->commits.push_back(commit);
}

vector<Usuario*>* Repo::getColabs() {
    return &colabs;
}

vector<Commit*>* Repo::getCommits() {
    return &commits;
}