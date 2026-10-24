#pragma once
#include <string>
#include "logica/AVL.h"
#include "dominio/Paciente.h"
using namespace std;

class Hospital
{
private:
    AVL *arboles[8];
    string nombres[8];

    int buscarIndice(string servicio);

public:
    Hospital();
    bool agregarPaciente(string servicio, Paciente *p);
    void mostrarTodos();
    void mostrarServicio(string servicio);
    Paciente *buscarPorId(int id, string &servicio, string &ruta);
    Paciente *buscarPorNombre(string nombre);
    ~Hospital();
};