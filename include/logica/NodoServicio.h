#pragma once
#include <string>
#include "logica/NodoPacienteServicio.h"
using namespace std;

class NodoServicio
{
public:
    string nombre;
    NodoPacienteServicio *pacientes;
    NodoServicio *siguiente;
};