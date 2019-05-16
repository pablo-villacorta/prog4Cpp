#include <iostream>
#include "Menu.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include "BD.h"
#include "main.h"
#include <sstream>

extern "C"{
#include "manejo_archivos.h"
#include "graficos.h"
}

using namespace std;

namespace menu_ {
/*
 * Devuelve true si el inicio de sesión está bien hecho
 * */
    bool inicioSesion() {
        string nom, con;
        cout << "Nombre de usuario: " << endl; //Conseguir nombre de usuario y comprobar en la BD
        //getline(cin, nom);
        cin >> nom;
        cout << "Contrasenya: " << endl; //Conseguir contraseña y comprobar en la BD
        //getline(cin, con);
        cin >> con;

        if( bbdd::iniciarSesion(nom, con) == 0){
            Usuario *u = bbdd::getUsuario(nom);
            controlador::usuarioActual = u;
            return true;
        } else {
            cout << "Los datos introducido no son correctos" << endl;
            return false;
        }
    }

    bool registrar() {
        //string nick, string nombre, string apellido, string contra
        string nomUsuario, nombre, apellido, contra;
        cout << "Introduzca su nombre: " << endl;
        cin >> nombre;
        cout << "Introduzca su apellido: " << endl;
        cin >> apellido;
        cout << "Introduzca su nombre de usuario: " << endl;
        cin >> nomUsuario;
        cout << "Introduzca su contrasenya: " << endl;
        cin >> contra;

        //cout << bbdd::registrarUsuario(nomUsuario, nombre, apellido, contra) << endl;
        if(bbdd::registrarUsuario(nomUsuario, nombre, apellido, contra) == 0){
            Usuario *u = new Usuario(nomUsuario, nombre, apellido, contra);
            controlador::usuarios.push_back(u);
            return true;
        } else {
            cout << "Nombre de usuario no valido" << endl;
            return false;
        }
    }

    void nuevoRepo(){
        string nombre, descripcion, ruta;
        cout << "Introduzca el nombre del repositorio: " << endl;
        cin >> nombre;
        if(bbdd::existeRepo(nombre)) {
            cout << "Error, ya existe un repositorio con el mismo nombre" << endl;
            gestionRepos();
        } else {
            cout << "Introduzca la descripcion del repositorio: " << endl;
            cin >> descripcion;
            cout << "Introduzca la ruta: " << endl;
            cin >> ruta;
            crearNuevoRepo(nombre, descripcion, ruta);
            cout << "Repositorio creado" << endl;
            gestionRepos();
        }
    }

    void gestionRepos() {
        char c;
        do {
            cin.clear();
            cout << "Selecciona una de las siguientes opciones: " << endl;
            cout << "1. Mis repositorios " << endl;
            cout << "2. Crear nuevo repositorio " << endl;
            cout << "3. Cerrar sesion " << endl;
            cout << "0. Salir " << endl;
            cin.get();
            cin.get(c);
        } while (c != '1' && c != '2' && c != '3' && c != '0');

        if (c == '1') {
            //Mis repositorios: muestra una lista de todos los repositorios en los que el usuario sea dueño/colaborador y tras elegir uno se ofrecerán las siguientes opciones
            //para dicho repositorio
            misRepositorios();
        } else if (c == '2') {
            //Crear nuevo repositorio: el usuario introducirá el nombre del repositorio y éste se añadirá a su lista de repositorios
            menu_::nuevoRepo();
        } else if (c == '3') {
            //Cierra sesión y vuelve al menú principal
            inicio();
        } else {
            //Salir
            return;
        }
    }

    void estadisticas();

    void opciones();

    void misRepositorios() {
        vector<Repo*> rep;
        string n = controlador::usuarioActual->getNickname();
        for(int i = 0; i < controlador::repos.size(); i++) {
            Repo *r = controlador::repos[i];
            if(r->getDuenyo()->getNickname().compare(n) == 0 || r->esColaborador(n)) {
                rep.push_back(r);
            }
        }

        int opcion = 0;
        string res;
        do {
            cin.clear();
            cout << "Elige un repositorio (introduce el numero correspondiente):" << endl;
            for(int i = 0; i < rep.size(); i++) {
                cout << i << ": " << rep[i]->getNombre() << " (de " << rep[i]->getDuenyo()->getNickname() << ")" << endl;
            }
            cout << rep.size() << ": Atras" << endl;
            cin >> res;
            opcion = stoi(res);
        } while(opcion < 0 || opcion >= rep.size() +1 );
        if(opcion == rep.size()) {
            gestionRepos();
        } else {
            controlador::repoActual = rep[opcion];
            menuMisRepositorios();
        }
    }

