#pragma once
#include "logica/NodoHistorial.h"
#include "dominio/Paciente.h"

class Stack
{
private:
    NodoHistorial *top;

public:
    Stack();
    void apilar(Paciente *p);
    Paciente *desapilar();
    bool estaVacia();
    void mostrar();
    ~Stack();
};