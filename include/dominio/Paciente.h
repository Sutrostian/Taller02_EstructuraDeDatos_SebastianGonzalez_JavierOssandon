#pragma once
#include <string>
#include "Persona.h"
using namespace std;

class Paciente : public Persona
{
private:
    string servicio;
    double temperatura;
    int presionArterial;
    int dolor;
    string tipoDolor;
    int trauma;
    int sintomasNeurologicos;
    int requiereCirugia;
    int diasHospitalizacion;

public:
        Paciente(int id, string nombre, int edad, double temperatura, // constructor nuevo
             int presionArterial, int dolor, string tipoDolor, int trauma,
             int sintomasNeurologicos, int requiereCirugia,
             int diasHospitalizacion);

    string getServicio() { return servicio; }
    void setServicio(string s) { servicio = s; }
    double getTemperatura() { return temperatura; }
    int getPresionArterial() { return presionArterial; }
    int getDolor() { return dolor; }
    string getTipoDolor() { return tipoDolor; }
    int getTrauma() { return trauma; }
    int getSintomasNeurologicos() { return sintomasNeurologicos; }
    int getRequiereCirugia() { return requiereCirugia; }
    int getDiasHospitalizacion() { return diasHospitalizacion; }

    ~Paciente();
};