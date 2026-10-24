#include "logica/AVL.h"
#include <iostream>
using namespace std;

AVL::AVL()
{
    raiz = nullptr;
    cantidad = 0;
}

int AVL::altura(NodoAVL *nodo)
{
    if (nodo == nullptr)
        return 0;
    return nodo->altura;
}

int AVL::factorBalance(NodoAVL *nodo)
{
    if (nodo == nullptr)
        return 0;
    return altura(nodo->izq) - altura(nodo->der);
}

void AVL::actualizarAltura(NodoAVL *nodo)
{
    int altIzq = altura(nodo->izq);
    int altDer = altura(nodo->der);
    if (altIzq > altDer)
        nodo->altura = altIzq + 1;
    else
        nodo->altura = altDer + 1;
}

NodoAVL *AVL::rotacionDerecha(NodoAVL *nodo)
{
    NodoAVL *nuevaRaiz = nodo->izq;
    nodo->izq = nuevaRaiz->der;
    nuevaRaiz->der = nodo;
    actualizarAltura(nodo);
    actualizarAltura(nuevaRaiz);
    return nuevaRaiz;
}

NodoAVL *AVL::rotacionIzquierda(NodoAVL *nodo)
{
    NodoAVL *nuevaRaiz = nodo->der;
    nodo->der = nuevaRaiz->izq;
    nuevaRaiz->izq = nodo;
    actualizarAltura(nodo);
    actualizarAltura(nuevaRaiz);
    return nuevaRaiz;
}

NodoAVL *AVL::rotacionIzquierdaDerecha(NodoAVL *nodo)
{
    nodo->izq = rotacionIzquierda(nodo->izq);
    return rotacionDerecha(nodo);
}

NodoAVL *AVL::rotacionDerechaIzquierda(NodoAVL *nodo)
{
    nodo->der = rotacionDerecha(nodo->der);
    return rotacionIzquierda(nodo);
}

NodoAVL *AVL::balancear(NodoAVL *nodo)
{
    actualizarAltura(nodo);
    int fb = factorBalance(nodo);

    if (fb > 1)
    {
        if (factorBalance(nodo->izq) >= 0)
        {
            cout << "Rotacion simple derecha en el nodo " << nodo->paciente->getId() << endl;
            return rotacionDerecha(nodo);
        }
        cout << "Rotacion doble izquierda-derecha en el nodo " << nodo->paciente->getId() << endl;
        return rotacionIzquierdaDerecha(nodo);
    }

    if (fb < -1)
    {
        if (factorBalance(nodo->der) <= 0)
        {
            cout << "Rotacion simple izquierda en el nodo " << nodo->paciente->getId() << endl;
            return rotacionIzquierda(nodo);
        }
        cout << "Rotacion doble derecha-izquierda en el nodo " << nodo->paciente->getId() << endl;
        return rotacionDerechaIzquierda(nodo);
    }

    return nodo;
}

NodoAVL *AVL::insertar(NodoAVL *nodo, Paciente *p, bool &insertado)
{
    if (nodo == nullptr)
    {
        NodoAVL *nuevo = new NodoAVL();
        nuevo->paciente = p;
        nuevo->izq = nullptr;
        nuevo->der = nullptr;
        nuevo->altura = 1;
        insertado = true;
        return nuevo;
    }

    if (p->getId() < nodo->paciente->getId())
        nodo->izq = insertar(nodo->izq, p, insertado);
    else if (p->getId() > nodo->paciente->getId())
        nodo->der = insertar(nodo->der, p, insertado);
    else
        return nodo;

    return balancear(nodo);
}

bool AVL::insertar(Paciente *p)
{
    bool insertado = false;
    raiz = insertar(raiz, p, insertado);
    if (insertado)
        cantidad++;
    return insertado;
}

Paciente *AVL::buscarPorId(int id, string &ruta)
{
    ruta = "";
    NodoAVL *actual = raiz;
    while (actual != nullptr)
    {
        if (ruta != "")
            ruta += " -> ";
        ruta += to_string(actual->paciente->getId());

        if (id == actual->paciente->getId())
            return actual->paciente;
        if (id < actual->paciente->getId())
            actual = actual->izq;
        else
            actual = actual->der;
    }
    return nullptr;
}

