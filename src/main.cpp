#include <iostream>
#include <fstream>
#include <cstring>
#include <cstdlib>
#include "logica/Queue.h"
#include "logica/ListaServicios.h"
#include "logica/Stack.h"
#include "dominio/Paciente.h"

using namespace std;

// Valida que un string represente un numero entero positivo (solo digitos, sin signo)
bool esEnteroValido(const char *str)
{
    if (str == nullptr || *str == '\0')
    {
        return false; // vacio no es valido
    }
    const char *p = str;
    while (*p != '\0')
    {
        if (*p < '0' || *p > '9')
        {
            return false; // cualquier caracter que no sea digito invalida el numero
        }
        p++;
    }
    return true;
}

// Función para leer el archivo
void cargarPacientesDesdeArchivo(Queue *colaEspera, const string &nombreArchivo)
{
    ifstream archivo(nombreArchivo);
    if (!archivo.is_open())
    {
        cout << "Error: No se pudo abrir el archivo '" << nombreArchivo << "'." << endl;
        return;
    }

    string lineaStr;
    int cargados = 0;

    while (getline(archivo, lineaStr))
    {
        if (lineaStr.empty())
            continue;

        char buffer[256];
        strcpy(buffer, lineaStr.c_str());

        char *ptr = buffer;
        char *inicio = buffer;

        // 1. Extraer ID
        while (*ptr != ';' && *ptr != '\0')
            ptr++;
        if (*ptr == '\0')
            continue;
        *ptr = '\0';

        // VALIDACION: el ID debe ser un numero valido
        if (!esEnteroValido(inicio))
        {
            cout << "Aviso: El ID '" << inicio << "' no es un numero valido. Se omitira la linea." << endl;
            continue;
        }
        int id = atoi(inicio);

        // Validación de duplicados usando la Queue
        if (colaEspera->existePaciente(id))
        {
            cout << "Aviso: El paciente con ID " << id << " ya existe (Duplicado). Se omitira." << endl;
            continue;
        }

        // Extraer Nombre
        ptr++;
        while (*ptr == ' ')
            ptr++;
        inicio = ptr;
        while (*ptr != ';' && *ptr != '\0')
            ptr++;
        if (*ptr == '\0')
            continue;
        *ptr = '\0';
        string nombre(inicio);

        // el nombre no puede estar vacio
        if (nombre.empty())
        {
            cout << "Aviso: El paciente ID " << id << " tiene nombre vacio. Se omitira." << endl;
            continue;
        }

        // Extraer Edad
        ptr++;
        while (*ptr == ' ')
            ptr++;
        inicio = ptr;
        while (*ptr != ';' && *ptr != '\0')
            ptr++;
        if (*ptr == '\0')
            continue;
        *ptr = '\0';

        // la edad debe ser un numero valido
        if (!esEnteroValido(inicio))
        {
            cout << "Aviso: El paciente ID " << id << " tiene una edad invalida ('" << inicio << "'). Se omitira." << endl;
            continue;
        }
        int edad = atoi(inicio);

        // Extraer Servicio
        ptr++;
        while (*ptr == ' ')
            ptr++;
        inicio = ptr;
        while (*ptr != '\r' && *ptr != '\n' && *ptr != '\0')
            ptr++;
        *ptr = '\0';
        string servicio(inicio);

        // Comprobar si el servicio es válido
        string serviciosValidos[8] = {
            "Urgencias", "Medicina General", "Cardiologia",
            "Neurologia", "Traumatologia", "Cirugia",
            "Pediatria", "Hospitalizacion"};

        bool servicioValido = false;
        for (int i = 0; i < 8; i++)
        {
            if (servicio == serviciosValidos[i])
            {
                servicioValido = true;
                break;
            }
        }

        if (!servicioValido)
        {
            cout << "Aviso: El servicio '" << servicio << "' no es valido. Se omitira el paciente ID " << id << "." << endl;
            continue; // Saltamos esta línea si el departamento no existe
        }
        //--------

        Paciente *p = new Paciente(id, nombre, edad, servicio);
        colaEspera->insertar(p);
        cargados++;
    }

    archivo.close();
    cout << "Se cargaron " << cargados << " pacientes desde el archivo.\n"
         << endl;
}

