#include <iostream>
#include <string>
#include "HashTable.h"

using namespace std;

// Constructor: Inicializa la tabla hash.
// Asigna memoria para el array de entradas y configura cada slot como vacío.
HashTable::HashTable(int tam) {
    capacidad = tam;
    cantidad = 0; // Número inicial de elementos válidos.
    ocupadosConEliminados = 0; // Número inicial de slots físicamente ocupados.
    tabla = new Entrada[capacidad]; // Asignación dinámica de memoria para la tabla.

    // Inicializa cada slot de la tabla.
    for (int i = 0; i < capacidad; i++) {
        tabla[i].ocupado = false;   // Marca el slot como vacío.
        tabla[i].eliminado = false; // Marca el slot como no lógicamente eliminado.
        tabla[i].valor = nullptr;   // Inicializa el puntero a datos del asistente como nulo.
    }
}

// Destructor: Libera toda la memoria dinámica asignada por la tabla hash.
// Iterar sobre la tabla para liberar la memoria de cada objeto DatosAsistente
// y luego liberar el array principal de entradas.
HashTable::~HashTable() {
    for (int i = 0; i < capacidad; i++) {
        // Si el slot fue ocupado en algún momento (activo o lógicamente eliminado),
        // su 'valor' fue asignado dinámicamente y debe ser liberado.
        if (tabla[i].valor != nullptr) {
            delete tabla[i].valor;
            tabla[i].valor = nullptr;
        }
    }
    delete[] tabla; // Libera el array de entradas de la tabla.
}

// Función Hash: Calcula el índice inicial para una clave.
// Utiliza el operador módulo y ajusta para claves negativas.
int HashTable::funcionHash(int clave) {
    int hash = clave % capacidad;
    // Asegura que el índice hash sea no negativo.
    return (hash < 0) ? hash + capacidad : hash;
}

// Busca un slot adecuado para la inserción usando sondaje lineal.
// Puede reutilizar slots marcados como eliminados lógicamente para optimizar espacio.
int HashTable::buscarSlotInsercion(int claveOriginal, int indiceInicial) {
    int i = 0;
    int indice = indiceInicial;
    int primerEliminado = -1; // Almacena el primer índice de un slot lógicamente eliminado encontrado.

    while (i < capacidad) {
        indice = (indiceInicial + i) % capacidad; // Aplica sondaje lineal.

        if (!tabla[indice].ocupado) { // Si el slot está completamente vacío
            // Si encontramos un slot eliminado previamente, lo reutilizamos antes de un slot totalmente vacío.
            if (primerEliminado != -1) {
                return primerEliminado;
            }
            return indice; // Se encontró un slot vacío y no había eliminados previos.
        }

        if (tabla[indice].eliminado) { // Si el slot está lógicamente eliminado
            if (primerEliminado == -1) {
                primerEliminado = indice; // Guarda el índice del primer slot eliminado.
            }
        } else if (tabla[indice].clave == claveOriginal) {
            // La clave ya existe en un slot ocupado válido.
            // Esto es un caso de seguridad, ya que 'insertar' ya verifica duplicados.
            return -2; // Código especial para indicar que la clave ya existe.
        }
        i++; // Avanza al siguiente slot en la secuencia de sondaje.
    }

    // Si se recorrió toda la secuencia de sondaje y solo se encontraron slots eliminados o llenos,
    // se devuelve el primer slot eliminado encontrado (si lo hay).
    if (primerEliminado != -1) {
        return primerEliminado;
    }

    // No se encontró ningún slot disponible (la tabla está efectivamente llena para esta secuencia).
    return -1;
}

// Duplica la capacidad de la tabla y reinserta todos los elementos válidos.
// Este proceso se realiza para mantener la eficiencia de las operaciones.
void HashTable::rehash() {
    int antiguaCapacidad = capacidad;
    Entrada* tablaAntigua = tabla; // Guarda la referencia a la tabla actual.

    capacidad *= 2; // Duplica la capacidad.
    tabla = new Entrada[capacidad]; // Asigna nueva memoria para la tabla más grande.
    cantidad = 0; // Reinicia el contador de elementos válidos para la nueva tabla.
    ocupadosConEliminados = 0; // Reinicia el contador de slots físicamente ocupados.

    // Inicializa todos los nuevos slots como vacíos.
    for (int i = 0; i < capacidad; i++) {
        tabla[i].ocupado = false;
        tabla[i].eliminado = false;
        tabla[i].valor = nullptr;
    }

    cout << ">>> Rehashing iniciado. Nueva capacidad: " << capacidad << endl;

    // Reinserta solo los elementos que estaban activos (no eliminados lógicamente)
    // de la tabla antigua a la nueva tabla.
    for (int i = 0; i < antiguaCapacidad; i++) {
        if (tablaAntigua[i].ocupado) {
            // Llama al método insertar para reubicar la clave y sus datos en la nueva tabla.
            // Se crea una nueva copia de DatosAsistente en la nueva tabla.
            insertar(tablaAntigua[i].clave, tablaAntigua[i].valor->nombre,
                     tablaAntigua[i].valor->edad, tablaAntigua[i].valor->horaCompra);
        }
        // Libera la memoria de los DatosAsistente de la entrada antigua.
        if (tablaAntigua[i].valor != nullptr) {
            delete tablaAntigua[i].valor;
            tablaAntigua[i].valor = nullptr;
        }
    }

    delete[] tablaAntigua; // Libera la memoria del array de la tabla antigua.
    cout << ">>> Rehashing completado. Nueva capacidad: " << capacidad << endl;
}

