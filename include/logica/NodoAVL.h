#pragma once
#include "dominio/Paciente.h"

class NodoAVL
{
public:
    Paciente *paciente;
    NodoAVL *izq;
    NodoAVL *der;
    int altura;
};

