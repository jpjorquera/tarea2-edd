#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

tNodo *movetopost(tABB arbol, int posicion){
    int i;
    printf("%d\n",posicion);
    if (posicion < 0) return NULL;
    tNodo *auxiliar = (tNodo *)malloc(sizeof(tNodo *));
    auxiliar = arbol.raiz;
    pol aux = auxiliar->valor_polinomio;
    while(aux.pos!=posicion){
        if (posicion<aux.pos){
            auxiliar = auxiliar->izq;
            aux = auxiliar->valor_polinomio;
        }
        else{
            auxiliar = auxiliar->der;
            aux = auxiliar->valor_polinomio;
        }
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


float evaluar(tABB arbol, int posicion, float evaluado){
    tNodo *act= movetopost(arbol, posicion);
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


float coeficiente(tABB arbol, int pos, float expo){
    tNodo *act= movetopost(arbol, pos);
    pol elemento = act->valor_polinomio;
    int i=0;
    for(i;i<sizeof(elemento.exponente)/4;i++){
        if(elemento.exponente[i]==expo){
            float res = elemento.coeficiente[i];
            return res;
        }
    }
    return 0;
}


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
            float resul = evaluar(arbol, posleido, valorleido);
            fprintf(salida,"%.6f\n",resul);
        }
        else if(strncmp(funcion,pp2,11)==0){
            int resul = coeficiente(arbol, posleido, valorleido);
            fprintf(salida,"%d\n",resul);
        }
        fscanf(archivo,"%s %d %f",funcion, &posleido, &valorleido);
    }
    fclose(salida);
    fclose(archivo);
}
