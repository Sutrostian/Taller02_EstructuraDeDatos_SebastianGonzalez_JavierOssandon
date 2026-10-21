#include "dominio/Paciente.h"

Paciente::Paciente(int id, string nombre, int edad, string servicio)
    : Persona(id, nombre, edad) {
    this->servicio = servicio;
}

Paciente::~Paciente() {
}