#include <iostream>
#include <string> // Necesario para std::string
#include "HashTable.h"

using namespace std;

// Constructor
HashTable::HashTable(int tam) {
    capacidad = tam;
    cantidad = 0;
    ocupadosConEliminados = 0; // Inicializar nuevo contador
    tabla = new Entrada[capacidad];

    for (int i = 0; i < capacidad; i++) {
        tabla[i].ocupado = false;
        tabla[i].eliminado = false; // Inicializar la nueva bandera
        tabla[i].valor = nullptr; // Inicializar puntero a null
    }
}

// Destructor
HashTable::~HashTable() {
    // Liberar memoria de cada DatosAsistente si fueron asignados dinámicamente
    for (int i = 0; i < capacidad; i++) {
        if (tabla[i].ocupado || tabla[i].eliminado) { // Si estuvo ocupado alguna vez
            delete tabla[i].valor;
            tabla[i].valor = nullptr;
        }
    }
    delete[] tabla;
}

// Función Hash (sin cambios, ya maneja negativos)
int HashTable::funcionHash(int clave) {
    int hash = clave % capacidad;
    return (hash < 0) ? hash + capacidad : hash;
}

// Renombrada y ajustada para encontrar un slot para inserción
// Considera slots eliminados como posibles para reusar
int HashTable::buscarSlotInsercion(int claveOriginal, int indiceInicial) {
    int i = 0;
    int indice = indiceInicial;
    int primerEliminado = -1; // Para recordar el primer slot eliminado encontrado

    while (i < capacidad) { // Cambiado a i < capacidad para evitar bucle infinito
        indice = (indiceInicial + i) % capacidad;

        if (!tabla[indice].ocupado) { // Si el slot está completamente vacío
            if (primerEliminado != -1) {
                return primerEliminado; // Preferimos usar un slot eliminado si encontramos uno antes
            }
            return indice; // Slot vacío encontrado
        }

        if (tabla[indice].eliminado) { // Si el slot está lógicamente eliminado
            if (primerEliminado == -1) {
                primerEliminado = indice; // Registramos el primer slot eliminado
            }
        } else if (tabla[indice].clave == claveOriginal) {
            // Ya existe la clave, esto debería ser manejado por el método insertar antes de llamar a esto.
            // Pero como fallback, podemos indicar que ya existe o lanzar una excepción.
            // Para este contexto, retornar -2 para indicar que ya existe.
            return -2;
        }
        i++;
    }

    if (primerEliminado != -1) {
        return primerEliminado; // Si no encontramos un slot vacío pero sí uno eliminado
    }

    return -1; // Tabla llena o no se encontró espacio disponible (esto debería ser manejado por rehash)
}


// Rehashing (cambios menores para manejar DatosAsistente y eliminado)
void HashTable::rehash() {
    int antiguaCapacidad = capacidad;
    int nuevaCapacidad = capacidad * 2;
    Entrada* nuevaTabla = new Entrada[nuevaCapacidad];

    for (int i = 0; i < nuevaCapacidad; i++) {
        nuevaTabla[i].ocupado = false;
        nuevaTabla[i].eliminado = false;
        nuevaTabla[i].valor = nullptr;
    }

    // Guardamos la tabla antigua para reinsertar
    Entrada* tablaAntigua = tabla;
    tabla = nuevaTabla; // La nueva tabla se convierte en la actual
    capacidad = nuevaCapacidad;
    cantidad = 0; // Resetear cantidad de elementos válidos
    ocupadosConEliminados = 0; // Resetear contador de slots físicamente ocupados

    cout << ">>> Rehashing iniciado. Nueva capacidad: " << capacidad << endl;

    // Reinserta elementos antiguos que NO estén eliminados lógicamente
    for (int i = 0; i < antiguaCapacidad; i++) {
        if (tablaAntigua[i].ocupado) { // Solo reinsertamos elementos VÁLIDOS
            int clave = tablaAntigua[i].clave;
            // Reusar insertar para manejar la inserción con sondaje en la nueva tabla
            // IMPORTANTE: Se crea una copia del valor. El original se borrará con tablaAntigua.
            insertar(clave, tablaAntigua[i].valor->nombre, tablaAntigua[i].valor->edad, tablaAntigua[i].valor->horaCompra);
        }
        // Liberar la memoria del valor de la entrada antigua
        delete tablaAntigua[i].valor;
        tablaAntigua[i].valor = nullptr;
    }

    delete[] tablaAntigua; // Liberar la memoria de la tabla antigua
    cout << ">>> Rehashing completado. Nueva capacidad: " << capacidad << endl;
}

