#include "Commit.h"
#include <string>
#include "Repo.h"
#include <sstream>


Commit::Commit(string desc, int fecha, Usuario *autor, Repo *repo) {
    this->descripcion = string(desc);
    this->fecha = fecha;
    this->autor = autor;
    this->repo = repo;

    string c = repo->getCarpetaRepo();
    std::stringstream ss;
    ss << c;
    ss << "/commit_" << repo->getCommits()->size();
    this->ruta = string(ss.str());
}

Commit::Commit(Commit *c, Repo *r) {
    this->descripcion = string(c->getDescripcion());
    this->fecha = c->getFecha();
    this->autor = c->getAutor();
    this->repo = r;

    string cc = repo->getCarpetaRepo();
    std::stringstream ss;
    ss << cc;
    ss << "/commit_" << repo->getCommits()->size();
    this->ruta = string(ss.str());
}

Commit::~Commit() {
    delete autor;
    delete repo;
}

string Commit::getDescripcion() const {
    return descripcion;
}

int Commit::getFecha() const {
    return fecha;
}

Usuario* Commit::getAutor() const {
    return autor;
}

Repo* Commit::getRepo() const {
    return repo;
}

string Commit::getRuta() const {
    return this->ruta;
}

