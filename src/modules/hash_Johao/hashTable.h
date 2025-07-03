#ifndef HASHTABLE_H
#define HASHTABLE_H

struct Entrada {
    int clave;
    bool ocupado;
};

class HashTable {
private:
    Entrada* tabla;
    int capacidad;
    int cantidad;

    int funcionHash(int clave);
    int resolverColision(int clave);
    void rehash();

public:
    HashTable(int tam);
    ~HashTable();

    bool insertar(int clave);
    bool buscar(int clave);
    void mostrar();
};

#endif