#pragma once
#include <string>
using namespace std;

class Persona
{
protected:
    int id;
    string nombre;
    int edad;

public:
    Persona(int id, string nombre, int edad);
    virtual ~Persona(); // para que borre bien un Paciente aunque lo tenga como Persona*

    //"getter" para obtener los atributos de la persona
    int getId() { return id; }
    string getNombre() { return nombre; }
    int getEdad() { return edad; }
};