#pragma once
#include <string>
#include "logica/NodoAVL.h"
#include "dominio/Paciente.h"
using namespace std;

class AVL
{
private:
    NodoAVL *raiz;
    int cantidad;

    int altura(NodoAVL *nodo);
    int factorBalance(NodoAVL *nodo);
    void actualizarAltura(NodoAVL *nodo);

    NodoAVL *rotacionDerecha(NodoAVL *nodo);
    NodoAVL *rotacionIzquierda(NodoAVL *nodo);
    NodoAVL *rotacionIzquierdaDerecha(NodoAVL *nodo);
    NodoAVL *rotacionDerechaIzquierda(NodoAVL *nodo);
    NodoAVL *balancear(NodoAVL *nodo);

    NodoAVL *insertar(NodoAVL *nodo, Paciente *p, bool &insertado);
    NodoAVL *eliminar(NodoAVL *nodo, int id, Paciente *&eliminado);
    NodoAVL *minimo(NodoAVL *nodo);
    Paciente *buscarPorNombre(NodoAVL *nodo, string nombre);

    void inorden(NodoAVL *nodo);
    void preorden(NodoAVL *nodo);
    void postorden(NodoAVL *nodo);
    void mostrarBalance(NodoAVL *nodo);
    void mostrarNombres(NodoAVL *nodo, bool &primero);
    void liberar(NodoAVL *nodo);

public:
    AVL();
    bool insertar(Paciente *p);
    Paciente *buscarPorId(int id, string &ruta);
    Paciente *buscarPorNombre(string nombre);
    Paciente *eliminar(int id);
    bool estaVacio();
    int getAltura();
    int getCantidad();
    void mostrarInorden();
    void mostrarPreorden();
    void mostrarPostorden();
    void mostrarBalance();
    void mostrarNombres();
    ~AVL();
};
