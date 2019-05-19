#include <iostream>
#include "Menu.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include "BD.h"
#include "main.h"
#include <sstream>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

extern "C"{
#include "manejo_archivos.h"
#include "graficos.h"
}

using namespace std;

namespace menu_ {
    /**
    * Devuelve true si el inicio de sesión está bien hecho
    **/
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
        string nomUsuario, nombre, apellido, contra;
        cout << "Introduzca su nombre: " << endl;
        cin >> nombre;
        cout << "Introduzca su apellido: " << endl;
        cin >> apellido;
        cout << "Introduzca su nombre de usuario: " << endl;
        cin >> nomUsuario;
        cout << "Introduzca su contrasenya: " << endl;
        cin >> contra;

        string coma(",");
        bool filtro = nombre.find(coma) == string::npos;
        filtro = filtro && apellido.find(coma) == string::npos;
        filtro = filtro && nomUsuario.find(coma) == string::npos;
        filtro = filtro && contra.find(coma) == string::npos;

        if(!filtro) {
            cout << "Error, ninguno de los campos introducidos puede contener el caracter de coma (',')" << endl;
            return false;
        }

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

            string coma(",");
            bool filtro = nombre.find(coma) == string::npos;
            filtro = filtro && descripcion.find(coma) == string::npos;
            filtro = filtro && ruta.find(coma) == string::npos;

