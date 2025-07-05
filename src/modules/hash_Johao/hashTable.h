#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <string> // Necesario para std::string

// Estructura para almacenar los datos de la persona/asistente
struct DatosAsistente {
    std::string nombre;
    int edad;
    std::string horaCompra;
};

struct Entrada {
    int clave; // DNI
    DatosAsistente* valor; // Puntero a los datos asociados. Será nullptr si el slot está vacío o eliminado lógicamente.
    bool ocupado; // Indica si el slot tiene una clave VÁLIDA
    bool eliminado; // Indica si el slot fue ocupado y ahora está lógicamente vacío (para borrado)
};

class HashTable {
private:
    Entrada* tabla;
    int capacidad;
    int cantidad; // Cantidad de elementos actualmente válidos (no eliminados)
    int ocupadosConEliminados; // Cantidad de slots físicamente ocupados (incluye eliminados lógicamente)

    int funcionHash(int clave);
    int buscarSlotInsercion(int claveOriginal, int indiceInicial);
    void rehash();

public:
    HashTable(int tam);
    ~HashTable();

    // Ahora insertar recibe los datos del asistente
    bool insertar(int clave, const std::string& nombre, int edad, const std::string& horaCompra);
    // Buscar ahora podría retornar los datos del asistente si lo encuentra
    DatosAsistente* buscar(int clave); // Retorna puntero a DatosAsistente o nullptr
    bool eliminar(int clave); // Nuevo método para eliminación lógica
    void mostrar();

    // Agregamos un getter para la capacidad para el testeo
    int getCapacidad() const { return capacidad; }
    int getCantidad() const { return cantidad; } // Puedes agregar este también para ver cuántos elementos "activos" hay
};

#endif