    void hacerCommit(){
        string desc;

        cout << "Introduce una descripcion: " << endl;
        cin >> desc;

        controlador::repoActual->commit(desc);
    }

    void crearRepoDuplicado(){
        string nom, ruta;

        cout << "Introduce el nombre con el que se quieres que guarde el duplicado: " << endl;
        cin >> nom;
        cout << "Introduce la ruta donde quieres que se guarde el repositorio duplicado: " << endl;
        cin >> ruta;

        string fin;
        char *r = new char[controlador::repoActual->getRuta().size()+1];
        aChar(r, controlador::repoActual->getRuta());
        int p = getPosUltimaBarra(r);
        fin = controlador::repoActual->getRuta().substr(p);

        ruta.append(fin);
        char *a = new char[ruta.size()+1];
        aChar(a, ruta);
        crearCarpeta(a);
        copiaCarpeta(r, a, 0);

        Repo *n = crearNuevoRepo(nom, controlador::repoActual->getDescripcion(), ruta);
        //copiar en BD+objetos colaboradores y commits
        //copiar colaboradores
        for(int i = 0; i < controlador::repoActual->getColabs()->size(); i++) {
            Usuario *u = controlador::repoActual->getColabs()->at(i);
            n->addColaborador(u);
            bbdd::registrarColaborador(u, n);
        }
        //copiar registro de commits
        for(int i = 0; i < controlador::repoActual->getCommits()->size(); i++) {
            Commit *c = new Commit(controlador::repoActual->getCommits()->at(i), n);
            n->addCommit(c);
            bbdd::registrarCommit(c->getDescripcion(), c->getFecha(), c->getAutor(), c->getRepo());
        }

        //copiar carpeta repo
        string repoOriginal, repoDestino;
        repoOriginal = controlador::repoActual->getCarpetaRepo();
        repoDestino = n->getCarpetaRepo();
        char *rO, *rD;
        rO = new char[repoOriginal.size()+1];
        rD = new char[repoDestino.size()+1];
        aChar(rO, repoOriginal);
        aChar(rD, repoDestino);
        copiaCarpeta(rO, rD, 0);
    }

    void listarColaborador(){
        vector<Usuario*> *colab = controlador::repoActual->getColabs();
        for(int i = 0; i < controlador::repoActual->getColabs()->size(); i++){
            cout << i+1 << ". ";
            cout << ((controlador::repoActual->getColabs())->at(i))->getNickname() << endl;
        }
    }

    void menuMisRepositorios() {
        char c;
        do {
            cin.clear();
            cout << "Selecciona una de las siguientes opciones: " << endl;
            cout << "1. Historial de versiones " << endl;
            cout << "2. Hacer commit " << endl;
            cout << "3. Duplicar " << endl;
            cout << "4. Listar colaboradores " << endl;
            cout << "5. Anyadir colaborador " << endl;
            cout << "6. Estadisticas " << endl;
            cout << "7. Esquema de archivos " << endl;
            cout << "8. Opciones " << endl;
            cout << "9. Atras " << endl;
            cout << "0. Salir" << endl;
            cin.get();
            cin.get(c);
        } while (c != '1' && c != '2' && c != '3' && c != '4' && c != '5' && c != '6' && c != '7' && c != '8' &&
                 c != '9' && c != '0');

        if (c == '1') {
            //Historial de versiones, muestra lista de todas las versiones del repositorio, mostrando la fecha y el autor del commit, el usuario
            //podrá seleccionar un commit para verlo con mayor nivel de detalle
            historialVersiones();
            menuMisRepositorios();
        } else if (c == '2') {
            //Hacer commit: el sistema pedirá al usuario que seleccione qué archivos quiere incluir en el commit y realizará el commit
            hacerCommit();
            menuMisRepositorios();
        } else if (c == '3') {
            //Duplicar: el sistema creará una copia del repositorio en el sistema de archivos del usuario, pidiendo antes al usuario que introduzca un nombre
            //que no esté en uso para dicho repositorio
            crearRepoDuplicado();
            menuMisRepositorios();
        } else if (c == '4') {
            //Listar colaborador: muestra una lista con los diferentes colaboradores del repositorio
            listarColaborador();
            menuMisRepositorios();
        } else if (c == '5') {
            //Añadir colaborador: se pide el nombre de un usuario y se añadirá a la lista de los colaboradores de dicho repositorio siempre y cuando
            //exista dicho usuario
            addColaborador(); //NOUVEAU
            menuMisRepositorios(); //NOUVEAU
        } else if (c == '6') {
            //Estadísticas: distintos datos y gráficos que permiten analizar la evolución y el estado actual del repositorio
            estadisticas();
        } else if (c == '7') {
            //Esquema de archivos: se mostrará un esquema de los archivos que componen el repositorio, en forma de árbol, de manera legible
            //para el usuario
            //NOUVEAU
            Repo *r = controlador::repoActual;
            char *rut = new char[r->getRuta().size()+1];
            aChar(rut, r->getRuta());
            mostrarArbolDirectorios(rut, 0);
            menuMisRepositorios();
        } else if (c == '8') {
            //Opciones
            opciones();
        } else if (c == '9') {
            //Atrás
            gestionRepos();
        } else {
            //Salir
            return;
        }
    }

