#pragma once
#include <string>
#include "logica/NodoDecision.h"
#include "dominio/Paciente.h"
using namespace std;

class ArbolDecision
{
private:
    NodoDecision *raiz;

    NodoDecision *crearPregunta(string atributo, string operador, double umbral,
                                NodoDecision *si, NodoDecision *no);
    NodoDecision *crearPreguntaTexto(string atributo, string valorTexto,
                                     NodoDecision *si, NodoDecision *no);
    NodoDecision *crearHoja(string servicio);

    double obtenerValor(Paciente *p, string atributo);
    bool cumpleCondicion(NodoDecision *nodo, Paciente *p);
    string textoCondicion(NodoDecision *nodo, bool resultado);
    string textoNodo(NodoDecision *nodo);

    void preorden(NodoDecision *nodo, int nivel);
    void inorden(NodoDecision *nodo, int nivel);
    void postorden(NodoDecision *nodo, int nivel);
    void liberar(NodoDecision *nodo);

public:
    ArbolDecision();
    string evaluar(Paciente *p, string &ruta);
    void mostrarPreorden();
    void mostrarInorden();
    void mostrarPostorden();
    ~ArbolDecision();
};