#include "logica/ArbolDecision.h"
#include <iostream>
#include <sstream>
using namespace std;

ArbolDecision::ArbolDecision()
{
    NodoDecision *dias = crearPregunta("DiasHospitalizacion", ">=", 2,
                                       crearHoja("Hospitalizacion"),
                                       crearHoja("Medicina General"));

    NodoDecision *cirugiaSinTrauma = crearPregunta("RequiereCirugia", "==", 1,
                                                   crearHoja("Cirugia"),
                                                   dias);

    NodoDecision *pecho = crearPreguntaTexto("TipoDolor", "pecho",
                                             crearHoja("Cardiologia"),
                                             cirugiaSinTrauma);

    NodoDecision *neuro = crearPregunta("SintomasNeurologicos", "==", 1,
                                        crearHoja("Neurologia"),
                                        pecho);

    NodoDecision *cirugiaConTrauma = crearPregunta("RequiereCirugia", "==", 1,
                                                   crearHoja("Cirugia"),
                                                   crearHoja("Traumatologia"));

    NodoDecision *trauma = crearPregunta("Trauma", "==", 1,
                                         cirugiaConTrauma,
                                         neuro);

    NodoDecision *edad = crearPregunta("Edad", "<", 15,
                                       crearHoja("Pediatria"),
                                       trauma);

    NodoDecision *presionBaja = crearPregunta("PresionArterial", "<", 90,
                                              crearHoja("Urgencias"),
                                              edad);

    NodoDecision *presionAlta = crearPregunta("PresionArterial", ">=", 180,
                                              crearHoja("Urgencias"),
                                              presionBaja);

    NodoDecision *dolor = crearPregunta("Dolor", ">=", 9,
                                        crearHoja("Urgencias"),
                                        presionAlta);

    raiz = crearPregunta("Temperatura", ">=", 39.5,
                         crearHoja("Urgencias"),
                         dolor);
}

NodoDecision *ArbolDecision::crearPregunta(string atributo, string operador, double umbral,
                                           NodoDecision *si, NodoDecision *no)
{
    NodoDecision *nuevo = new NodoDecision();
    nuevo->atributo = atributo;
    nuevo->operador = operador;
    nuevo->umbral = umbral;
    nuevo->valorTexto = "";
    nuevo->servicio = "";
    nuevo->si = si;
    nuevo->no = no;
    return nuevo;
}

NodoDecision *ArbolDecision::crearPreguntaTexto(string atributo, string valorTexto,
                                                NodoDecision *si, NodoDecision *no)
{
    NodoDecision *nuevo = crearPregunta(atributo, "==", 0, si, no);
    nuevo->valorTexto = valorTexto;
    return nuevo;
}

NodoDecision *ArbolDecision::crearHoja(string servicio)
{
    NodoDecision *nuevo = new NodoDecision();
    nuevo->atributo = "";
    nuevo->operador = "";
    nuevo->umbral = 0;
    nuevo->valorTexto = "";
    nuevo->servicio = servicio;
    nuevo->si = nullptr;
    nuevo->no = nullptr;
    return nuevo;
}

double ArbolDecision::obtenerValor(Paciente *p, string atributo)
{
    if (atributo == "Edad")
        return p->getEdad();
    if (atributo == "Temperatura")
        return p->getTemperatura();
    if (atributo == "PresionArterial")
        return p->getPresionArterial();
    if (atributo == "Dolor")
        return p->getDolor();
    if (atributo == "Trauma")
        return p->getTrauma();
    if (atributo == "SintomasNeurologicos")
        return p->getSintomasNeurologicos();
    if (atributo == "RequiereCirugia")
        return p->getRequiereCirugia();
    if (atributo == "DiasHospitalizacion")
        return p->getDiasHospitalizacion();
    return 0;
}

bool ArbolDecision::cumpleCondicion(NodoDecision *nodo, Paciente *p)
{
    if (nodo->valorTexto != "")
    {
        return p->getTipoDolor() == nodo->valorTexto;
    }

    double valor = obtenerValor(p, nodo->atributo);
    if (nodo->operador == ">=")
        return valor >= nodo->umbral;
    if (nodo->operador == "<")
        return valor < nodo->umbral;
    if (nodo->operador == "==")
        return valor == nodo->umbral;
    return false;
}

string ArbolDecision::textoCondicion(NodoDecision *nodo, bool resultado)
{
    string operador = nodo->operador;
    if (!resultado)
    {
        if (operador == ">=")
            operador = "<";
        else if (operador == "<")
            operador = ">=";
        else if (operador == "==")
            operador = "!=";
    }

    ostringstream texto;
    texto << nodo->atributo << " " << operador << " ";
    if (nodo->valorTexto != "")
        texto << nodo->valorTexto;
    else
        texto << nodo->umbral;
    return texto.str();
}

string ArbolDecision::textoNodo(NodoDecision *nodo)
{
    if (nodo->si == nullptr && nodo->no == nullptr)
    {
        return "[Servicio] " + nodo->servicio;
    }
    return "[Pregunta] " + textoCondicion(nodo, true) + " ?";
}

string ArbolDecision::evaluar(Paciente *p, string &ruta)
{
    ruta = "";
    NodoDecision *actual = raiz;

    while (actual->si != nullptr && actual->no != nullptr)
    {
        bool resultado = cumpleCondicion(actual, p);

        if (ruta != "")
            ruta += " -> ";
        ruta += textoCondicion(actual, resultado);

        if (resultado)
            actual = actual->si;
        else
            actual = actual->no;
    }

    return actual->servicio;
}

void ArbolDecision::preorden(NodoDecision *nodo, int nivel)
{
    if (nodo == nullptr)
        return;
    cout << string(nivel * 2, ' ') << textoNodo(nodo) << endl;
    preorden(nodo->si, nivel + 1);
    preorden(nodo->no, nivel + 1);
}

void ArbolDecision::inorden(NodoDecision *nodo, int nivel)
{
    if (nodo == nullptr)
        return;
    inorden(nodo->si, nivel + 1);
    cout << string(nivel * 2, ' ') << textoNodo(nodo) << endl;
    inorden(nodo->no, nivel + 1);
}

void ArbolDecision::postorden(NodoDecision *nodo, int nivel)
{
    if (nodo == nullptr)
        return;
    postorden(nodo->si, nivel + 1);
    postorden(nodo->no, nivel + 1);
    cout << string(nivel * 2, ' ') << textoNodo(nodo) << endl;
}

void ArbolDecision::mostrarPreorden()
{
    cout << "=== ARBOL DE DECISION (PREORDEN) ===" << endl;
    preorden(raiz, 0);
}

void ArbolDecision::mostrarInorden()
{
    cout << "=== ARBOL DE DECISION (INORDEN) ===" << endl;
    inorden(raiz, 0);
}

void ArbolDecision::mostrarPostorden()
{
    cout << "=== ARBOL DE DECISION (POSTORDEN) ===" << endl;
    postorden(raiz, 0);
}

void ArbolDecision::liberar(NodoDecision *nodo)
{
    if (nodo == nullptr)
        return;
    liberar(nodo->si);
    liberar(nodo->no);
    delete nodo;
}

ArbolDecision::~ArbolDecision()
{
    liberar(raiz);
}