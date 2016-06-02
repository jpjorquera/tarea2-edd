#include <stdio.h>
#include <stdlib.h>

typedef struct polinomio {
    int tam;
    unsigned int *exponente;
    int *coeficiente;
} pol;

typedef struct node {
    pol valor_polinomio;
    struct node *next;
} tNodo;

typedef struct lista {
    tNodo *head;
    tNodo *tail;
    tNodo *curr;
    unsigned int listSize;
    unsigned int pos;
} tLista;


tLista initialize(tLista lista){
    lista.head = lista.tail = lista.curr = (tNodo *)malloc(sizeof(tNodo));
    lista.head->next = NULL;
    lista.listSize = 0;
    lista.pos = 0;
    return lista;
}

void moveToStart(tLista lista) {
    lista.curr = lista.head; lista.pos = 0;
}

tLista insert(tLista lista, pol polinomio) {
    if (lista.head == lista.curr){

    }
    /*tNodo *aux = lista.curr->next;
    lista.curr->next = (tNodo *)malloc(sizeof(tNodo));
    lista.curr->next->valor_polinomio = polinomio;
    lista.curr->next->next = aux;
    if (lista.curr == lista.tail) lista.tail = lista.curr->next;
    lista.listSize++;*/
    return lista;
}

void main(){
    FILE *archivo = fopen("entradaPolinomio.txt", "r");
    int cant_pol;
    int i, tam_pol, j, k;
    unsigned int exp, coef;
    fscanf(archivo, "%d", &cant_pol);
    tLista lista_pol;
    lista_pol = initialize(lista_pol);

    unsigned int mayor_exp;
    int mayor_coef;
    unsigned int *mayores_exp = (unsigned int *)malloc(sizeof(unsigned int)*cant_pol);
    int *mayores_coef = (int *)malloc(sizeof(int)*cant_pol);

    for (i=0; i<cant_pol; i++){
        fscanf(archivo, "%d", &tam_pol);
        pol polinom;
        polinom.tam = tam_pol;
        polinom.exponente = (unsigned int *)malloc(sizeof(unsigned int)* tam_pol);
        polinom.coeficiente = (int *)malloc(sizeof(int)* tam_pol);
        mayor_exp = 0;
        mayor_coef = 0;
        for (j=0; j<tam_pol; j++){
            fscanf(archivo, "%u %u", &exp, &coef);
            polinom.exponente[j] = exp;
            polinom.coeficiente[j] = coef;
            if (exp > mayor_exp){
                mayor_exp = exp;
                mayor_coef = coef;
            }
            else if (exp == mayor_exp){
                if (coef > mayor_coef){
                    mayor_coef = coef;
                }
            }
        }
        if (i==0) {
            lista_pol.head->valor_polinomio = polinom;
            lista_pol.head->next = (tNodo *)malloc(sizeof(tNodo));
            lista_pol.head->next = NULL;
        }
        else {
            k = 0;
            tNodo *aux;
            aux = lista_pol.head;
            while (aux->next != NULL){
                aux = aux->next;
                k++;
            }
            aux->next = (tNodo *)malloc(sizeof(tNodo));
            aux->next->valor_polinomio = polinom;
            aux->next->next = (tNodo *)malloc(sizeof(tNodo));
            aux->next->next = NULL;
        }
    }
    fclose(archivo);
}