    void historialVersiones() {
        Repo *r = controlador::repoActual;
        cout << "Historial de versiones del repositorio " << r->getNombre() << endl;
        for(int i = 0; i < r->getCommits()->size(); i++) {
            Commit *c = r->getCommits()->at(i);
            cout << i << ". " << c->getDescripcion();
            cout << " (de " << c->getAutor()->getNickname() << ")" << endl;
        }
        cout << r->getCommits()->size() << ". Atras" << endl;
        int op;
        do {
            cout << "Elige una opcion: " << endl;
            string res;
            cin >> res;
            op = stoi(res);
        } while(op < 0 || op > r->getCommits()->size());
        if(op < r->getCommits()->size()) {
            controlador::commitActual = r->getCommits()->at(op);
            menuHistorialVersiones();
        }
    }

    void menuHistorialVersiones() {
        char c;
        do {
            cin.clear();
            cout << "Selecciona una de las siguientes opciones: " << endl;
            cout << "1. Esquema de archivos " << endl;
            cout << "2. Restaurar a esta versión " << endl;
            cout << "3. Atrás " << endl;
            cout << "0. Salir" << endl;
            cin.get();
            cin.get(c);
        } while (c != '1' && c != '2' && c != '3' && c != '0');

        if (c == '1') {
            //Esquema de archivos: muestra un esquema en forma de árbol del workspace en el momento de realizar el commit
            string ru = controlador::commitActual->getRuta();
            char *r = new char[ru.size()+1];
            aChar(r, ru);
            mostrarArbolDirectorios(r, 0);
            delete[] r;
            menuHistorialVersiones();
        } else if (c == '2') {
            //Restaurar a esta versión: realiza un nuevo commit en el que se vuelve a la versión seleccionada
            char *rr, *rc; //ruta repo, ruta commit
            string rutaRepo = controlador::repoActual->getRuta();
            rr = new char[rutaRepo.size()+1];
            aChar(rr, rutaRepo);
            borrarCarpeta(rr, 0);
            crearCarpeta(rr);
            string rutaCommit = controlador::commitActual->getRuta();
            rc = new char[rutaCommit.size()+1];
            aChar(rc, rutaCommit);
            copiaCarpeta(rc,rr,0);
            menuHistorialVersiones();
        } else if (c == '3') {
            //Atrás
            menuMisRepositorios();
        } else {
            //Salir
            return;
        }
    }

    void addColaborador() {
        cout << "Introduce el nombre de usuario del usuario que quieres hacer colaborador:"  << endl;
        string nick;
        cin >> nick;
        if(bbdd::existeUsuario(nick)) {
            if(controlador::repoActual->getDuenyo()->getNickname().compare(nick) == 0) {
                cout << "El usuario seleccionado es el duenyo del repositorio" << endl;
            } else if(controlador::repoActual->esColaborador(nick)) {
                cout << "El usuario seleccionado ya es colaborador en el repositorio" << endl;
            } else {
                Usuario *u = bbdd::getUsuario(nick);
                controlador::repoActual->addColaborador(u);
                bbdd::registrarColaborador(u, controlador::repoActual);
            }
        } else {
            cout << "Error, el nombre de usuario introducido no coincide con el de ningún usuario" << endl;
            cout << "Elige una opcion: " << endl;
            cout << "1. Volver a intentarlo" << endl;
            cout << "2. Atras" << endl;
            string res;
            cin >> res;
            if(res.compare("1") == 0) {
                addColaborador();
            }
        }
    }