int main()
{
    Queue *colaEspera = new Queue();
    ListaServicios *hospital = new ListaServicios();
    Stack *historial = new Stack();

    cargarPacientesDesdeArchivo(colaEspera, "data/pacientes_casos_borde.txt");

    int opcion;
    do
    {
        cout << "=== HOSPITAL MARMAJA ===" << endl;
        cout << "1. Atender pacientes" << endl;
        cout << "2. Ver departamento" << endl;
        cout << "3. Revisar historial de atencion" << endl;
        cout << "4. Buscar paciente por ID" << endl;
        cout << "5. Salir" << endl;
        cout << "Seleccionar opcion: ";
        cin >> opcion;
        cout << endl;

        switch (opcion)
        {
        case 1:
        {
            colaEspera->mostrar();

            if (!colaEspera->estaVacia())
            {
                int cantidad;
                cout << "\nIndique la cantidad de pacientes a atender: ";
                cin >> cantidad;

                cout << "\n=== ATENDIENDO PACIENTES ===" << endl;
                for (int i = 0; i < cantidad; i++)
                {
                    if (colaEspera->estaVacia())
                    {
                        cout << "Ya no hay mas pacientes en la cola." << endl;
                        break;
                    }

                    Paciente *p = colaEspera->eliminar();

                    cout << "ID: " << p->getId() << "\nNombre: " << p->getNombre()
                         << "\nEdad: " << p->getEdad() << "\nServicio: " << p->getServicio() << endl;

                    hospital->agregarPaciente(p->getServicio(), p);
                    historial->apilar(p);

                    cout << "Paciente enviado a " << p->getServicio() << ".\n"
                         << endl;
                }
            }
            break;
        }
        case 2:
        {
            hospital->mostrarTodos();

            int opcDepto;
            cout << "\nSeleccionar departamento (1-8) o 0 para cancelar: ";
            cin >> opcDepto;

            string nombresServicios[] = {"Urgencias", "Medicina General", "Cardiologia",
                                         "Neurologia", "Traumatologia", "Cirugia",
                                         "Pediatria", "Hospitalizacion"};

            if (opcDepto >= 1 && opcDepto <= 8)
            {
                cout << endl;
                hospital->mostrarServicio(nombresServicios[opcDepto - 1]);
            }
            cout << endl;
            break;
        }
        case 3:
        {
            historial->mostrar();
            cout << endl;
            break;
        }
        case 4:
        {
            int idBuscado;
            cout << "Ingrese el ID del paciente a buscar: ";
            cin >> idBuscado;

            Paciente *encontrado = colaEspera->buscarPorId(idBuscado);
            if (encontrado != nullptr)
            {
                cout << "\nPaciente encontrado (en espera):" << endl;
                cout << "ID: " << encontrado->getId() << "\nNombre: " << encontrado->getNombre()
                     << "\nEdad: " << encontrado->getEdad() << "\nServicio: " << encontrado->getServicio() << endl;
                break;
            }

            encontrado = hospital->buscarPacientePorId(idBuscado);
            if (encontrado != nullptr)
            {
                cout << "\nPaciente encontrado (ya atendido):" << endl;
                cout << "ID: " << encontrado->getId() << "\nNombre: " << encontrado->getNombre()
                     << "\nEdad: " << encontrado->getEdad() << "\nServicio: " << encontrado->getServicio() << endl;
                break;
            }

            cout << "No se encontro ningun paciente con ID " << idBuscado << "." << endl;
            break;
        }
        case 5:
            cout << "Hasta luego : D." << endl;
            break;
        default:
            cout << "Opcion invalida. Intente nuevamente." << endl
                 << endl;
        }
    } while (opcion != 5);

    delete colaEspera;
    delete historial;
    delete hospital;

    return 0;
}