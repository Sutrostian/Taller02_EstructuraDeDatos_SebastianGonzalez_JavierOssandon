#include "logica/Hospital.h"
#include <iostream>
using namespace std;

Hospital::Hospital()
{
    string lista[8] = {"Urgencias", "Medicina General", "Cardiologia", "Neurologia",
                       "Traumatologia", "Cirugia", "Pediatria", "Hospitalizacion"};
    for (int i = 0; i < 8; i++)
    {
        nombres[i] = lista[i];
        arboles[i] = new AVL(); // un avl por servicio
    }
}

int Hospital::buscarIndice(string servicio)
{
    for (int i = 0; i < 8; i++)
    {
        if (nombres[i] == servicio)
            return i;
    }
    return -1; // no existe el servicio
}

bool Hospital::agregarPaciente(string servicio, Paciente *p)
{
    int i = buscarIndice(servicio);
    if (i == -1)
    {
        cout << "Error: El servicio '" << servicio << "' no existe." << endl;
        return false;
    }
    return arboles[i]->insertar(p);
}

void Hospital::mostrarTodos()
{
    cout << "Lista de Servicios y cantidad de pacientes:" << endl;
    for (int i = 0; i < 8; i++)
    {
        cout << (i + 1) << ". " << nombres[i] << " - "
             << arboles[i]->getCantidad() << " pacientes" << endl;
    }
}

void Hospital::mostrarServicio(string servicio)
{
    int i = buscarIndice(servicio);
    if (i == -1)
    {
        cout << "Error: El servicio '" << servicio << "' no existe." << endl;
        return;
    }
    cout << "Pacientes en el servicio de " << servicio << " (inorden):" << endl;
    arboles[i]->mostrarInorden();
}

Paciente *Hospital::buscarPorId(int id, string &servicio, string &ruta)
{
    for (int i = 0; i < 8; i++)
    {
        Paciente *p = arboles[i]->buscarPorId(id, ruta);
        if (p != nullptr)
        {
            servicio = nombres[i];
            return p;
        }
    }
    ruta = "";
    return nullptr;
}

Paciente *Hospital::buscarPorNombre(string nombre)
{
    for (int i = 0; i < 8; i++)
    {
        Paciente *p = arboles[i]->buscarPorNombre(nombre);
        if (p != nullptr)
            return p;
    }
    return nullptr;
}

Hospital::~Hospital()
{
    for (int i = 0; i < 8; i++)
        delete arboles[i];
}