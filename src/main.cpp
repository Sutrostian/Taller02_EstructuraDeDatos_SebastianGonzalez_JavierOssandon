#include <iostream>
#include <fstream>
#include <cstring>
#include <cstdlib>
#include "logica/Queue.h"
#include "logica/Hospital.h"
#include "logica/Stack.h"
#include "dominio/Paciente.h"
#include "logica/ArbolDecision.h"
#include <iomanip> // para mostrar temperaturas con 1 decimal
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

bool esDecimalValido(const char *str)
{
    if (str == nullptr || *str == '\0')
    {
        return false;
    }
    const char *p = str;
    bool hayPunto = false;
    bool hayDigito = false;
    while (*p != '\0')
    {
        if (*p == '.')
        {
            if (hayPunto)
            {
                return false;
            }
            hayPunto = true;
        }
        else if (*p >= '0' && *p <= '9')
        {
            hayDigito = true;
        }
        else
        {
            return false;
        }
        p++;
    }
    return hayDigito;
}

bool esBinarioValido(const char *str)
{
    return (*str == '0' || *str == '1') && *(str + 1) == '\0';
}

bool esTipoDolorValido(const string &tipo)
{
    string tipos[5] = {"pecho", "cabeza", "abdomen", "extremidad", "otro"};
    for (int i = 0; i < 5; i++)
    {
        if (tipo == tipos[i])
        {
            return true;
        }
    }
    return false;
}

int leerEntero(const char *mensaje) // Lee un entero validando la entrada; si no es numero, vuelve a pedirlo
{
    int valor;
    while (true)
    {
        cout << mensaje;
        if (cin >> valor)
        {
            cin.ignore(10000, '\n'); // descarta el resto de la linea
            return valor;
        }
        cin.clear();
        cin.ignore(10000, '\n');
        cout << "Entrada invalida. Ingrese un numero." << endl;
    }
}

