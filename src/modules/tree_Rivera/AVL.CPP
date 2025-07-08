#include "avl.h"
#include <iostream>
#include <algorithm>
using namespace std;

AVLTree::AVLTree() {
    raiz = nullptr;
}

int AVLTree::altura(NodoAVL* nodo) {
    return nodo ? nodo->altura : 0;
}

int AVLTree::balance(NodoAVL* nodo) {
    return nodo ? altura(nodo->izq) - altura(nodo->der) : 0;
}

NodoAVL* AVLTree::rotarDer(NodoAVL* y) {
    NodoAVL* x = y->izq;
    NodoAVL* T2 = x->der;
    x->der = y;
    y->izq = T2;
    y->altura = max(altura(y->izq), altura(y->der)) + 1;
    x->altura = max(altura(x->izq), altura(x->der)) + 1;
    return x;
}

NodoAVL* AVLTree::rotarIzq(NodoAVL* x) {
    NodoAVL* y = x->der;
    NodoAVL* T2 = y->izq;
    y->izq = x;
    x->der = T2;
    x->altura = max(altura(x->izq), altura(x->der)) + 1;
    y->altura = max(altura(y->izq), altura(y->der)) + 1;
    return y;
}

void AVLTree::agregarZona(NodoAVL* nodo, int zona) {
    for (int i = 0; i < nodo->numZonas; ++i) {
        if (nodo->accesosPorZona[i].zona == zona) {
            nodo->accesosPorZona[i].cantidad++;
            return;
        }
    }
    if (nodo->numZonas < MAX_ZONAS) {
        nodo->accesosPorZona[nodo->numZonas].zona = zona;
        nodo->accesosPorZona[nodo->numZonas].cantidad = 1;
        nodo->numZonas++;
    }
}

NodoAVL* AVLTree::insertar(NodoAVL* nodo, const string& hora, int zona) {
    if (!nodo) {
        NodoAVL* nuevo = new NodoAVL(hora);
        nuevo->accesosPorZona[0].zona = zona;
        nuevo->accesosPorZona[0].cantidad = 1;
        nuevo->numZonas = 1;
        return nuevo;
    }
    if (hora < nodo->hora)
        nodo->izq = insertar(nodo->izq, hora, zona);
    else if (hora > nodo->hora)
        nodo->der = insertar(nodo->der, hora, zona);
    else {
        agregarZona(nodo, zona);
        return nodo;
    }
    nodo->altura = 1 + max(altura(nodo->izq), altura(nodo->der));
    int b = balance(nodo);
    if (b > 1 && hora < nodo->izq->hora) return rotarDer(nodo);
    if (b < -1 && hora > nodo->der->hora) return rotarIzq(nodo);
    if (b > 1 && hora > nodo->izq->hora) {
        nodo->izq = rotarIzq(nodo->izq);
        return rotarDer(nodo);
    }
    if (b < -1 && hora < nodo->der->hora) {
        nodo->der = rotarDer(nodo->der);
        return rotarIzq(nodo);
    }
    return nodo;
}

void AVLTree::insertar(const string& hora, int zona) {
    raiz = insertar(raiz, hora, zona);
}

void AVLTree::mostrarRango(NodoAVL* nodo, const string& desde, const string& hasta) {
    if (!nodo) return;
    if (nodo->hora > desde) mostrarRango(nodo->izq, desde, hasta);
    if (nodo->hora >= desde && nodo->hora <= hasta) {
        cout << "Hora: " << nodo->hora << " | Zonas: ";
        for (int i = 0; i < nodo->numZonas; ++i) {
            cout << nodo->accesosPorZona[i].zona << "(" << nodo->accesosPorZona[i].cantidad << ") ";
        }
        cout << endl;
    }
    if (nodo->hora < hasta) mostrarRango(nodo->der, desde, hasta);
}

void AVLTree::consultarRango(const string& desde, const string& hasta) {
    mostrarRango(raiz, desde, hasta);
}

void AVLTree::acumularZonas(NodoAVL* nodo, int* totalZonas, int& maxZona, int& maxCantidad) {
    if (!nodo) return;
    for (int i = 0; i < nodo->numZonas; ++i) {
        int z = nodo->accesosPorZona[i].zona;
        totalZonas[z] += nodo->accesosPorZona[i].cantidad;
        if (totalZonas[z] > maxCantidad) {
            maxCantidad = totalZonas[z];
            maxZona = z;
        }
    }
    acumularZonas(nodo->izq, totalZonas, maxZona, maxCantidad);
    acumularZonas(nodo->der, totalZonas, maxZona, maxCantidad);
}

int AVLTree::zonaConMasAccesos() {
    int totalZonas[MAX_ZONAS] = {0};
    int maxZona = -1, maxCantidad = 0;
    acumularZonas(raiz, totalZonas, maxZona, maxCantidad);
    return maxZona;
}

void AVLTree::inOrder(NodoAVL* nodo) {
    if (!nodo) return;
    inOrder(nodo->izq);
    cout << "Hora: " << nodo->hora << " | Zonas: ";
    for (int i = 0; i < nodo->numZonas; ++i) {
        cout << nodo->accesosPorZona[i].zona << "(" << nodo->accesosPorZona[i].cantidad << ") ";
    }
    cout << endl;
    inOrder(nodo->der);
}

void AVLTree::mostrar() {
    inOrder(raiz);
}