Paciente *AVL::buscarPorNombre(NodoAVL *nodo, string nombre)
{
    if (nodo == nullptr)
        return nullptr;
    if (nodo->paciente->getNombre() == nombre)
        return nodo->paciente;
    Paciente *encontrado = buscarPorNombre(nodo->izq, nombre);
    if (encontrado != nullptr)
        return encontrado;
    return buscarPorNombre(nodo->der, nombre);
}

Paciente *AVL::buscarPorNombre(string nombre)
{
    return buscarPorNombre(raiz, nombre);
}

NodoAVL *AVL::minimo(NodoAVL *nodo)
{
    while (nodo->izq != nullptr)
        nodo = nodo->izq;
    return nodo;
}

NodoAVL *AVL::eliminar(NodoAVL *nodo, int id, Paciente *&eliminado)
{
    if (nodo == nullptr)
        return nullptr;

    if (id < nodo->paciente->getId())
        nodo->izq = eliminar(nodo->izq, id, eliminado);
    else if (id > nodo->paciente->getId())
        nodo->der = eliminar(nodo->der, id, eliminado);
    else
    {
        eliminado = nodo->paciente;

        if (nodo->izq == nullptr || nodo->der == nullptr)
        {
            NodoAVL *hijo = nodo->izq;
            if (hijo == nullptr)
                hijo = nodo->der;
            delete nodo;
            return hijo;
        }

        NodoAVL *sucesor = minimo(nodo->der);
        nodo->paciente = sucesor->paciente;
        Paciente *aux = nullptr;
        nodo->der = eliminar(nodo->der, sucesor->paciente->getId(), aux);
    }

    return balancear(nodo);
}

Paciente *AVL::eliminar(int id)
{
    Paciente *eliminado = nullptr;
    raiz = eliminar(raiz, id, eliminado);
    if (eliminado != nullptr)
        cantidad--;
    return eliminado;
}

bool AVL::estaVacio()
{
    return raiz == nullptr;
}

int AVL::getAltura()
{
    return altura(raiz);
}

int AVL::getCantidad()
{
    return cantidad;
}

void AVL::inorden(NodoAVL *nodo)
{
    if (nodo == nullptr)
        return;
    inorden(nodo->izq);
    cout << nodo->paciente->getNombre() << " (" << nodo->paciente->getId() << ")" << endl;
    inorden(nodo->der);
}

void AVL::preorden(NodoAVL *nodo)
{
    if (nodo == nullptr)
        return;
    cout << nodo->paciente->getNombre() << " (" << nodo->paciente->getId() << ")" << endl;
    preorden(nodo->izq);
    preorden(nodo->der);
}

void AVL::postorden(NodoAVL *nodo)
{
    if (nodo == nullptr)
        return;
    postorden(nodo->izq);
    postorden(nodo->der);
    cout << nodo->paciente->getNombre() << " (" << nodo->paciente->getId() << ")" << endl;
}

void AVL::mostrarBalance(NodoAVL *nodo)
{
    if (nodo == nullptr)
        return;
    mostrarBalance(nodo->izq);
    cout << "ID " << nodo->paciente->getId()
         << ": altura " << nodo->altura
         << ", factor de balance " << factorBalance(nodo) << endl;
    mostrarBalance(nodo->der);
}

void AVL::mostrarInorden()
{
    if (estaVacio())
        cout << "Sin pacientes" << endl;
    else
        inorden(raiz);
}

void AVL::mostrarPreorden()
{
    if (estaVacio())
        cout << "Sin pacientes" << endl;
    else
        preorden(raiz);
}

void AVL::mostrarPostorden()
{
    if (estaVacio())
        cout << "Sin pacientes" << endl;
    else
        postorden(raiz);
}

void AVL::mostrarBalance()
{
    if (estaVacio())
        cout << "Sin pacientes" << endl;
    else
        mostrarBalance(raiz);
}

void AVL::liberar(NodoAVL *nodo)
{
    if (nodo == nullptr)
        return;
    liberar(nodo->izq);
    liberar(nodo->der);
    delete nodo->paciente;
    delete nodo;
}

AVL::~AVL()
{
    liberar(raiz);
}
