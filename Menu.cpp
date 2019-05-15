#include <iostream>
#include "Menu.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
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
        bool b = true;
        cout << "Nombre de usuario: " << endl; //Conseguir nombre de usuario y comprobar en la BD
        cout << "Contrasenya: " << endl; //Conseguir contraseña y comprobar en la BD
        /**
         * Hacer atributos nombre y contraseña??
         * */
        if (b == false) {
            char c;
            cout << "Error en el inicio de sesion, ¿desea volver a intentarlo?" << endl;
            do {
                cout << "1. Si" << endl;
                cout << "2. No" << endl;
                cin.get();
                cin.get(c);
            } while (c != '1' && c != '2');

            if (c == '1') {
                inicioSesion();
            } else {
                b = false;
            }
        }
        return b;
    }

    void gestionRepos() {
        char c;
        do {
            cin.sync();
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
            gestionRepos();
        } else if (c == '3') {
            //Cierra sesión y vuelve al menú principal
            inicio();
        } else {
            //Salir
            return;
        }
    }

    void estadisticas();

    void historialVersiones();

    void opciones();

    void misRepositorios() {
        char c;
        do {
            cin.sync();
            cout << "Selecciona una de las siguientes opciones: " << endl;
            cout << "1. Historial de versiones " << endl;
            cout << "2. Hacer commit " << endl;
            cout << "3. Duplicar " << endl;
            cout << "4. Listar colaborador " << endl;
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
        } else if (c == '2') {
            //Hacer commit: el sistema pedirá al usuario que seleccione qué archivos quiere incluir en el commit y realizará el commit
            misRepositorios();
        } else if (c == '3') {
            //Duplicar: el sistema creará una copia del repositorio en el sistema de archivos del usuario, pidiendo antes al usuario que introduzca un nombre
            //que no esté en uso para dicho repositorio
            misRepositorios();
        } else if (c == '4') {
            //Listar colaborador: muestra una lista con los diferentes colaboradores del repositorio
            misRepositorios();
        } else if (c == '5') {
            //Añadir colaborador: se pide el nombre de un usuario y se añadirá a la lista de los colaboradores de dicho repositorio siempre y cuando
            //exista dicho usuario
            misRepositorios();
        } else if (c == '6') {
            //Estadísticas: distintos datos y gráficos que permiten analizar la evolución y el estado actual del repositorio
            estadisticas();
        } else if (c == '7') {
            //Esquema de archivos: se mostrará un esquema de los archivos que componen el repositorio, en forma de árbol, de manera legible
            //para el usuario
            mostrarArbolDirectorios("/Users/alvaro/eclipse-workspace2/proyectoMenu/prueba", 0);
            misRepositorios();
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
        char c;
        do {
            cin.sync();
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
            historialVersiones();
        } else if (c == '2') {
            //Restaurar a esta versión: realiza un nuevo commit en el que se vuelve a la versión seleccionada
            historialVersiones();
        } else if (c == '3') {
            //Atrás
            misRepositorios();
        } else {
            //Salir
            return;
        }
    }

    void estadisticas() {
        char c;
        do {
            cin.sync();
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
            cin.sync();
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
            cin.sync();
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
            inicio();
        } else {
            //Salir del programa
            return;
        }
    }
}
/*
int main() {
    //crearCarpeta("/Users/alvaro/eclipse-workspace2/proyectoMenu/a11");
    copiaCarpeta("C:/Users/pablo/Desktop/prog4_bd/origen","C:/Users/pablo/Desktop/prog4_bd/destino", 0);
    //copiarArchivo("/Users/alvaro/eclipse-workspace2/proyectoMenu/a/a.docx", "/Users/alvaro/eclipse-workspace2/proyectoMenu/src/a.docx");
    //mostrarArbolDirectorios("/Users/alvaro/eclipse-workspace2/proyectoMenu/a11", 0);
    //crearCarpeta("/Users/alvaro/eclipse-workspace2/proyectoMenu/a_commit");
    inicio();

    return 0;
}*/