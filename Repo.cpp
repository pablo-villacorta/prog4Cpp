#include "Repo.h"
#include <string.h>
#include <iostream>
#include <sstream>
#include "Usuario.h"
#include "Commit.h"
#include "Main.h"
#include "BD.h"
#include <ctime>

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

string Repo::getCarpetaRepo() {
    char *r = new char[ruta.size()+1];
    aChar(r, ruta);
    int p = getPosUltimaBarra(r);
    string s = ruta.substr(0, p);
    string n("/repo");
    s.append(n);
    return s;
}

void Repo::crearCarpetaCommits() {
    string a = getCarpetaRepo();
    char *b = new char[a.size()+1];
    aChar(b, a);
    crearCarpeta(b);
    delete[] b;
}

void Repo::commit(string descripcion) {
    string s = getCarpetaRepo();
    std::stringstream ss;
    ss << s;
    ss << "/commit_";
    ss << commits.size();
    string r = string(ss.str());
    char *a = new char[r.size()+1];
    aChar(a, r);
    crearCarpeta(a);
    char *b = new char[ruta.size()+1];
    aChar(b, ruta);
    copiaCarpeta(b,a,0);

    time_t t;
    t = time(NULL);
    bbdd::registrarCommit(descripcion, t, controlador::usuarioActual, this);
    Commit *c = new Commit(descripcion, t, controlador::usuarioActual, this);
    this->addCommit(c);

    delete[] a;
    delete[] b;
}