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
    Paciente *eliminarPorId(int id, string &servicio);
    string getNombreServicio(int indice);
    void mostrarRecorrido(int indice, int tipo); // 1 inorden, 2 preorden, 3 postorden
    void mostrarAltura(int indice);
    void mostrarBalance(int indice);
    ~Hospital();
};