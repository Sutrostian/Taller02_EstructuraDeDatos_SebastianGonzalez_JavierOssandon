#pragma once
#include <string>
#include "logica/NodoServicio.h"
#include "dominio/Paciente.h"
using namespace std;

class ListaServicios
{
private:
    NodoServicio *inicio;

    NodoServicio *buscarServicio(string nombreServicio);

public:
    ListaServicios();
    void agregarPaciente(string nombreServicio, Paciente *p);
    void mostrarServicio(string nombreServicio);
    void mostrarTodos();
    Paciente *buscarPacientePorId(int idBuscado);
    ~ListaServicios();
};