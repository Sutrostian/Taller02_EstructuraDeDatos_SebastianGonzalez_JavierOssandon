#include "logica/Stack.h"
#include <iostream>
using namespace std;

Stack::Stack()
{
    // inicializar  en nullptr
    top = nullptr;
}

void Stack::apilar(Paciente *p)
{

    NodoHistorial *nuevo = new NodoHistorial();
    nuevo->paciente = p;
    nuevo->siguiente = top; // el nuevo nodo apunta al top actual
    top = nuevo;            // el top pasa a ser el nuevo nodo
}

Paciente *Stack::desapilar()
{
    if (estaVacia())
    {
        return nullptr;
    }

    NodoHistorial *aux = top;
    Paciente *p = aux->paciente;

    // El top baja al siguiente nodo
    top = top->siguiente;

    // Borramos el nodo de la pila (pero no el paciente)
    delete aux;

    return p;
}

bool Stack::estaVacia()
{
    // retornar true si top es nullptr
    return top == nullptr;
}

void Stack::mostrar()
{
    // recorrer desde top hasta el final, imprimiendo cada paciente
    // (queda en orden LIFO)
    if (estaVacia())
    {
        cout << "El historial de atenciones esta vacio." << endl;
        return;
    }

    cout << "Historial de atenciones (de la mas reciente a la mas antigua):" << endl;
    NodoHistorial *aux = top;
    while (aux != nullptr)
    {
        Paciente *p = aux->paciente;
        cout << "Nombre: " << p->getNombre() << ", Edad: " << p->getEdad() << ", Servicio: " << p->getServicio() << endl;
        aux = aux->siguiente;
    }
    cout << "--" << endl;
}

Stack::~Stack()
{
    // liberar todos los nodos restantes para evitar fugas de memoria
    while (!estaVacia())
    {
        NodoHistorial *aux = top;
        top = top->siguiente;
        delete aux; // Solo borrar el nodo.
    }
}