// Inserta una nueva clave y sus datos asociados en la tabla hash.
// Retorna true si la inserción es exitosa, false si la clave ya existe.
bool HashTable::insertar(int clave, const std::string& nombre, int edad, const std::string& horaCompra) {
    // Verifica si la clave ya existe para evitar duplicados.
    if (buscar(clave) != nullptr) {
        cout << "Error: La clave " << clave << " ya existe. No se puede insertar duplicado.\n";
        return false;
    }

    // Activa el rehashing si el factor de carga (elementos activos / capacidad)
    // alcanza o supera el 75%.
    if (static_cast<double>(cantidad) / capacidad >= 0.75) {
        rehash();
    }

    int hash = funcionHash(clave); // Calcula el índice hash inicial.
    int indice = buscarSlotInsercion(clave, hash); // Encuentra el slot de inserción.

    // Manejo de errores si no se encuentra un slot válido para insertar.
    if (indice == -1) {
        cerr << "Error: No se encontró espacio para insertar la clave " << clave << ".\n";
        return false;
    }
    if (indice == -2) { // Este caso solo debería ocurrir si hay un error lógico en el flujo.
        cout << "Error interno: Clave " << clave << " ya existía al intentar insertar.\n";
        return false;
    }

    // Incrementa el contador de slots físicamente ocupados solo si es un slot nuevo y no reutilizado.
    if (!tabla[indice].ocupado && !tabla[indice].eliminado) {
        ocupadosConEliminados++;
    }

    tabla[indice].clave = clave; // Almacena la clave.
    // Crea dinámicamente un objeto DatosAsistente y almacena el puntero.
    tabla[indice].valor = new DatosAsistente{nombre, edad, horaCompra};
    tabla[indice].ocupado = true;   // Marca el slot como ocupado.
    tabla[indice].eliminado = false; // Asegura que no esté marcado como eliminado.
    cantidad++; // Incrementa el contador de elementos válidos.
    return true;
}

// Busca una clave en la tabla hash.
// Retorna un puntero a los DatosAsistente asociados si la clave es encontrada,
// o nullptr si la clave no existe o está lógicamente eliminada.
DatosAsistente* HashTable::buscar(int clave) {
    int hash = funcionHash(clave); // Calcula el índice hash inicial.
    int i = 0;
    int indice = hash;

    while (i < capacidad) {
        indice = (hash + i) % capacidad; // Sondaje lineal.

        // Si el slot está completamente vacío (no ocupado y no eliminado),
        // la clave no puede existir más adelante en esta secuencia de sondaje.
        if (!tabla[indice].ocupado && !tabla[indice].eliminado) {
            return nullptr;
        }

        // Si el slot está ocupado y la clave coincide, se encontró el elemento.
        if (tabla[indice].ocupado && tabla[indice].clave == clave) {
            return tabla[indice].valor; // Retorna los datos asociados.
        }
        // Si el slot está ocupado con otra clave, o está lógicamente eliminado,
        // se continúa sondeando.
        i++;
    }
    return nullptr; // La clave no fue encontrada después de recorrer toda la secuencia.
}

// Marca una entrada como lógicamente eliminada.
// Esto permite que el slot sea potencialmente reutilizado en futuras inserciones
// sin romper las secuencias de sondaje existentes.
bool HashTable::eliminar(int clave) {
    int hash = funcionHash(clave); // Calcula el índice hash inicial.
    int i = 0;
    int indice = hash;

    while (i < capacidad) {
        indice = (hash + i) % capacidad; // Sondaje lineal.

        // Si el slot está completamente vacío, la clave no puede existir.
        if (!tabla[indice].ocupado && !tabla[indice].eliminado) {
            return false;
        }

        // Si la clave coincide y está ocupada (válida).
        if (tabla[indice].ocupado && tabla[indice].clave == clave) {
            tabla[indice].ocupado = false;   // Marca el slot como no activo.
            tabla[indice].eliminado = true;  // Marca el slot como lógicamente eliminado.
            // Libera la memoria de los datos del asistente.
            if (tabla[indice].valor != nullptr) {
                delete tabla[indice].valor;
                tabla[indice].valor = nullptr;
            }
            cantidad--; // Decrementa el contador de elementos válidos.
            cout << "Clave " << clave << " eliminada lógicamente." << endl;
            return true;
        }
        i++; // Continúa sondeando.
    }
    return false; // La clave no fue encontrada para eliminar.
}

// Muestra el estado actual de la tabla hash.
// Incluye la capacidad, el número de elementos activos y el detalle de cada slot.
void HashTable::mostrar() {
    cout << "\n--- Estado Actual de la Tabla Hash ---\n";
    cout << "Capacidad Total: " << capacidad
         << ", Elementos Activos: " << cantidad
         << ", Slots Ocupados (físicos, incl. eliminados): " << ocupadosConEliminados << "\n";
    cout << "--------------------------------------\n";

    for (int i = 0; i < capacidad; i++) {
        cout << "[" << i << "] ";
        if (tabla[i].ocupado) {
            // Muestra los detalles completos del asistente si el slot está ocupado.
            cout << "-> Clave: " << tabla[i].clave
                 << ", Nombre: " << tabla[i].valor->nombre
                 << ", Edad: " << tabla[i].valor->edad
                 << ", Hora Compra: " << tabla[i].valor->horaCompra
                 << " (ACTIVO)" << endl;
        } else if (tabla[i].eliminado) {
            // Indica que el slot está lógicamente eliminado.
            cout << "-> (ELIMINADO LOGICAMENTE)" << endl;
        } else {
            // Indica que el slot está completamente vacío.
            cout << "-> (VACÍO)" << endl;
        }
    }
    cout << "--------------------------------------\n";
}

// Getter para la capacidad actual de la tabla.
int HashTable::getCapacidad() const {
    return capacidad;
}

// Getter para la cantidad de elementos válidos en la tabla.
int HashTable::getCantidad() const {
    return cantidad;
}