    void estadisticas() {
        char c;
        do {
            cin.clear();
            cout << "Selecciona una de las siguientes opciones: " << endl;
            cout << "1. Commits/usuario" << endl;
            cout << "2. Evolución de un archivo" << endl;
            cout << "3. Visualizacion de commits a lo largo del tiempo" << endl;
            cout << "4. Tamaño actual del proyecto" << endl;
            cout << "5. Extensiones de los archivos" << endl;
            cout << "6. Atras" << endl;
            cout << "0. Salir" << endl;
            cin.get();
            cin.get(c);
        } while (c != '1' && c != '2' && c != '3' && c != '4' && c != '5' && c != '6' && c != '0');

        if (c == '1') {
            //Commits/usuario: gráfico de barras que muestra la cantidad de commits realizados por cada usuario
            estadisticas();
        } else if (c == '2') {
            //Evolución de un archivo: gráfico que muestra la evolución del tamaño de un archivo a lo largo de las diferentes versiones
            char **m = (char **) malloc(10 * sizeof(char *));
            for (int i = 0; i < 5; i++) {
                m[i] = (char *) malloc(10 * sizeof(char));
            }

            m[0] = "/Users/alvaro/eclipse-workspace2/proyectoMenu/prueba/ver1.txt";
            m[1] = "/Users/alvaro/eclipse-workspace2/proyectoMenu/prueba/ver2.txt";
            m[2] = "/Users/alvaro/eclipse-workspace2/proyectoMenu/prueba/ver3.txt";
            m[3] = "/Users/alvaro/eclipse-workspace2/proyectoMenu/prueba/ver4.txt";
            m[4] = "/Users/alvaro/eclipse-workspace2/proyectoMenu/prueba/ver5.txt";

            graficoTamanyoArchivos(m, 5, "Ver.txt");
            estadisticas();
        } else if (c == '3') {
            //Visualización de commits a lo largo del tiempo: gráfico que muestra la evolución del tamaño del repositorio a lo largo de las ditintas versiones
            estadisticas();
        } else if (c == '4') {
            //Tamaño actual del proyecto: en líneas y en bytes o derivados
            cout << tamanyoCarpeta("/Users/alvaro/eclipse-workspace2/proyectoMenu/prueba", 0) << " bytes" << endl;
            estadisticas();
        } else if (c == '5') {
            //Extensiones de los archivos: gráfico de barras que muestra la frecuencia de las distintas extensiones de los archivos del repositorio
            char **n = (char **) malloc(10 * sizeof(char *));
            for (int i = 0; i < 10; i++) {
                n[i] = (char *) malloc(10 * sizeof(char));
            }

            n[0] = "queso.java";
            n[1] = "queso.png";
            n[2] = "queso.avi";
            n[3] = "queso.mp3";
            n[4] = "queso.";
            n[5] = "queso.jpg";
            n[6] = "queso.avi";
            n[7] = "queso.avi";
            n[8] = "queso.jpg";
            n[9] = "queso";

            graficoExtensiones(n, 10);
            estadisticas();
        } else if (c == '6') {
            //Atrás
            misRepositorios();
        } else {
            //Salir
            return;
        }
    }

    void opciones() {
        char c;
        do {
            cin.clear();
            cout << "Selecciona una de las siguientes opciones: " << endl;
            cout << "1. Eliminar colaborador" << endl;
            cout << "2. Borrar repositorio" << endl;
            cout << "3. Atras" << endl;
            cout << "0. Salir" << endl;
            cin.get(c);
        } while (c != '1' && c != '2' && c != '3' && c != '0');

        if (c == '1') {
            //Eliminar colaborador: el sistema borrará al usuario introducido como colaborador del repositorio
            opciones();
        } else if (c == '2') {
            //Borrar repositorio: el sistema borrará toda la información relativa al repositorio
            borrarCarpeta("/Users/alvaro/eclipse-worksape2/proyectoMenu/borrar", 0);
            cout << "Repositorio borrado correctamente." << endl;
            opciones();
        } else if (c == '3') {
            //Atrás
            misRepositorios();
        } else {
            //Salir
            return;
        }
    }

    void inicio() {
        char c;

        do {
            cin.clear();
            cout << "Selecciona una de las siguientes opciones: " << endl;
            cout << "1. Iniciar sesion" << endl;
            cout << "2. Registrarse" << endl;
            cout << "0. Salir" << endl;
            cin.get(c);
        } while (c != '1' && c != '2' && c != '0');

        if (c == '1') {
            //Inicio de sesión: el usuario introduce su nombre de usuario y contraseña para ingresar en la alpicación
            bool inicioExitoso = inicioSesion();
            if (inicioExitoso == true) {
                gestionRepos();
            } else {
                inicio(); //Vueve al menú principal
            }
        } else if (c == '2') {
            //Registro de un nuevo usuario
            registrar();
            inicio();
        } else {
            //Salir del programa
            return;
        }
    }
}

