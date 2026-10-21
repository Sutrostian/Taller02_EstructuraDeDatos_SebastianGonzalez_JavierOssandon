#pragma once
#include "dominio/Paciente.h"

class NodoPacienteServicio
{
public:
    Paciente *paciente;
    NodoPacienteServicio *siguiente;
};