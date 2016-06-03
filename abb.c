#include <stdio.h>
#include <stdlib.h>

typedef struct polinomio {
    int tam;
    int pos;
    unsigned int *exponente;
    int *coeficiente;
} pol;

typedef struct node {
    pol valor_polinomio;
    struct node *izq;
    struct node *der;

} tNodo;

typedef struct abb {
    tNodo *raiz;
    int Size;
} tABB;

int crear_hijos(tNodo *base, unsigned int cant_hijos){
    if (cant_hijos == 0 ){
        base->izq = NULL;
        base->der = NULL;
        return 1;
    }
    base->izq = (tNodo *)malloc(sizeof(tNodo));
    cant_hijos = cant_hijos/2-1;
    base->der = (tNodo *)malloc(sizeof(tNodo));
    crear_hijos(base->izq, cant_hijos);
    crear_hijos(base->der, cant_hijos);
}

unsigned int power(int x, unsigned int y)
{
    if( y == 0)
        return 1;
    else if (y%2 == 0)
        return power(x, y/2)*power(x, y/2);
    else
        return x*power(x, y/2)*power(x, y/2);

}

tABB initialize(int cantPol){
    tABB tree;
    tree.raiz = (tNodo *)malloc(sizeof(tNodo));
    int i;
    unsigned int h = 0;
    unsigned int nodos = 0;
    while (cantPol > nodos){
        nodos += power(2, h++);
    }
    crear_hijos(tree.raiz, nodos-1);
    return tree;
}

void asignar(tNodo *base, pol polinom, unsigned int half){
    if (half == polinom.pos){
        base->valor_polinomio = polinom;
        return;
    }
    else {
        if (polinom.pos < half) {
            half /= 2;
            asignar(base->izq, polinom, half);
        }
        else {
            if (half == 1){
                half+=1;
            }
            else {
                half += half / 2;
            }
            asignar(base->der, polinom, half);
        }
    }
}

void limpiar(tNodo *base){
    if (base->izq != NULL) {
        if (base->izq->valor_polinomio.tam == 0) {
            tNodo *aux;
            aux = base->izq;
            base->izq = NULL;
            free(aux);
        }
        else {
            limpiar(base->izq);
        }
    }
    if (base->der != NULL) {
        if (base->der->valor_polinomio.tam == 0) {
            tNodo *aux;
            aux = base->der;
            base->der = NULL;
            free(aux);
        }
        else {
            limpiar(base->der);
        }
    }
}



void main(){
    FILE *archivo = fopen("entradaPolinomio.txt", "r");
    int cant_pol;
    int i, tam_pol, j;
    unsigned int exp, coef, half;
    fscanf(archivo, "%d", &cant_pol);
    tABB arbol;
    arbol = initialize(cant_pol);
    arbol.Size = cant_pol;
    for (i=0; i<cant_pol; i++){
        fscanf(archivo, "%d", &tam_pol);
        pol polinom;
        polinom.tam = tam_pol;
        polinom.pos = i;
        polinom.exponente = (unsigned int *)malloc(sizeof(unsigned int)* tam_pol);
        polinom.coeficiente = (int *)malloc(sizeof(int)* tam_pol);
        for (j=0; j<tam_pol; j++){
            fscanf(archivo, "%u %u", &exp, &coef);
            polinom.exponente[j] = exp;
            polinom.coeficiente[j] = coef;
        }
        half = (unsigned int) (cant_pol / 2);
        asignar(arbol.raiz, polinom, half);
    }
    limpiar(arbol.raiz);
    fclose(archivo);
}