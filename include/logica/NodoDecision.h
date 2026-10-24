#pragma once
#include <string>
using namespace std;

class NodoDecision
{
public:
    string atributo;
    string operador;
    double umbral;
    string valorTexto;
    string servicio;
    NodoDecision *si;
    NodoDecision *no;
};