            if(!filtro) {
                cout << "Error, ninguno de los campos introducidos puede contener el caracter de coma (',')" << endl;
                gestionRepos();
                return;
            }

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
            controlador::usuarioActual = 0;
            controlador::repoActual = 0;
            controlador::commitActual = 0;
            inicio();
        } else {
            //Salir
            return;
        }
    }

    void estadisticas();

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
            cout << rep.size() << ". Atras" << endl;
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

        string coma(",");
        if(desc.find(coma) != string::npos) {
            cout << "Error, el campo introducido no puede contener el caracter de coma (',')" << endl;
            return;
        }

        controlador::repoActual->commit(desc);
    }

    void crearRepoDuplicado(){
        string nom, ruta;

        cout << "Introduce el nombre con el que se quieres que guarde el duplicado: " << endl;
        cin >> nom;
        cout << "Introduce la ruta donde quieres que se guarde el repositorio duplicado: " << endl;
        cin >> ruta;

        string coma(",");
        bool filtro = nom.find(coma) == string::npos;
        filtro = filtro && ruta.find(coma) == string::npos;

        if(!filtro) {
            cout << "Error, ninguno de los campos introducidos puede contener el caracter de coma (',')" << endl;
            return;
        }

        if(bbdd::existeRepo(nom)) {
            cout << "Error, ya existe un repositorio con ese nombre" << endl;
            return;
        }

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

        delete[] r;
        delete[] a;
        delete[] rO;
        delete[] rD;
    }

    void listarColaborador(){
        vector<Usuario*> *colab = controlador::repoActual->getColabs();
        for(int i = 0; i < controlador::repoActual->getColabs()->size(); i++){
            cout << i+1 << ". ";
            cout << ((controlador::repoActual->getColabs())->at(i))->getNickname() << endl;
        }
        if(colab->size() == 0) {
            cout << "Este repositorio no tiene ningun colaborador" << endl;
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
            cout << "8. Atras " << endl;
            cout << "0. Salir" << endl;
            cin.get();
            cin.get(c);
        } while (c != '1' && c != '2' && c != '3' && c != '4' && c != '5' && c != '6' && c != '7' && c != '8' && c != '0');

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
            addColaborador();
            menuMisRepositorios();
        } else if (c == '6') {
            //Estadísticas: distintos datos y gráficos que permiten analizar la evolución y el estado actual del repositorio
            estadisticas();
        } else if (c == '7') {
            //Esquema de archivos: se mostrará un esquema de los archivos que componen el repositorio, en forma de árbol, de manera legible
            //para el usuario
            Repo *r = controlador::repoActual;
            char *rut = new char[r->getRuta().size()+1];
            aChar(rut, r->getRuta());
            mostrarArbolDirectorios(rut, 0);
            menuMisRepositorios();
            delete[] rut;
        } else if (c == '8') {
            //Atrás
            misRepositorios();
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
            cout << "2. Restaurar a esta version " << endl;
            cout << "3. Atras " << endl;
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
            delete[] rr;
            delete[] rc;
        } else if (c == '3') {
            //Atrás
            misRepositorios();
        } else {
            //Salir
            exit(0);
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
            cout << "Error, el nombre de usuario introducido no coincide con el de ningun usuario" << endl;
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

    void commitsUsuario(){
        int len = (controlador::repoActual->getColabs()->size())+1;
        string nombres[len];
        if(len > 0){
            for(int i = 0; i < len-1; i++){
                nombres[i] = controlador::repoActual->getColabs()->at(i)->getNickname();
            }
        }
        nombres[len-1] = controlador::repoActual->getDuenyo()->getNickname();

        long int valores[len];
        for(int i = 0; i < len; i++){
            valores[i] = 0;
        }

        for(int i = 0; i < len; i++) {
            for(int j = 0; j < controlador::repoActual->getCommits()->size(); j++){
                if(controlador::repoActual->getCommits()->at(j)->getAutor()->getNickname() == nombres[i]){
                    valores[i]++;
                }
            }
        }

        char *nom[len];
        for(int i = 0; i < len; i++){
            nom[i] = new char[nombres[i].size()+1];
            aChar(nom[i], nombres[i]);
        }

        graficoBarras(nom, valores, len);
        for(int i = 0; i < len; i++){
            delete[] nom[i];
        }
    }

    void estadisticas() {
        char c;
        do {
            cin.clear();
            cout << "Selecciona una de las siguientes opciones: " << endl;
            cout << "1. Commits/usuario" << endl;
            cout << "2. Evolucion de un archivo" << endl;
            cout << "3. Visualizacion de commits a lo largo del tiempo" << endl;
            cout << "4. Tamanyo actual del proyecto" << endl;
            cout << "5. Extensiones de los archivos" << endl;
            cout << "6. Atras" << endl;
            cout << "0. Salir" << endl;
            cin.get();
            cin.get(c);
        } while (c != '1' && c != '2' && c != '3' && c != '4' && c != '5' && c != '6' && c != '0');

        if (c == '1') {
            //Commits/usuario: gráfico de barras que muestra la cantidad de commits realizados por cada usuario
/* */       commitsUsuario();
            estadisticas();
        } else if (c == '2') {
            //Evolución de un archivo: gráfico que muestra la evolución del tamaño de un archivo a lo largo de las diferentes versiones
            statEvolucionArchivo(); //NOUVEAU
            estadisticas(); //NOUVEAU
        } else if (c == '3') {
            //Visualización de commits a lo largo del tiempo: gráfico que muestra la evolución del tamaño del repositorio a lo largo de las ditintas versiones
            statEvolucionRepo(); //NOUVEAU
            estadisticas(); //NOUVEAU
        } else if (c == '4') {
            //Tamaño actual del proyecto
            string s = controlador::repoActual->getRuta();
            char *c = new char[(controlador::repoActual->getRuta()).size()+1];
            aChar(c, s);
            cout << "Tamanyo de la carpeta: " << tamanyoCarpeta(c, 0) << " bytes" << endl;
            estadisticas();
            delete[] c;
        } else if (c == '5') {
            //Extensiones de los archivos: gráfico de barras que muestra la frecuencia de las distintas extensiones de los archivos del repositorio
            statExtensiones();
            estadisticas();
        } else if (c == '6') {
            //Atrás
            menuMisRepositorios();
        } else {
            //Salir
            return;
        }
    }


    void inicio() {
        string s;
        int c;
        do {
            cin.clear();
            cout << "Selecciona una de las siguientes opciones: " << endl;
            cout << "1. Iniciar sesion" << endl;
            cout << "2. Registrarse" << endl;
            cout << "0. Salir" << endl;
            cin >> s;

            c = stoi(s);
        } while (c != 1 && c != 2 && c != 0);

        if (c == 1) {
            //Inicio de sesión: el usuario introduce su nombre de usuario y contraseña para ingresar en la alpicación
            if (inicioSesion()) {
                gestionRepos();
            } else {
                inicio(); //Vueve al menú principal
            }
        } else if (c == 2) {
            //Registro de un nuevo usuario
            registrar();
            inicio();
        } else {
            //Salir del programa
            return;
        }
    }

    //NOUVEAU
    void statEvolucionRepo() {
        char **pathsCarpetas;
        char *nombres = new char[7];
        strcpy(nombres, "commit");
        int c;

        Repo *repo = controlador::repoActual;
        c = repo->getCommits()->size();
        vector<string> paths;

        if(c == 0) {
            cout << "Este repositorio no tiene ningun commit registrado" << endl;
            return;
        }

        for(int i = 0; i < c; i++) {
            paths.push_back(repo->getCommits()->at(i)->getRuta());
        }

        //convertir vector<string> a char**
        pathsCarpetas = new char*[c];
        for(int i = 0; i < c; i++) {
            pathsCarpetas[i] = new char[paths[i].size()+1];
            aChar(pathsCarpetas[i], paths[i]);
        }

        graficoTamanyoCarpeta(pathsCarpetas, c, nombres);
        delete[] pathsCarpetas;
        delete[] nombres;
    }

    //NOUVEAU
    void statEvolucionArchivo() {
        string nom;
        cout << "Introduce la ruta relativa del archivo del que quieres realizar el seguimiento: " << endl;
        cin >> nom;

        Repo *r = controlador::repoActual;

        char *n = new char[nom.size()+1];
        aChar(n, nom);
        int len = r->getCommits()->size();

        if(len == 0) {
            cout << "Este repositorio no tiene ningun commit registrado" << endl;
            return;
        }

        char **p = new char*[len];
        for(int i = 0; i < len; i++) {
            Commit *c = r->getCommits()->at(i);
            string a = c->getRuta();
            a.append("/");
            a.append(nom);
            p[i] = new char[a.size()+1];
            aChar(p[i], a);
        }

        graficoTamanyoArchivos(p, len, n);
        delete[] n;
        delete[] p;
    }

    void statExtensiones() {
        string p = controlador::repoActual->getRuta();
        char *path = new char[p.size()+1];
        aChar(path, p);

        vector<char*> files;
        listarArchivos(&files, path);

        char **a = new char*[files.size()];
        for(int i = 0; i < files.size(); i++) {
            a[i] = files[i];
        }

        graficoExtensiones(a, files.size());
        delete[] path;
        delete[] a;
    }

    int is_regular_file(const char *path)
    {
        struct stat path_stat;
        stat(path, &path_stat);
        return S_ISREG(path_stat.st_mode);
    }

    void listarArchivos(vector<char*> *a, char *path) {
        struct dirent *entry;
        DIR *dp;

        if(is_regular_file(path)) {
            a->push_back(path);
            return;
        }

        dp = opendir(path);
        if (dp == NULL) {
            return;
        }

        while ((entry = readdir(dp))) {
            if(strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
                string p(path);
                p.append("/");
                p.append(string(entry->d_name));
                char *b = new char[p.size()+1];
                aChar(b, p);
                listarArchivos(a, b);
            }
        }

        closedir(dp);
    }
}
