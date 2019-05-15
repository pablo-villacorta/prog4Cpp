#include "Usuario.h"
#include <string.h>
#include <iostream>
#include "Main.h"

Usuario::Usuario(string nickname, string nombre, string apellido, string contra) {
    this->nickname = string(nickname);
    this->nombre = string(nombre);
    this->apellido = string(apellido);
    this->contra = string(contra);
}

Usuario::~Usuario() {

}

string Usuario::getNickname() const {
    return nickname;
}

string Usuario::getNombre() const {
    return nombre;
}

string Usuario::getApellido() const {
    return apellido;
}

string Usuario::getContra() const {
    return contra;
}


