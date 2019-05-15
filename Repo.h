#ifndef PROG4CPP_REPO_H
#define PROG4CPP_REPO_H

#include <string>
#include "Usuario.h"
#include <vector>
#include "Commit.h"

using namespace std;

class Usuario;

class Repo {
private:
    string nombre, descripcion, ruta;
    Usuario *duenyo;
    vector<Usuario*> colabs;
    vector<Commit*> commits;
public:
    Repo(string nombre, string desc, string ruta, Usuario *duenyo); //para inicializar desde la bbdd
    ~Repo();
    string getNombre() const;
    string getDescripcion() const;
    string getRuta() const;
    Usuario* getDuenyo() const;
    void addColaborador(Usuario *usuario);
    vector<Usuario*>* getColabs();
    void addCommit(Commit *commit);
    vector<Commit*>* getCommits();

    void crearCarpetaCommits();
    //void getCarpetaRepo(char *b);
    string getCarpetaRepo();
    void commit(string descripcion);
};



//Repo* nuevoRepo();

#endif //PROG4CPP_REPO_H
