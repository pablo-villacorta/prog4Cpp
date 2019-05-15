#include "BD.h"
#include <iostream>
#include <sstream>
#include "main.h"
#include "Usuario.h"
#include "Commit.h"
#include <fstream>
#include <string>

#define T_USUARIO "usuario.csv"
#define T_REPO "repo.csv"
#define T_COLAB "colab.csv"
#define T_COMMIT "commit.csv"

namespace bbdd {
    void cargarUsuarios() {
        ifstream t(T_USUARIO);
        string line;
        if(t.is_open()) {
            while(getline(t, line)) {
                std::stringstream ss;
                ss << line;
                int i = 0;
                string nick,nombre,contra,apellido;
                while(ss.good()) {
                    string substr;
                    getline(ss, substr, ',');
                    if(i == 0) {
                        nick = string(substr);
                    } else if(i == 1) {
                        nombre = string(substr);
                    } else if(i == 2) {
                        apellido = string(substr);
                    } else {
                        contra = string(substr);
                    }
                    i++;
                }
                Usuario *u = new Usuario(nick, nombre, contra, apellido);
                controlador::usuarios.push_back(u);
            }
            t.close();
        }
    }

    void cargarRepos() {
        ifstream t(T_REPO);
        string line;
        if(t.is_open()) {
            while(getline(t, line)) {
                std::stringstream ss;
                ss << line;
                int i = 0;
                string nombre, desc, ruta, nick;
                while(ss.good()) {
                    string substr;
                    getline(ss, substr, ',');
                    if(i == 0) {
                        nombre = string(substr);
                    } else if(i == 1) {
                        desc = string(substr);
                    } else if(i == 2) {
                        ruta = string(substr);
                    } else {
                        nick = string(substr);
                    }
                    i++;
                }
                Usuario *u = getUsuario(nick);
                Repo *r = new Repo(nombre, desc, ruta, u);
                controlador::repos.push_back(r);
            }
            t.close();
        }
    }

    void cargarColaboradores() {
        ifstream t(T_COLAB);
        string line;
        if(t.is_open()) {
            while(getline(t, line)) {
                std::stringstream ss;
                ss << line;
                int i = 0;
                string nick, repo;
                while(ss.good()) {
                    string substr;
                    getline(ss, substr, ',');
                    if(i == 0) {
                        nick = string(substr);
                    } else {
                        repo = string(substr);
                    }
                    i++;
                }
                //cout << repo << endl;
                Usuario *u = getUsuario(nick);
                Repo *r = getRepo(repo);
                //r->addColaborador(u);
            }
            t.close();
        }
    }

    void cargarCommits() {
        ifstream t(T_COMMIT);
        string line;
        if(t.is_open()) {
            while(getline(t, line)) {
                std::stringstream ss;
                ss << line;
                int i = 0;
                string desc, nick, repo;
                int fecha;
                while(ss.good()) {
                    string substr;
                    getline(ss, substr, ',');
                    if(i == 0) {
                        desc = string(substr);
                    } else if(i == 1) {
                        fecha = stoi(string(substr));
                    } else if(i == 2) {
                        nick = string(substr);
                    } else {
                        repo = string(substr);
                    }
                    i++;
                }
                Usuario *u = getUsuario(nick);
                Repo *r = getRepo(repo);
                Commit *c = new Commit(desc, fecha, u, r);
                r->addCommit(c);
            }
            t.close();
        }
    }

    int iniciarSesion(string nick, string contra) {
        for(int i = 0; i < controlador::usuarios.size(); i++) {
            if(nick.compare(controlador::usuarios[i]->getNickname()) == 0) {
                if(contra.compare(controlador::usuarios[i]->getContra()) == 0) {
                    controlador::usuarioActual = controlador::usuarios[i];
                    return 0;
                }
            }
        }
        return -1;
    }

    Usuario* getUsuario(string nick) {
        Usuario *u;
        for(int i = 0; i < controlador::usuarios.size(); i++) {
            if(nick.compare(controlador::usuarios[i]->getNickname()) == 0) {
                u = controlador::usuarios[i];
                return u;
            }
        }
        return 0;
    }

    Repo* getRepo(string repo) {
        Repo *r;
        for(int i = 0; i < controlador::repos.size(); i++) {
            if(repo.compare(controlador::repos[i]->getNombre()) == 0) {
                r = controlador::repos[i];
                return r;
            }
        }
        return 0;
    }

    int registrarUsuario(string nick, string nombre, string apellido, string contra) {
        //TODO comprobar que no existe nickname
        ofstream t;
        t.open(T_USUARIO, ios::app);
        t << nick << "," << nombre << "," << apellido << "," << contra << "\n";
        t.close();
        return 0;
    }

    int registrarRepositorio(string nombre, string desc, string ruta, Usuario *duenyo) {
        //TODO comprobar que no existe mismo nombre
        ofstream t;
        t.open(T_REPO, ios::app);
        t << nombre << "," << desc << "," << ruta << "," << duenyo->getNickname() << "\n";
        t.close();
        return 0;
    }

    int registrarCommit(string desc, int fecha, Usuario *autor, Repo *repo) {
        ofstream t;
        t.open(T_COMMIT, ios::app);
        t << desc << "," << fecha << "," << autor->getNickname() << "," << repo->getNombre() << "\n";
        t.close();
        return 0;
    }

    int registrarColaborador(Usuario *autor, Repo *repo) {
        ofstream t;
        t.open(T_COLAB, ios::app);
        t << autor->getNickname() << "," << repo->getNombre() << "\n";
        t.close();
        return 0;
    }

    bool existeRepo(string nombre) {
        for(int i = 0; i < controlador::repos.size(); i++) {
            if(nombre.compare(controlador::repos[i]->getNombre()) == 0) {
                return true;
            }
        }
        return false;
    }

    void cargarBD() {
        cargarUsuarios();
        cargarRepos();
        cargarColaboradores();
        cargarCommits();
    }
}