// Inserción (ahora recibe nombre, edad, horaCompra)
bool HashTable::insertar(int clave, const std::string& nombre, int edad, const std::string& horaCompra) {
    if (buscar(clave) != nullptr) { // Usamos el nuevo buscar que retorna DatosAsistente*
        cout << "Error: La clave " << clave << " ya existe. No se puede insertar duplicado.\n";
        return false;
    }

    // Punto 7: Umbral de rehashing al 75%
    if (static_cast<double>(cantidad) / capacidad >= 0.75) {
        rehash();
    }

    int hash = funcionHash(clave);
    int indice = buscarSlotInsercion(clave, hash);

    if (indice == -1) { // Esto solo debería ocurrir si la tabla está completamente llena después de rehash,
                        // o si la función de sondaje no encontró espacio.
                        // Con el umbral de rehashing, esto es menos probable, pero es un buen fallback.
        cerr << "Error: No se encontró espacio para insertar la clave " << clave << " después de rehashing.\n";
        return false;
    }
    if (indice == -2) { // Clave ya existe (manejo de seguridad)
        cout << "Error interno: Clave " << clave << " ya existía al intentar insertar. Esto no debería pasar después de la verificación inicial.\n";
        return false;
    }

    // Si estamos reusando un slot "eliminado", no incrementamos ocupadosConEliminados
    if (!tabla[indice].ocupado && !tabla[indice].eliminado) {
        ocupadosConEliminados++; // Solo incrementa si es un slot realmente nuevo
    }

    tabla[indice].clave = clave;
    // Creamos una nueva instancia de DatosAsistente
    tabla[indice].valor = new DatosAsistente{nombre, edad, horaCompra};
    tabla[indice].ocupado = true;
    tabla[indice].eliminado = false; // Asegurarse de que no esté marcado como eliminado
    cantidad++; // Incrementa solo la cantidad de elementos válidos
    return true;
}

// Búsqueda (ahora retorna puntero a DatosAsistente)
DatosAsistente* HashTable::buscar(int clave) {
    int hash = funcionHash(clave);
    int i = 0;
    int indice = hash;

    while (i < capacidad) {
        indice = (hash + i) % capacidad;

        if (!tabla[indice].ocupado && !tabla[indice].eliminado) {
            // Slot vacío no marcado como eliminado -> la clave no existe
            return nullptr;
        }

        if (tabla[indice].ocupado && tabla[indice].clave == clave) {
            // Clave encontrada y no está lógicamente eliminada
            return tabla[indice].valor;
        }
        // Si el slot está ocupado pero con otra clave, o está eliminado, continuamos sondeando
        i++;
    }
    return nullptr; // Clave no encontrada después de recorrer toda la secuencia de sondaje
}

// Nuevo método de eliminación lógica (Punto 8)
bool HashTable::eliminar(int clave) {
    int hash = funcionHash(clave);
    int i = 0;
    int indice = hash;

    while (i < capacidad) {
        indice = (hash + i) % capacidad;

        if (!tabla[indice].ocupado && !tabla[indice].eliminado) {
            // Slot vacío y no eliminado -> la clave no existe
            return false;
        }

        if (tabla[indice].ocupado && tabla[indice].clave == clave) {
            // Clave encontrada y está ocupada (válida)
            tabla[indice].ocupado = false;   // Marcar como no ocupado
            tabla[indice].eliminado = true;  // Marcar como lógicamente eliminado
            delete tabla[indice].valor;      // Liberar memoria de los datos del asistente
            tabla[indice].valor = nullptr;   // Poner el puntero a null
            cantidad--;                      // Decrementar la cantidad de elementos válidos
            cout << "Clave " << clave << " eliminada lógicamente." << endl;
            return true;
        }
        i++;
    }
    return false; // Clave no encontrada
}

// Mostrar (Punto 9: Mostrar todos los datos)
void HashTable::mostrar() {
    cout << "\n--- Tabla Hash --- (Capacidad: " << capacidad << ", Elementos Activos: " << cantidad << ", Slots Ocupados (físicos): " << ocupadosConEliminados << ")\n";
    for (int i = 0; i < capacidad; i++) {
        cout << "[" << i << "] ";
        if (tabla[i].ocupado) {
            cout << "-> Clave: " << tabla[i].clave
                 << ", Nombre: " << tabla[i].valor->nombre
                 << ", Edad: " << tabla[i].valor->edad
                 << ", Hora Compra: " << tabla[i].valor->horaCompra
                 << " (OCUPADO)" << endl;
        } else if (tabla[i].eliminado) {
            cout << "-> (ELIMINADO LOGICAMENTE)" << endl;
        } else {
            cout << "-> (VACÍO)" << endl;
        }
    }
    cout << "-------------------\n";
}