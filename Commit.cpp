#include "Commit.h"
#include <string>



Commit::Commit(string desc, int fecha, Usuario *autor, Repo *repo) {
    this->descripcion = string(desc);
    this->fecha = fecha;
    this->autor = autor;
    this->repo = repo;
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