char *siguienteCampo(char *&ptr)
{
    if (ptr == nullptr)
    {
        return nullptr;
    }
    while (*ptr == ' ')
        ptr++;
    char *inicio = ptr;
    while (*ptr != ';' && *ptr != '\r' && *ptr != '\n' && *ptr != '\0')
        ptr++;
    char *fin = ptr - 1;
    if (*ptr == ';')
    {
        *ptr = '\0';
        ptr++;
    }
    else
    {
        *ptr = '\0';
        ptr = nullptr;
    }
    while (fin >= inicio && *fin == ' ')
    {
        *fin = '\0';
        fin--;
    }
    return inicio;
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
    int numLinea = 0;

    while (getline(archivo, lineaStr))
    {
        numLinea++;
        if (lineaStr.empty() || lineaStr == "\r")
            continue;

        if (lineaStr.size() >= 256)
        {
            cout << "Aviso: La linea " << numLinea << " es demasiado larga. Se omitira." << endl;
            continue;
        }

        char buffer[256];
        strcpy(buffer, lineaStr.c_str());

        char *campos[11];
        char *ptr = buffer;
        int n = 0;
        while (n < 11 && ptr != nullptr)
        {
            campos[n] = siguienteCampo(ptr);
            n++;
        }

        if (n < 11 || (ptr != nullptr && *ptr != '\0'))
        {
            cout << "Aviso: La linea " << numLinea << " no tiene los 11 campos esperados. Se omitira." << endl;
            continue;
        }

        if (!esEnteroValido(campos[0]))
        {
            cout << "Aviso: El ID '" << campos[0] << "' no es un numero valido. Se omitira la linea." << endl;
            continue;
        }
        int id = atoi(campos[0]);

        if (colaEspera->existePaciente(id))
        {
            cout << "Aviso: El paciente con ID " << id << " ya existe (Duplicado). Se omitira." << endl;
            continue;
        }

        string nombre(campos[1]);
        if (nombre.empty())
        {
            cout << "Aviso: El paciente ID " << id << " tiene nombre vacio. Se omitira." << endl;
            continue;
        }

        if (!esEnteroValido(campos[2]))
        {
            cout << "Aviso: El paciente ID " << id << " tiene una edad invalida ('" << campos[2] << "'). Se omitira." << endl;
            continue;
        }
        int edad = atoi(campos[2]);

        if (!esDecimalValido(campos[3]))
        {
            cout << "Aviso: El paciente ID " << id << " tiene una temperatura invalida ('" << campos[3] << "'). Se omitira." << endl;
            continue;
        }
        double temperatura = atof(campos[3]);

        if (!esEnteroValido(campos[4]))
        {
            cout << "Aviso: El paciente ID " << id << " tiene una presion arterial invalida ('" << campos[4] << "'). Se omitira." << endl;
            continue;
        }
        int presion = atoi(campos[4]);

        if (!esEnteroValido(campos[5]) || atoi(campos[5]) > 10)
        {
            cout << "Aviso: El paciente ID " << id << " tiene un dolor invalido ('" << campos[5] << "', debe ser 0-10). Se omitira." << endl;
            continue;
        }
        int dolor = atoi(campos[5]);

        string tipoDolor(campos[6]);
        if (!esTipoDolorValido(tipoDolor))
        {
            cout << "Aviso: El paciente ID " << id << " tiene un tipo de dolor invalido ('" << tipoDolor << "'). Se omitira." << endl;
            continue;
        }

        if (!esBinarioValido(campos[7]) || !esBinarioValido(campos[8]) || !esBinarioValido(campos[9]))
        {
            cout << "Aviso: El paciente ID " << id << " tiene Trauma, SintomasNeurologicos o RequiereCirugia distinto de 0/1. Se omitira." << endl;
            continue;
        }
        int trauma = atoi(campos[7]);
        int sintomasNeuro = atoi(campos[8]);
        int requiereCirugia = atoi(campos[9]);

        if (!esEnteroValido(campos[10]))
        {
            cout << "Aviso: El paciente ID " << id << " tiene dias de hospitalizacion invalidos ('" << campos[10] << "'). Se omitira." << endl;
            continue;
        }
        int dias = atoi(campos[10]);

        Paciente *p = new Paciente(id, nombre, edad, temperatura, presion, dolor, tipoDolor,
                                   trauma, sintomasNeuro, requiereCirugia, dias);
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
    Hospital *hospital = new Hospital();
    Stack *historial = new Stack();
    ArbolDecision *arbol = new ArbolDecision();

    cargarPacientesDesdeArchivo(colaEspera, "data/pacientes_prueba_t2.txt");

    int opcion;
    do
    {
        cout << "=== HOSPITAL MARMAJA ===" << endl;
        cout << "1. Atender pacientes" << endl;
        cout << "2. Ver departamento" << endl;
        cout << "3. Revisar historial de atencion" << endl;
        cout << "4. Buscar paciente por ID o nombre" << endl;
        cout << "5. Ver arbol de decision" << endl;
        cout << "6. Eliminar paciente por ID" << endl;
        cout << "7. Salir" << endl;
        opcion = leerEntero("Seleccionar opcion: ");
        cout << endl;

        switch (opcion)
        {
        case 1:
        {
            colaEspera->mostrar();

            if (!colaEspera->estaVacia())
            {
                int cantidad = leerEntero("\nIndique la cantidad de pacientes a atender: ");

                cout << "\n=== ATENDIENDO PACIENTES ===" << endl;
                for (int i = 0; i < cantidad; i++)
                {
                    if (colaEspera->estaVacia())
                    {
                        cout << "Ya no hay mas pacientes en la cola." << endl;
                        break;
                    }

                    Paciente *p = colaEspera->eliminar();

                    string ruta;
                    string servicioAsignado = arbol->evaluar(p, ruta);
                    p->setServicio(servicioAsignado); // guardo el servicio que asignado por el arbol

                    cout << "ID: " << p->getId() << ", Nombre: " << p->getNombre() << ", Edad: " << p->getEdad()
                         << ", Temperatura: " << fixed << setprecision(1) << p->getTemperatura() << ", Presion: " << p->getPresionArterial()
                         << ", Dolor: " << p->getDolor() << ", TipoDolor: " << p->getTipoDolor()
                         << ", Trauma: " << p->getTrauma() << ", SintomasNeuro: " << p->getSintomasNeurologicos()
                         << ", RequiereCirugia: " << p->getRequiereCirugia() << ", DiasHospitalizacion: " << p->getDiasHospitalizacion()
                         << "\nRuta: " << ruta << "\nServicio asignado: " << servicioAsignado << endl;

                    if (hospital->agregarPaciente(p->getServicio(), p))
                    {
                        historial->apilar(p);
                        cout << "Paciente insertado en el AVL de " << p->getServicio() << ".\n"
                             << endl;
                    }
                    else
                    {
                        cout << "No se pudo insertar al paciente " << p->getId() << " (ID repetido)." << endl;
                        delete p; // si no entro al avl lo borro yo
                    }
                }
            }
            break;
        }
        case 2:
        {
            hospital->mostrarTodos();

            int opcDepto = leerEntero("\nSeleccionar departamento (1-8) o 0 para cancelar: ");
            if (opcDepto >= 1 && opcDepto <= 8)
            {
                int idx = opcDepto - 1;
                int sub;
                do
                {
                    cout << "\n--- " << hospital->getNombreServicio(idx) << " ---" << endl;
                    cout << "1. Recorrido inorden" << endl;
                    cout << "2. Recorrido preorden" << endl;
                    cout << "3. Recorrido postorden" << endl;
                    cout << "4. Mostrar altura" << endl;
                    cout << "5. Mostrar factor de balance" << endl;
                    cout << "0. Volver" << endl;
                    sub = leerEntero("Opcion: ");
                    cout << endl;

                    if (sub >= 1 && sub <= 3)
                        hospital->mostrarRecorrido(idx, sub);
                    else if (sub == 4)
                        hospital->mostrarAltura(idx);
                    else if (sub == 5)
                        hospital->mostrarBalance(idx);
                } while (sub != 0);
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
            cout << "Indique el ID o Nombre del paciente: ";
            string entrada;
            getline(cin, entrada);
            if (!entrada.empty() && entrada.back() == '\r')
                entrada.pop_back();

            Paciente *encontrado = nullptr;
            bool enEspera = false;
            string servicioEncontrado, rutaBusqueda;

            if (esEnteroValido(entrada.c_str()))
            {
                int id = atoi(entrada.c_str());
                encontrado = colaEspera->buscarPorId(id);
                if (encontrado != nullptr)
                    enEspera = true;
                else
                    encontrado = hospital->buscarPorId(id, servicioEncontrado, rutaBusqueda);
            }
            else
            {
                encontrado = colaEspera->buscarPorNombre(entrada);
                if (encontrado != nullptr)
                    enEspera = true;
                else
                {
                    encontrado = hospital->buscarPorNombre(entrada);
                    if (encontrado != nullptr) // con el ID obtengo servicio y ruta
                        hospital->buscarPorId(encontrado->getId(), servicioEncontrado, rutaBusqueda);
                }
            }

            if (encontrado == nullptr)
            {
                cout << "No se encontro ningun paciente con ese ID o nombre." << endl;
                break;
            }

            cout << "\nPaciente encontrado:" << endl;
            cout << "-------" << endl;
            cout << "ID: " << encontrado->getId() << "\nNombre: " << encontrado->getNombre()
                 << "\nEdad: " << encontrado->getEdad() << endl;
            if (enEspera)
            {
                cout << "Estado: En espera (pendiente de clasificacion)" << endl;
            }
            else
            {
                cout << "Departamento: " << servicioEncontrado << endl;
                cout << "T: " << fixed << setprecision(1) << encontrado->getTemperatura()
                     << " | D: " << encontrado->getDolor()
                     << " | TD: " << encontrado->getTipoDolor()
                     << " | Tr: " << (encontrado->getTrauma() ? "Si" : "No")
                     << " | SN: " << (encontrado->getSintomasNeurologicos() ? "Si" : "No")
                     << " | RC: " << (encontrado->getRequiereCirugia() ? "Si" : "No")
                     << " | DH: " << encontrado->getDiasHospitalizacion() << endl;
                cout << "-------" << endl;
                cout << "Ruta de busqueda: " << rutaBusqueda << endl;
            }
            break;
        }
        case 5:
        {
            cout << "\n=== ARBOL DE DECISION ===" << endl;
            arbol->mostrarPreorden();
            cout << endl;
            arbol->mostrarInorden();
            cout << endl;
            arbol->mostrarPostorden();
            cout << endl;
            break;
        }
        case 6:
        {
            int idEliminar = leerEntero("Ingrese el ID del paciente a eliminar: ");
            string servicioEliminado;
            Paciente *eliminado = hospital->eliminarPorId(idEliminar, servicioEliminado);
            if (eliminado != nullptr)
            {
                cout << "Paciente con ID " << idEliminar << " eliminado del servicio " << servicioEliminado << "." << endl;
                delete eliminado; // Liberar memoria del paciente eliminado
            }
            else
            {
                cout << "No se encontro ningun paciente con ID " << idEliminar << ", verifique que el paciente exista y que no este en la cola de espera." << endl;
            }
            break;
        }

        case 7:
            cout << "Hasta luego : D." << endl;
            break;
        default:
            cout << "Opcion invalida. Intente nuevamente." << endl
                 << endl;
        }
    } while (opcion != 7);

    delete colaEspera;
    delete historial;
    delete hospital;
    delete arbol;
    return 0;
}