//
// Created by pablo on 14/05/2019.
//

#ifndef PROG4CPP_COMMIT_H
#define PROG4CPP_COMMIT_H

#include <string>

class Usuario;
class Repo;

using namespace std;

class Commit {
private:
    int fecha;
    string descripcion;
    Usuario *autor;
    Repo *repo;
public:
    Commit(string desc, int fecha, Usuario *autor, Repo *repo);
    ~Commit();
    int getFecha() const;
    string getDescripcion() const;
    Usuario* getAutor() const;
    Repo* getRepo() const;
};


#endif //PROG4CPP_COMMIT_H
