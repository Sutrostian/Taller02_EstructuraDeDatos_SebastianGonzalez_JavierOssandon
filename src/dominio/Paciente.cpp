#include "dominio/Paciente.h"

Paciente::Paciente(int id, string nombre, int edad, double temperatura,
                   int presionArterial, int dolor, string tipoDolor, int trauma,
                   int sintomasNeurologicos, int requiereCirugia,
                   int diasHospitalizacion)
    : Persona(id, nombre, edad)
{
    this->servicio = ""; // todavia no lo asignamos.
    this->temperatura = temperatura;
    this->presionArterial = presionArterial;
    this->dolor = dolor;
    this->tipoDolor = tipoDolor;
    this->trauma = trauma;
    this->sintomasNeurologicos = sintomasNeurologicos;
    this->requiereCirugia = requiereCirugia;
    this->diasHospitalizacion = diasHospitalizacion;
}

Paciente::~Paciente()
{
}