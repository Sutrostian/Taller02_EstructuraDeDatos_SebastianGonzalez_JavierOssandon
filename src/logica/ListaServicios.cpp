#include "logica/ListaServicios.h"
#include <iostream>
using namespace std;

ListaServicios::ListaServicios()
{
    // crear los 8 nodos fijos de servicios (Urgencias, Medicina General, Cardiologia, Neurologia, Traumatologia, Cirugia, Pediatria, Hospitalizacion)
    //  y encadenarlos entre si, dejando 'inicio' apuntando al primero

    string servicios[] = {"Urgencias", "Medicina General", "Cardiologia", "Neurologia", "Traumatologia", "Cirugia", "Pediatria", "Hospitalizacion"};
    inicio = nullptr;
    NodoServicio *actual = nullptr;
    for (int i = 0; i < 8; i++)
    {
        NodoServicio *nuevo = new NodoServicio();
        nuevo->nombre = servicios[i];
        nuevo->pacientes = nullptr;
        nuevo->siguiente = nullptr;

        if (inicio == nullptr)
        {
            inicio = nuevo;
            actual = inicio;
        }
        else
        {
            actual->siguiente = nuevo;
            actual = actual->siguiente;
        }
    }
}

NodoServicio *ListaServicios::buscarServicio(string nombreServicio)
{
    // recorrer la lista desde 'inicio' comparando nombres
    //  retornar el nodo si lo encuentra, o nullptr si no existe ese servicio

    NodoServicio *aux = inicio;
    while (aux != nullptr)
    {
        if (aux->nombre == nombreServicio)
        {
            return aux; // Retorna el nodo si encuentra el servicio
        }
        aux = aux->siguiente;
    }
    return nullptr; // Si el servicio no existe o está mal escrito
}

void ListaServicios::agregarPaciente(string nombreServicio, Paciente *p)
{
    NodoServicio *servicio = buscarServicio(nombreServicio);

    if (servicio != nullptr)
    {
        // Crear un nuevo nodo para la sub-lista del paciente
        NodoPacienteServicio *nuevo = new NodoPacienteServicio();
        nuevo->paciente = p;
        nuevo->siguiente = nullptr;

        // Si el servicio no tiene pacientes, este es el primero
        if (servicio->pacientes == nullptr)
        {
            servicio->pacientes = nuevo;
        }
        else
        {
            // Si ya tiene pacientes, lo insertamos al final de su fila
            NodoPacienteServicio *aux = servicio->pacientes;
            while (aux->siguiente != nullptr)
            {
                aux = aux->siguiente;
            }
            aux->siguiente = nuevo;
        }
    }
    else
    {
        cout << "Error: El servicio '" << nombreServicio << "' no existe." << endl;
    }
}

void ListaServicios::mostrarServicio(string nombreServicio)
{
    // usar buscarServicio(), recorrer su lista de pacientes e imprimir cada uno

    NodoServicio *servicio = buscarServicio(nombreServicio);
    if (servicio != nullptr)
    {
        cout << "Pacientes en el servicio de " << nombreServicio << ":" << endl;
        NodoPacienteServicio *aux = servicio->pacientes;
        int posicion = 1;
        while (aux != nullptr)
        {
            cout << posicion << ". " << aux->paciente->getId() << " - "
                 << aux->paciente->getNombre() << endl;
            aux = aux->siguiente;
            posicion++;
        }
        if (posicion == 1)
        {
            cout << "No hay pacientes en este servicio." << endl;
        }
    }
    else
    {
        cout << "Error: El servicio '" << nombreServicio << "' no existe." << endl;
    }
}

void ListaServicios::mostrarTodos()
{
    // recorrer la lista principal de servicios,
    // mostrando el nombre y cuantos pacientes tiene cada uno
    cout << "Lista de Servicios y cantidad de pacientes:" << endl;
    NodoServicio *aux = inicio;
    int contador = 1;
    while (aux != nullptr)
    {
        // Contar pacientes en el servicio actual
        int cantidadPacientes = 0;
        NodoPacienteServicio *auxPacientes = aux->pacientes;
        while (auxPacientes != nullptr)
        {
            cantidadPacientes++;
            auxPacientes = auxPacientes->siguiente;
        }

        cout << contador << ". " << aux->nombre << " - " << cantidadPacientes << " pacientes" << endl;
        aux = aux->siguiente;
        contador++;
    }
}

ListaServicios::~ListaServicios()
{
    // liberar TODOS los nodos: primero los pacientes de cada servicio,
    // luego los nodos de servicio en si
    NodoServicio *auxServicio = inicio;
    while (auxServicio != nullptr)
    {
        // Liberar la lista de pacientes del servicio actual
        NodoPacienteServicio *auxPaciente = auxServicio->pacientes;
        while (auxPaciente != nullptr)
        {
            NodoPacienteServicio *tempPaciente = auxPaciente;
            auxPaciente = auxPaciente->siguiente;

            delete tempPaciente->paciente; // <-- AGREGAR ESTA LÍNEA (Borra el objeto)
            delete tempPaciente;           // (Borra el nodo)
        }

        // Liberar el nodo de servicio actual
        NodoServicio *tempServicio = auxServicio;
        auxServicio = auxServicio->siguiente;
        delete tempServicio; // liberar memoria del nodo servicio
    }
}

Paciente *ListaServicios::buscarPacientePorId(int idBuscado)
{
    NodoServicio *servicioActual = inicio;
    while (servicioActual != nullptr)
    {
        NodoPacienteServicio *pacienteActual = servicioActual->pacientes;
        while (pacienteActual != nullptr)
        {
            if (pacienteActual->paciente->getId() == idBuscado)
            {
                return pacienteActual->paciente;
            }
            pacienteActual = pacienteActual->siguiente;
        }
        servicioActual = servicioActual->siguiente;
    }
    return nullptr; // no se encontro en ningun servicio
}