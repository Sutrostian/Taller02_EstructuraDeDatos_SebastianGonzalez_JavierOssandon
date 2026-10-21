#pragma once
#include "dominio/Paciente.h"

class NodoHistorial
{
public:
    Paciente *paciente;
    NodoHistorial *siguiente;
};