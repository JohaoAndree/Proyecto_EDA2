# Sistema de Control de Accesos — Gestión de Entradas

> Sistema de gestión y control de accesos para eventos masivos, desarrollado en **C++** como proyecto académico del curso de **Estructuras de Datos y Algoritmos II (EDA II)**.

---

## Tabla de Contenidos

- [Descripción General](#descripción-general)
- [Arquitectura del Sistema](#arquitectura-del-sistema)
- [Estructuras de Datos Implementadas](#estructuras-de-datos-implementadas)
  - [Hash Table — Registro y búsqueda de asistentes](#hash-table--registro-y-búsqueda-de-asistentes)
  - [Max Heap — Gestión de prioridades](#max-heap--gestión-de-prioridades)
  - [Árbol AVL — Análisis temporal y por zonas](#árbol-avl--análisis-temporal-y-por-zonas)
- [Estructura del Proyecto](#estructura-del-proyecto)
- [Funcionalidades](#funcionalidades)
- [Complejidad Algorítmica](#complejidad-algorítmica)
- [Requisitos](#requisitos)
- [Compilación y Ejecución](#compilación-y-ejecución)
- [Ejemplo de Uso](#ejemplo-de-uso)
- [Datos de Prueba](#datos-de-prueba)
- [Autores](#autores)
- [Licencia](#licencia)

---

## Descripción General

El sistema gestiona el ingreso de asistentes a un evento, permitiendo:

- **Registrar** accesos de forma individual o mediante carga masiva desde archivo.
- **Buscar y validar** asistentes por su DNI en tiempo constante promedio.
- **Priorizar** el acceso según un valor de prioridad asignado a cada persona.
- **Consultar** estadísticas de accesos por franjas horarias y zonas del evento.

Cada persona registrada se almacena simultáneamente en tres estructuras de datos especializadas, donde cada una resuelve un problema específico del dominio.

---

## Arquitectura del Sistema

```
┌─────────────────────────────────────────────────────────┐
│                    main.cpp (CLI)                        │
│          Menú interactivo de 8 opciones                 │
└──────────┬──────────────┬──────────────┬────────────────┘
           │              │              │
           ▼              ▼              ▼
   ┌──────────────┐ ┌───────────┐ ┌───────────────┐
   │  Hash Table  │ │  Max Heap │ │   Árbol AVL   │
   │  (DNI → Info)│ │ (Priorid.)│ │ (Hora → Zona) │
   └──────────────┘ └───────────┘ └───────────────┘
```

Las tres estructuras comparten el modelo de datos definido en `Persona.h`:

```cpp
struct Persona {
    string dni;       // Documento Nacional de Identidad
    string nombre;    // Nombre del asistente
    int    edad;      // Edad
    int    zona;      // Zona asignada en el evento
    string hora;      // Hora de acceso (formato HH:MM)
    int    prioridad; // Prioridad de acceso (1-100)
};
```

---

## Estructuras de Datos Implementadas

### Hash Table — Registro y búsqueda de asistentes

| Aspecto | Detalle |
|---|---|
| **Técnica de hashing** | División (módulo) con soporte para claves negativas |
| **Resolución de colisiones** | Sondaje lineal (*linear probing*) |
| **Eliminación** | Eliminación lógica (*lazy deletion*) con reutilización de slots |
| **Redimensionamiento** | Rehashing automático al alcanzar un factor de carga ≥ 75% (duplica capacidad) |
| **Capacidad inicial** | 1009 (número primo para mejor distribución) |

La tabla almacena la clave (DNI numérico) junto con un puntero a `DatosAsistente` que contiene nombre, edad y hora de compra. La gestión de memoria dinámica es manual con `new`/`delete`.

### Max Heap — Gestión de prioridades

| Aspecto | Detalle |
|---|---|
| **Tipo** | Max-Heap binario basado en array estático |
| **Capacidad** | 1000 elementos (configurable vía `HEAP_MAX`) |
| **Operaciones clave** | `insertar`, `extraerMax`, `mostrarTop5`, `actualizarPrioridad` |
| **Indexación auxiliar** | Mapa DNI→posición para verificación rápida de duplicados y actualizaciones |

El heap organiza a los asistentes por prioridad descendente, permitiendo extraer al de mayor prioridad y mostrar los top 5 sin alterar la estructura original (mediante copia temporal).

### Árbol AVL — Análisis temporal y por zonas

| Aspecto | Detalle |
|---|---|
| **Clave de ordenamiento** | Hora de acceso (`string` en formato HH:MM, comparación lexicográfica) |
| **Datos por nodo** | Array de hasta 100 zonas con su contador de accesos |
| **Balanceo** | Rotaciones simples y dobles (LL, RR, LR, RL) |
| **Consultas** | Rango por franja horaria, zona con más accesos globales |

Cada nodo del árbol agrupa todos los accesos de una misma hora, desglosados por zona. Esto permite consultas eficientes por rangos de tiempo y estadísticas globales de ocupación.

---

## Estructura del Proyecto

```
Proyecto_EDA2-main/
├── crear_estructura.sh              # Script de scaffolding del proyecto
├── doc/                             # Documentación adicional
├── tests/                           # Tests del proyecto
└── src/
    ├── main/
    │   ├── main.cpp                 # Punto de entrada — menú interactivo
    │   └── usuarios_masivos.txt     # Dataset de prueba (50+ registros)
    └── modules/
        ├── Persona.h                # Modelo de datos compartido
        ├── hash_Johao/
        │   ├── hashTable.h          # Declaración de la tabla hash
        │   ├── hashTable.cpp        # Implementación de la tabla hash
        │   └── testHash.cpp         # Tests unitarios de la tabla hash
        ├── heap_JeanPaul/
        │   ├── Heap.h               # Declaración del Max-Heap
        │   └── Heap.cpp             # Implementación del Max-Heap
        └── tree_Rivera/
            ├── AVL.h                # Declaración del árbol AVL
            └── AVL.CPP              # Implementación del árbol AVL
```

---

## Funcionalidades

| # | Funcionalidad | Estructura | Descripción |
|---|---|---|---|
| 1 | Registro manual | Hash + Heap + AVL | Ingresa los datos de una persona de forma interactiva |
| 2 | Carga masiva | Hash + Heap + AVL | Lee registros desde `usuarios_masivos.txt` y los inserta en lote |
| 3 | Buscar por DNI | Hash Table | Recupera nombre, edad y hora de compra dado un DNI |
| 4 | Validar existencia | Hash Table | Verifica si un DNI ya está registrado en el sistema |
| 5 | Top 5 prioridades | Max Heap | Muestra los 5 asistentes con mayor prioridad sin extraerlos |
| 6 | Extraer mayor prioridad | Max Heap | Retira del heap al asistente con la prioridad más alta |
| 7 | Consultar por rango horario | Árbol AVL | Lista los accesos registrados entre dos horas dadas, desglosados por zona |
| 8 | Zona con más accesos | Árbol AVL | Determina qué zona acumula la mayor cantidad de accesos globalmente |

---

## Complejidad Algorítmica

| Operación | Estructura | Caso Promedio | Peor Caso |
|---|---|---|---|
| Inserción | Hash Table | $O(1)$ | $O(n)$ |
| Búsqueda por DNI | Hash Table | $O(1)$ | $O(n)$ |
| Eliminación | Hash Table | $O(1)$ | $O(n)$ |
| Inserción | Max Heap | $O(\log n)$ | $O(\log n)$ |
| Extracción del máximo | Max Heap | $O(\log n)$ | $O(\log n)$ |
| Top-K | Max Heap | $O(k \log n)$ | $O(k \log n)$ |
| Inserción | Árbol AVL | $O(\log n)$ | $O(\log n)$ |
| Consulta por rango | Árbol AVL | $O(\log n + m)$ | $O(\log n + m)$ |
| Zona con más accesos | Árbol AVL | $O(n)$ | $O(n)$ |

> Donde $n$ es el número de elementos y $m$ es la cantidad de resultados en el rango consultado.

---

## Requisitos

- **Compilador C++**: g++ 9.0+ (o cualquier compilador compatible con C++11)
- **Sistema Operativo**: Windows / Linux / macOS

---

## Compilación y Ejecución

### Compilación manual

```bash
g++ -std=c++11 -o sistema_accesos \
    src/main/main.cpp \
    src/modules/hash_Johao/hashTable.cpp \
    src/modules/heap_JeanPaul/Heap.cpp \
    src/modules/tree_Rivera/AVL.CPP
```

### Ejecución

```bash
./sistema_accesos
```

### Compilar y ejecutar tests de la tabla hash

```bash
g++ -std=c++11 -o test_hash \
    src/modules/hash_Johao/testHash.cpp \
    src/modules/hash_Johao/hashTable.cpp

./test_hash
```

---

## Ejemplo de Uso

```
--- SISTEMA DE CONTROL DE ACCESOS ---
1. Registrar acceso manual (individual)
2. Cargar accesos desde archivo (masivo)
3. Buscar persona por DNI (Hash)
4. Validar existencia por DNI (Hash)
5. Mostrar top 5 prioridades (Heap)
6. Extraer acceso de mayor prioridad (Heap)
7. Consultar accesos por zona y hora (AVL)
8. Mostrar zona con mas accesos (AVL)
0. Salir
Seleccione opcion: 2
Carga masiva completada desde archivo predefinido.

Seleccione opcion: 5
Top 5 prioridades:
72897271 - Francisco (Prioridad: 100)
72897270 - Gabriela (Prioridad: 99)
72897272 - Mariana (Prioridad: 99)
72897269 - Martin (Prioridad: 98)
72897273 - Pablo (Prioridad: 98)

Seleccione opcion: 3
Ingrese DNI: 72897246
Nombre: Bruno, Edad: 23, Hora: 08:00

Seleccione opcion: 7
Desde hora (HH:MM): 08:00
Hasta hora (HH:MM): 10:00
Hora: 08:00 | Zonas: 1(1)
Hora: 08:10 | Zonas: 2(1)
Hora: 08:20 | Zonas: 3(1)
...
```

---

## Datos de Prueba

El archivo `usuarios_masivos.txt` contiene más de 50 registros con el formato:

```
DNI Nombre Edad Zona Hora Prioridad
```

Ejemplo:
```
72897246 Bruno 23 1 08:00 90
75426441 Matias 21 2 09:15 15
72987654 Lucia 30 3 10:30 80
```

Los registros cubren 3 zonas, horarios de 08:00 a 14:40 y prioridades de 10 a 100, lo que permite probar todos los escenarios del sistema.

---

## Autores

| Integrante | Módulo |
|---|---|
| **Johao** | Hash Table — Registro y búsqueda por DNI |
| **Jean Paul** | Max Heap — Gestión de prioridades |
| **Rivera** | Árbol AVL — Análisis temporal y por zonas |
| **Bruno** | Integración del sistema (`main.cpp`) y testing |
| **Matías** | Integración del sistema (`main.cpp`) y testing |

Proyecto desarrollado como parte del curso de **Estructuras de Datos y Algoritmos II**.

---

## Licencia

Este proyecto fue desarrollado con fines académicos. Uso libre con atribución a los autores.
