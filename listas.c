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

/*****
* tNodo movetopost
******
* recorre la lista enlazada hasta llegar al polinomio deseado
******
* Input:
* tLista lista : lista enlazad que contiene a todos los polinomios leidos
* int posicion : la posicion a la cual se desea llegar
******
* Returns:
* tNodo, retorna el nodo que se encuentra en la posicion deseada
*****/
tNodo *movetopost(tLista lista, int posicion){
    int i;
    if (posicion < 0) return NULL;
    tNodo *auxiliar = (tNodo *)malloc(sizeof(tNodo *));
    auxiliar = lista.head;
    int pos = 0;
    for (i = 0; i < posicion; i++) {
        auxiliar = auxiliar->next;
        pos++;
    }
    return auxiliar;
}

/*****
* void intercambiar
******
* intercambia la posición de dos valores de un arreglo
******
* Input:
* int * A : arreglo con los valores
* int i : posicion del arreglo que se quiere cambiar
* int j : posicion del arreglo que se quiere cambiar
******
* Returns:
* void
*****/
void intercambiar (int * A, int i, int j){
    int tmp = A[i];
    A[i] = A[j];
    A[j] = tmp;
}

/*****
* void ordenarpol
******
* ordena un polinomio de forma descendente con respecto a sus exponentes, el polinomio ordenado es de tipo [[1,2],[a,b]], 
  en donde 1 y a corresponden a exp y 2 y b corresponden a coef
******
* Input:
* int * A : arreglo de exponentes sin orden
* int * A : arreglo de coeficientes relacionados por posición a los exponentes anteriores
* int N : tamaño de cada arreglo(es el mismo para ambos)
* int arreglo[][2] : arreglo vacio al que se le ingresaran exp y coef ordenados
* int *C : copia del arreglo de exponentes(*A), que permanecera inmutable
******
* Returns:
* void
*****/
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

/*****
* float evaluar
******
* evalua el polinomio a traves del algoritmo de horner, segun el valor pedido
******
* Input:
* tLista lista : lista enlazada que contiene a todos los polinomios
* int posicion : posicion del polinomio que se quiere evaluar
* float evaluado : el valor al cual se quiere evaluar el polinomio de la posicion determinada
******
* Returns:
* float, resultado numerico de la evaluacion del polinomio
*****/
float evaluar(tLista lista, int posicion, float evaluado){
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
    int i;
    for(i=1;i<sizeof(elem)/8;i++){
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
    return poli;
}

/*****
* int coeficiente
******
* obtiene el valor del coeficiente que acompaña al monomio de grado requerido
******
* Input:
* tLista lista : lista enlazada con los polinomios
* int pos : posición del polinomio que se quiere evaluar
* int expo : valor del exponente que va acompañado del coeficiente requerido
******
* Returns:
* int, retorna el valor del coeficiente pedido, si no existe el monomio del coeficiente, retorna 0
*****/
int coeficiente(tLista lista, int pos, float expo){
    tNodo *act= movetopost(lista, pos);
    pol elemento = act->valor_polinomio;
    int i;
    for(i=0;i<sizeof(elemento.exponente)/4;i++){
        if(elemento.exponente[i]==expo){
            float res = elemento.coeficiente[i];
            return res;
        }
    }
    return 0;
}


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
    salida=fopen("salidaPolinomio.txt","w");                                         //crea el archivo de salida
    fscanf(archivo,"%s %d %f",funcion, &posleido, &valorleido);                      //lee archivo y almacena los valores en las valiares asignadas
    while(!feof(archivo)){                                                           //comprobar que no se ha llegado al EOF
        if (strncmp(funcion,pp1,7)==0){                                              //verifica si lo que se pide es EVALUAR
            float resul = evaluar(lista_pol, posleido, valorleido);                  //evalua segun lo pedido
            fprintf(salida,"%.6f\n",resul);                                          //escribe el resultado en el texto de salida
        }
        else if(strncmp(funcion,pp2,11)==0){                                         //verifica si lo que se pide es COEFICIENTE
            int resul = coeficiente(lista_pol, posleido, valorleido);                //aplica la funcion segun lo pedido
            fprintf(salida,"%d\n",resul);                                            //escribe el resultado en el texto de salida
        }
        fscanf(archivo,"%s %d %f",funcion, &posleido, &valorleido);                  //lee la siguiente linea del archivo
    }
    fclose(salida);
    free_linked(lista_pol.head);
    fclose(archivo);
}
