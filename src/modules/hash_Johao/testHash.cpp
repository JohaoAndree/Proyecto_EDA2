#include "HashTable.h"
#include <iostream>

using namespace std;

int main() {
    cout << "--- Inicio de Test de HashTable ---" << endl;

    HashTable tabla(4); // Pequeña a propósito para forzar rehash

    // Pruebas de inserción con datos completos
    cout << "\n--- Insertando elementos ---" << endl;
    tabla.insertar(12345678, "Juan Perez", 30, "10:00");
    tabla.mostrar(); // Muestra el estado después de cada inserción (opcional, para depurar)

    tabla.insertar(23456789, "Maria Lopez", 25, "10:15");
    tabla.mostrar();

    tabla.insertar(-111111, "Carlos Gomez", 40, "10:30");
    tabla.mostrar();

    cout << "\nIntentando insertar un elemento que forzará rehash (capacidad actual: " << tabla.getCapacidad() << ", elementos: " << tabla.getCantidad() << ")" << endl;
    tabla.insertar(20202020, "Ana Garcia", 35, "10:45"); // Esto debería forzar el rehash
    tabla.mostrar(); // Mostrar después del rehash

    tabla.insertar(99999999, "Luis Ramirez", 28, "11:00");
    tabla.mostrar();

    // Prueba de búsqueda
    cout << "\n--- Pruebas de Busqueda ---" << endl;
    int claveBuscar1 = 23456789;
    DatosAsistente* datos1 = tabla.buscar(claveBuscar1);
    if (datos1 != nullptr) {
        cout << "Clave " << claveBuscar1 << " encontrada: Nombre: " << datos1->nombre
             << ", Edad: " << datos1->edad << ", Hora: " << datos1->horaCompra << endl;
    } else {
        cout << "Clave " << claveBuscar1 << " NO encontrada." << endl;
    }

    int claveBuscar2 = 555555;
    DatosAsistente* datos2 = tabla.buscar(claveBuscar2);
    if (datos2 != nullptr) {
        cout << "Clave " << claveBuscar2 << " encontrada: Nombre: " << datos2->nombre << endl;
    } else {
        cout << "Clave " << claveBuscar2 << " NO encontrada." << endl;
    }

    // Prueba de duplicados
    cout << "\n--- Prueba de Duplicados ---" << endl;
    tabla.insertar(12345678, "Duplicado Juan", 99, "00:00"); // Debería fallar

    // Pruebas de eliminación
    cout << "\n--- Pruebas de Eliminacion ---" << endl;
    tabla.eliminar(-111111); // Eliminar un elemento
    tabla.mostrar(); // Mostrar después de la eliminación

    cout << "\nIntentando buscar clave eliminada (-111111):" << endl;
    if (tabla.buscar(-111111) == nullptr) {
        cout << "Clave -111111 correctamente no encontrada después de eliminar." << endl;
    } else {
        cout << "ERROR: Clave -111111 encontrada después de eliminar." << endl;
    }

    cout << "\nIntentando eliminar una clave que no existe (00000000):" << endl;
    tabla.eliminar(00000000); // Eliminar uno que no existe

    // Prueba de inserción después de eliminación para reusar el slot
    cout << "\n--- Insertando para reusar slot eliminado ---" << endl;
    // Esto podría reusar el slot -111111 si su posición de hash/sondaje lo permite
    // (o el primer slot eliminado que encuentre en su secuencia de sondaje).
    tabla.insertar(10101010, "Nuevo Asistente", 22, "12:00");
    tabla.mostrar();


    // Obtener la capacidad actual para el mensaje final
    cout << "\n--- Fin de Test de HashTable ---" << endl;
    cout << "Capacidad final de la tabla: " << tabla.getCapacidad() << endl;
    cout << "Cantidad final de elementos activos: " << tabla.getCantidad() << endl;

    return 0;
}