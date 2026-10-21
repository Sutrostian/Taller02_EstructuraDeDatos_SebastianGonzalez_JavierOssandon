#pragma once
#include "logica/NodoQueue.h"
#include "dominio/Paciente.h"

class Queue
{
private:
    NodoQueue *start;

public:
    Queue();
    void insertar(Paciente *p);
    Paciente *eliminar();
    bool estaVacia();
    void mostrar();
    bool existePaciente(int idBuscado);
    Paciente *buscarPorId(int idBuscado);
    ~Queue();
};