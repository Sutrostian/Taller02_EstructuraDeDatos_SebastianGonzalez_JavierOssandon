#pragma once
#include "dominio/Paciente.h"

class NodoQueue
{
public:
    Paciente *paciente;
    NodoQueue *next;
};