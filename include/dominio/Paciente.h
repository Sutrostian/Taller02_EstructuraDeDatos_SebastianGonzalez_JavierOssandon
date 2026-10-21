#pragma once
#include <string>
#include "Persona.h"
using namespace std;

class Paciente : public Persona
{
private:
    string servicio;

public:
    // constructor
    Paciente(int id, string nombre, int edad, string servicio);
    //"getter" para obtener el servicio del paciente
    string getServicio() { return servicio; }

    ~Paciente();
};