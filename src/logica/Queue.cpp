#include "logica/Queue.h"
#include <iostream>
using namespace std;

Queue::Queue()
{

    // inicializar start en nullptr
    start = nullptr;
}

void Queue::insertar(Paciente *p)
{

    // Creamos el nuevo nodo queue y le asignamos el paciente
    NodoQueue *nuevo = new NodoQueue();
    nuevo->paciente = p;
    nuevo->next = nullptr;

    // Si la cola está vacía, el nuevo nodo es el inicio
    if (estaVacia())
    {
        start = nuevo;
    }
    else
    {
        // si no, recorremos hasta el final para agregarlo al último (FIFO)
        NodoQueue *aux = start;
        while (aux->next != nullptr)
        {
            aux = aux->next;
        }
        aux->next = nuevo;
    }
}

Paciente *Queue::eliminar()
{
    // sacar el nodo del frente, retornar su paciente y liberar el nodo
    // debe retornar nullptr si la cola está vacía
    if (estaVacia())
    {
        return nullptr;
    }
    else
    {
        NodoQueue *aux = start;
        Paciente *pacienteEliminado = aux->paciente;
        start = start->next;      // mover el inicio al siguiente nodo
        delete aux;               // liberar memoria del nodo eliminado
        return pacienteEliminado; // retornar el paciente eliminado
    }
}

bool Queue::estaVacia()
{

    if (start == nullptr)
    {
        return true;
    }
    return false;
}

void Queue::mostrar()
{
    // recorrer la cola e imprimir cada paciente

    if (estaVacia())
    {
        cout << "La cola de espera esta vacia." << endl;
        return;
    }

    NodoQueue *aux = start;
    int posicion = 1;
    cout << "PACIENTES EN ESPERA" << endl;
    while (aux != nullptr)
    {
        cout << posicion << ". " << aux->paciente->getId() << " - "
             << aux->paciente->getNombre() << endl;

        aux = aux->next;
        posicion++;
    }
}

bool Queue::existePaciente(int idBuscado)
{
    NodoQueue *actual = start;
    while (actual != nullptr)
    {
        if (actual->paciente->getId() == idBuscado)
        {
            return true;
        }
        actual = actual->next; // duplicado
    }
    return false; // No está duplicado
}

Queue::~Queue()
{
    // liberar todos los nodos restantes

    while (!estaVacia())
    {
        NodoQueue *aux = start;
        delete aux->paciente;
        start = start->next;
        delete aux;
    }
}

Paciente *Queue::buscarPorId(int idBuscado)
{
    NodoQueue *actual = start;
    while (actual != nullptr)
    {
        if (actual->paciente->getId() == idBuscado)
        {
            return actual->paciente;
        }
        actual = actual->next;
    }
    return nullptr; // no se encontro
}
