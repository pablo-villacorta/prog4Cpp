#include "Repo.h"
#include <string.h>
#include <iostream>
#include <sstream>
#include "Usuario.h"
#include "Commit.h"
#include "Main.h"
#include "BD.h"

extern "C" {
#include "manejo_archivos.h"
}

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

char* Repo::getCarpetaRepo() {
    //char *r = getDirectorioPadre(aChar(this->ruta));
    char *r = new char[ruta.size()+1];
    aChar(r, this->ruta);
    char * p = new char[getPosUltimaBarra(r)];
    getDirectorioPadre(p, r);
    strcat(p, "/repo");
    return p;
    //strcpy(b, p);
}

void Repo::crearCarpetaCommits() {
    crearCarpeta(getCarpetaRepo());
}

void Repo::commit() {
    std::stringstream ss;
    char *p = getCarpetaRepo();
    ss << p;
    cout << ss.str();
}