# Taller 2 Estructura de Datos Hospital Marmaja

## Descripción
Sistema de gestión de pacientes de un hospital, desarrollado en C++ con punteros, programación orientada a objetos y estructuras de datos implementadas manualmente. El sistema lee los pacientes desde un archivo de texto, los encola respetando su orden de llegada, determina el servicio de cada uno mediante un **árbol de decisión** y los almacena en un **árbol AVL** por servicio.

Flujo principal:

```
Archivo → Cola → Árbol de decisión → Servicio → AVL
```

## Integrantes
- Sebastián González - 21.186.016-2 - [Sutrostian](https://github.com/Sutrostian) - Ingeniería Civil Industrial - Paralelo: C1
- Javier Ossandón - 21.979.689-7 - [itsvoiiid](https://github.com/itsvoiiid) - Ingeniería Civil Industrial - Paralelo: C1

## Requisitos previos
- Compilador de C++ compatible con C++17 (g++).
- En Windows se recomienda MSYS2 (UCRT64). En Linux o GitHub Codespaces, g++ ya viene instalado.

## Compilación

Windows:
```bash
g++ -std=c++17 -Iinclude src/dominio/*.cpp src/logica/*.cpp src/main.cpp -o Taller.exe
```

Linux / GitHub Codespaces:
```bash
g++ -std=c++17 -Iinclude src/dominio/*.cpp src/logica/*.cpp src/main.cpp -o Taller
```

## Ejecución

Windows:
```bash
./Taller.exe
```

Linux / GitHub Codespaces:
```bash
./Taller
```

Hay que ejecutar el programa desde la carpeta raíz del proyecto, porque la ruta del archivo de pacientes es relativa (`data/pacientes_prueba_t2.txt`).

## Uso
Al ejecutar el programa se carga el archivo de pacientes y se muestra el menú principal:

```
=== HOSPITAL MARMAJA ===
1. Atender pacientes
2. Ver departamento
3. Revisar historial de atencion
4. Buscar paciente por ID o nombre
5. Ver arbol de decision
6. Eliminar paciente por ID
7. Salir
Seleccionar opcion:
```

- **Atender pacientes**: muestra la cola de espera y permite indicar cuántos atender. Cada paciente se saca de la cola, se evalúa con el árbol de decisión (se muestra la ruta seguida), se le asigna el servicio, se inserta en el AVL de ese servicio y se registra en el historial.
- **Ver departamento**: muestra los ocho servicios con su cantidad de pacientes y los nombres de quienes están en cada uno. Luego permite elegir un servicio para abrir su submenú, con las opciones: recorrido inorden, preorden y postorden, altura del AVL y factor de balance de cada nodo.
- **Revisar historial de atención**: muestra el registro de atenciones, de la más reciente a la más antigua.
- **Buscar paciente por ID o nombre**: si lo ingresado es numérico se busca por ID; si no, por nombre. Busca primero en la cola de espera y luego en los AVL de los servicios. Si el paciente ya fue atendido, muestra su departamento, sus datos clínicos y la ruta de búsqueda dentro del AVL.
- **Ver árbol de decisión**: muestra los recorridos preorden, inorden y postorden del árbol.
- **Eliminar paciente por ID**: busca al paciente en los AVL de los servicios, lo elimina manteniendo el árbol balanceado y libera su memoria. Solo elimina pacientes ya atendidos; si el paciente sigue en la cola de espera o no existe, se informa. El historial de atenciones se conserva como registro, porque guarda una copia de los datos del paciente.
- **Salir**: termina la ejecución liberando la memoria dinámica.

Las opciones numéricas del menú están validadas: si se ingresa un texto en vez de un número, el programa avisa y vuelve a pedir el dato.

## Formato del archivo de entrada
Un paciente por línea, con los campos separados por `;`:

```
ID;Nombre;Edad;Temperatura;PresionArterial;Dolor;TipoDolor;Trauma;SintomasNeurologicos;RequiereCirugia;DiasHospitalizacion
```

| Campo | Significado |
|---|---|
| ID | Identificador único (entero positivo) |
| Nombre | Nombre del paciente |
| Edad | Edad en años (entero) |
| Temperatura | Temperatura corporal en °C (decimal con punto, ej. `37.5`) |
| PresionArterial | Presión sistólica en mmHg (entero) |
| Dolor | Escala de 0 a 10 |
| TipoDolor | `pecho`, `cabeza`, `abdomen`, `extremidad` u `otro` |
| Trauma | `0` no, `1` sí |
| SintomasNeurologicos | `0` no, `1` sí |
| RequiereCirugia | `0` no, `1` sí |
| DiasHospitalizacion | Días estimados (entero) |

Las líneas inválidas no se cargan: se informa un aviso con el motivo y el programa continúa con la siguiente línea. Se controlan archivo inexistente, cantidad incorrecta de campos, valores no numéricos, rangos fuera de lo permitido, tipo de dolor desconocido, valores distintos de 0/1 y pacientes con ID duplicado.

## Árbol de decisión

### Estructura
Cada nodo (`NodoDecision`) es una pregunta o una hoja. Las preguntas tienen un atributo, un operador, un umbral y dos punteros (`si` / `no`); las hojas contienen el servicio. El árbol permite construcción, evaluación de un paciente (devolviendo el servicio y la ruta seguida) y recorridos preorden, inorden y postorden.

### Criterio de decisión
Las preguntas se evalúan en este orden, y la primera que se cumple define el camino:

```
¿Temperatura >= 39.5?              Sí -> Urgencias
¿Dolor >= 9?                       Sí -> Urgencias
¿PresionArterial >= 180?           Sí -> Urgencias
¿PresionArterial < 90?             Sí -> Urgencias
¿Edad < 15?                        Sí -> Pediatria
¿Trauma == 1?
   Sí: ¿RequiereCirugia == 1?      Sí -> Cirugia / No -> Traumatologia
¿SintomasNeurologicos == 1?        Sí -> Neurologia
¿TipoDolor == "pecho"?             Sí -> Cardiologia
¿RequiereCirugia == 1?             Sí -> Cirugia
¿DiasHospitalizacion >= 2?         Sí -> Hospitalizacion / No -> Medicina General
```

Atributos utilizados (9): Temperatura, Dolor, PresionArterial, Edad, Trauma, SintomasNeurologicos, TipoDolor, RequiereCirugia y DiasHospitalizacion. Los ocho servicios tienen al menos una ruta.

### Justificación
- **Urgencias primero**: los signos vitales críticos (fiebre alta, dolor intenso, presión muy alta o muy baja) indican riesgo inmediato y deben atenderse antes de pensar en la especialidad, incluso en niños.
- **Pediatría antes que las especialidades**: un paciente menor de 15 años se atiende en pediatría salvo que esté en urgencia vital.
- **Trauma antes que neurología y cardiología**: un trauma define el servicio de la lesión; dentro de él, la necesidad de cirugía separa Cirugía de Traumatología.
- **Neurología y cardiología**: los síntomas neurológicos y el dolor de pecho son indicadores específicos de esas especialidades.
- **Cirugía sin trauma**: un paciente que requiere cirugía sin ser un caso de trauma va a Cirugía.
- **Hospitalización**: si no corresponde a ninguna especialidad, una estadía estimada de 2 días o más indica internación; si no, se atiende en Medicina General.

### Datos inválidos, insuficientes y empates
- Un registro inválido o con campos faltantes no llega al árbol: se descarta al cargar el archivo, con un aviso.
- No hay empates: cada paciente sigue un único camino y el orden de las preguntas es el criterio de prioridad (la primera condición que se cumple gana).
- Si ninguna condición de especialidad se cumple, el paciente va a Medicina General.

## Árbol AVL
Cada servicio tiene su propio AVL, con el ID del paciente como clave de ordenamiento.

- Altura de un puntero nulo: 0; altura de una hoja: 1.
- Factor de balance = altura(izquierdo) − altura(derecho). El árbol está balanceado cuando va de −1 a 1.
- Rotaciones: simple izquierda, simple derecha, doble izquierda-derecha y doble derecha-izquierda.
- Operaciones: insertar (rechaza IDs duplicados), buscar por ID (con la ruta recorrida), buscar por nombre, eliminar por ID (con rebalanceo), recorridos inorden/preorden/postorden, altura y factor de balance. Cada vez que una inserción o eliminación requiere rotar, el programa lo informa por pantalla.

## Estructura del proyecto
```
├── include/
│   ├── dominio/     → Persona.h, Paciente.h
│   └── logica/      → NodoQueue.h, Queue.h, NodoHistorial.h, Stack.h,
│                       NodoDecision.h, ArbolDecision.h, NodoAVL.h, AVL.h, Hospital.h
├── src/
│   ├── dominio/     → Persona.cpp, Paciente.cpp
│   ├── logica/      → Queue.cpp, Stack.cpp, ArbolDecision.cpp, AVL.cpp, Hospital.cpp
│   └── main.cpp     → carga del archivo, menú principal
├── data/            → pacientes_prueba_t2.txt (casos de prueba del Taller 2)
├── .gitignore
└── README.md
```

## Jerarquía de clases (POO)
- **Persona** (clase padre): `id`, `nombre`, `edad`, con getters y destructor virtual.
- **Paciente** (hija de Persona): agrega servicio, temperatura, presión arterial, dolor, tipo de dolor, trauma, síntomas neurológicos, requiere cirugía y días de hospitalización.

## Funcionalidades
- [x] Jerarquía de clases Persona/Paciente
- [x] Carga de pacientes con el formato de 11 campos y validaciones
- [x] Cola de pacientes pendientes (Queue) - FIFO
- [x] Historial de atenciones (Stack) - LIFO
- [x] Árbol de decisión: construcción, evaluación con ruta y recorridos
- [x] Árbol AVL con rotaciones, inserción, búsqueda, eliminación y recorridos
- [x] Un AVL por servicio integrado al flujo de atención
- [x] Menú con recorridos, altura y factor de balance por servicio
- [x] Búsqueda por ID o nombre con ruta de búsqueda en el AVL
- [x] Eliminación de pacientes por ID desde el menú
- [x] Liberación de memoria (destructores en Queue, Stack, ArbolDecision, AVL y Hospital)

## Casos de prueba (`data/pacientes_prueba_t2.txt`)

### Clasificación (un caso por servicio)

| ID | Caso | Servicio esperado |
|---|---|---|
| 001 | Adulto sin síntomas | Medicina General |
| 002 | Niño de 10 años | Pediatria |
| 003 | Trauma con cirugía | Cirugia |
| 004 | Trauma sin cirugía | Traumatologia |
| 005 | Síntomas neurológicos | Neurologia |
| 006 | Dolor de pecho | Cardiologia |
| 007 | Temperatura 39.8 (sobre el umbral de 39.5) | Urgencias |
| 008 | 4 días de hospitalización | Hospitalizacion |

### Rotaciones del AVL
Los pacientes 030 a 060 y 020/017 (ya clasificados por el árbol) fuerzan las cuatro rotaciones. Al atender a todos los pacientes se muestran los mensajes de rotación:

| Servicio | IDs insertados (en orden) | Rotación | Nodo |
|---|---|---|---|
| Neurologia | 005, 030, 040 | Simple izquierda | 5 |
| Neurologia | 035, 033 (con 040 en el árbol) | Simple derecha | 40 |
| Traumatologia | 004, 050, 060 | Simple izquierda | 4 |
| Traumatologia | 055, 057 (con 060 en el árbol) | Doble izquierda-derecha | 60 |
| Cardiologia | 006, 020, 017 | Doble derecha-izquierda | 6 |

### Datos inválidos (se descartan al cargar, con aviso)

| Línea | Problema |
|---|---|
| ID 003 repetido | Paciente duplicado |
| ID 009 | Faltan campos |
| ID 010 | Temperatura no numérica (`3x.5`) |
| ID 011 | Dolor fuera de rango (15) |
| ID 012 | TipoDolor desconocido (`espalda`) |
| ID 013 | Trauma distinto de 0/1 |
| (sin ID) | ID vacío |
| ID 014 | Nombre vacío |

También se controla un archivo inexistente: se informa el error y el programa continúa con la cola vacía.

## Servicios disponibles
Urgencias, Medicina General, Cardiología, Neurología, Traumatología, Cirugía, Pediatría, Hospitalización
