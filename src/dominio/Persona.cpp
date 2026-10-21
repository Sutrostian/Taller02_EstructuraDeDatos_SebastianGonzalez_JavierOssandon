#include "dominio/Persona.h"

Persona::Persona(int id, string nombre, int edad) {
    this->id = id;
    this->nombre = nombre;
    this->edad = edad;
}

Persona::~Persona() {
}