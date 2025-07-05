#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <string>

// Estructura que contiene los datos detallados de un asistente.
struct DatosAsistente {
    std::string nombre;
    int edad;
    std::string horaCompra; // Formato "HH:MM"
};

// Representa un slot (entrada) individual dentro de la tabla hash.
struct Entrada {
    int clave; // DNI o código QR del asistente.
    DatosAsistente* valor; // Puntero a los datos del asistente. nullptr si el slot está vacío.
    bool ocupado; // True si el slot contiene una clave y datos válidos.
    bool eliminado; // True si el slot fue ocupado y borrado lógicamente (para sondaje).
};

// Implementa una Tabla Hash con sondaje lineal para control de accesos.
// Permite almacenar, buscar y eliminar asistentes por clave (DNI).
class HashTable {
private:
    Entrada* tabla; // Array dinámico de entradas de la tabla.
    int capacidad;  // Tamaño actual de la tabla.
    int cantidad;   // Número de elementos activos (no eliminados lógicamente).
    int ocupadosConEliminados; // Slots físicamente ocupados (incluye eliminados).

    // Calcula el índice hash para una clave, manejando valores negativos.
    int funcionHash(int clave);

    // Encuentra un slot adecuado para inserción/re-inserción usando sondaje lineal.
    // Puede reutilizar slots eliminados. Retorna el índice o -1/-2 en caso de error.
    int buscarSlotInsercion(int claveOriginal, int indiceInicial);

    // Redimensiona la tabla (duplica capacidad) y reubica todos los elementos activos.
    void rehash();

public:
    // Constructor: Inicializa la tabla con una capacidad dada.
    HashTable(int tam);

    // Destructor: Libera toda la memoria dinámica de la tabla y los DatosAsistente.
    ~HashTable();

    // Inserta un nuevo asistente en la tabla. Retorna true si es exitoso, false si la clave ya existe.
    bool insertar(int clave, const std::string& nombre, int edad, const std::string& horaCompra);

    // Busca un asistente por su clave. Retorna puntero a DatosAsistente o nullptr si no se encuentra.
    DatosAsistente* buscar(int clave);

    // Marca un asistente como lógicamente eliminado. Retorna true si es exitoso, false si no se encuentra.
    bool eliminar(int clave);

    // Imprime el estado actual de la tabla, mostrando sus elementos.
    void mostrar();

    // Retorna la capacidad actual de la tabla.
    int getCapacidad() const;

    // Retorna el número de elementos activos en la tabla.
    int getCantidad() const;
};

#endif