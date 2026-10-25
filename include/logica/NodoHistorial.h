#pragma once
#include <string>
using namespace std;

class NodoHistorial
{
public:
    int id;
    string nombre;
    int edad;
    string servicio;
    NodoHistorial *siguiente;
};