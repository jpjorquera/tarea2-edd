#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct polinomio {          //Estructura polinomio
    int tam;                        // Almacena tamano, exponentes y coeficientes del polinomio
    unsigned int *exponente;
    int *coeficiente;
} pol;

typedef struct node {               //Estructura nodo de lista enlazada
    pol valor_polinomio;            //Lleva el polinomio como su valor
    struct node *next;
} tNodo;

typedef struct lista {              //Estructura lista
    tNodo *head;                    //Almacena la cabeza de la lista enlazada
} tLista;

/*****
* tLista initialize
******
* Inicialize la lista segun su head
******
* Input:
* tLista lista : lista enlazada que se requiere inicializar
******
* Returns:
* tlista, Devuelve la lista ya inicializada
*****/
tLista initialize(tLista lista){
    lista.head = (tNodo *)malloc(sizeof(tNodo));        //Crear cabeza
    lista.head->next = NULL;
    return lista;
}

/*****
* void free_linked
******
* Libera la memoria utilizada por la lista enlazada
******
* Input:
* tNodo * base : puntero del nodo actual que se quiere liberar
******
* Returns:
* void
*****/
void free_linked(tNodo *base){
    if (base->next == NULL){                            //Caso base, final de lista
        free(base->valor_polinomio.coeficiente);
        free(base->valor_polinomio.exponente);
        free(base);
        return;
    }
    tNodo *auxiliar = base;
    base = base->next;                                  //Apuntar siguiente
    free(auxiliar->valor_polinomio.coeficiente);        //liberar auxiliar
    free(auxiliar->valor_polinomio.exponente);
    free(auxiliar);
    free_linked(base->next);
}

tNodo *movetopost(tLista lista, int posicion){
    int i;
    if (posicion < 0) return;
    tNodo *auxiliar = (tNodo *)malloc(sizeof(tNodo *));
    auxiliar = lista.head;
    int pos = 0;
    for (i = 0; i < posicion; i++) {
        auxiliar = auxiliar->next;
        pos++;
    }
    return auxiliar;
}

void intercambiar (int * A, int i, int j){
    int tmp = A[i];
    A[i] = A[j];
    A[j] = tmp;
}

void ordenarpol (int * A, int * B , int N, int arreglo[][2], int * C){
    int i, j, k;
    for (i = 0; i < N - 1; i++)
    {
        for (k = i, j = i + 1; j < N; j++)
            if (A[j] > A[k])
                k = j;
        if (k != i)
            intercambiar (A, i, k);
    }
    for (i = 0; i < N ; i++){
        arreglo[i][0]= A[i];
        for (j = 0; j < N ; j++){
            if(arreglo[i][0]==C[j]){
                arreglo[i][1]=B[j];
            }
        }
    }
}


float evaluar(tLista lista, int posicion, float evaluado){
/*void evaluar(int posicion, int evaluado){*/
    tNodo *act= movetopost(lista, posicion);
    pol elemento = act->valor_polinomio;
    int exponen[elemento.tam];
    int exponen2[elemento.tam];
    int coee[elemento.tam];
    int z;
    for (z=0;z<elemento.tam;z++){
    	exponen[z]=elemento.exponente[z];
    	exponen2[z]=elemento.exponente[z];
    	coee[z]=elemento.coeficiente[z];
    }
    int tamanoo= sizeof(exponen)/4;
    int elem[tamanoo][2];
    ordenarpol(exponen2, coee, tamanoo, elem, exponen);
    float raiz = 0;
    float coef = elem[0][1];
    float poli = coef + raiz;
    int i=1;
    for(i;i<sizeof(elem)/8;i++){
        int a = elem[i-1][0]-elem[i][0];
        if(a!=1){
            while(a>1){
                raiz = poli*evaluado;
                coef = 0;
                poli = coef + raiz;
                a--;
            }
        }
        if (a==1){
            raiz = poli*evaluado;
            coef = elem[i][1];
            poli = coef + raiz;
        }
    }
    int cortar = 1000000*poli;
	float mandar = cortar/1000000;
    return poli;
}


int coeficiente(tLista lista, int pos, float expo){
/*int coeficiente(int pos, int expo){*/
    tNodo *act= movetopost(lista, pos);
    pol elemento = act->valor_polinomio;
    /*int elem[3][2]={{3,345},{1,3},{0,-3}};*/
    int i=0;
    for(i;i<sizeof(elemento.exponente)/4;i++){
        if(elemento.exponente[i]==expo){
            float res = elemento.coeficiente[i];
            return res;
        }
    }
    return 0;
}

/*void movetopost(tLista lista, int posicion){
    int i;
    tNodo curr;
    if (posicion < 0 || posicion > lista.listSize) return;
    lista.curr = lista.head;
    pos = 0;
    for (i = 0; i < posicion; i++) {
        lista.curr = lista.curr->next;
        pos++;
    }
}*/


void main(){
    FILE *archivo = fopen("entradaPolinomio.txt", "r");             //Entrada
    int cant_pol;
    int i, tam_pol, j, k, coef;
    unsigned int exp;
    fscanf(archivo, "%d", &cant_pol);                               //Almacenar cantidad de polinomios
    tLista lista_pol;
    lista_pol = initialize(lista_pol);                              //Inicializar lista
    pol polinom;
    for (i=0; i<cant_pol; i++){
        fscanf(archivo, "%d", &tam_pol);                            //Leer tamano polinomio
        polinom.tam = tam_pol;
        polinom.exponente = (unsigned int *)malloc(sizeof(unsigned int)* tam_pol);  //Crear arreglo exponentes
        polinom.coeficiente = (int *)malloc(sizeof(int)* tam_pol);                  // y coeficientes
        for (j=0; j<tam_pol; j++){
            fscanf(archivo, "%u %d", &exp, &coef);              //leer exponentes y coeficientes
            polinom.exponente[j] = exp;
            polinom.coeficiente[j] = coef;
        }
        if (i==0) {
            lista_pol.head->valor_polinomio = polinom;                  //Asignar polinomio a la cabeza
            lista_pol.head->next = (tNodo *)malloc(sizeof(tNodo));      //Crear siguiente nodo
            lista_pol.head->next = NULL;
        }
        else {
            k = 0;                                  //Asignar nodos no cabeza
            tNodo *aux;                             //en la posicion final
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
    char funcion[13]; 
    char pp1[]="EVALUAR";
    char pp2[]="COEFICIENTE";
    int posleido;
    float valorleido;
    FILE *salida;
    salida=fopen("salidaPolinomio.txt","w");
    fscanf(archivo,"%s %d %f",funcion, &posleido, &valorleido);
    while(!feof(archivo)){
        if (strncmp(funcion,pp1,7)==0){
            float resul = evaluar(lista_pol, posleido, valorleido);
            fprintf(salida,"%.6f\n",resul);
        }
        else if(strncmp(funcion,pp2,11)==0){
            int resul = coeficiente(lista_pol, posleido, valorleido);
            fprintf(salida,"%d\n",resul);
        }
        fscanf(archivo,"%s %d %f",funcion, &posleido, &valorleido);
    }
    fclose(salida);
    free_linked(lista_pol.head);
    fclose(archivo);
}