#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct polinomio {          //Estructura que almacena
    int tam;                        // tamano, posicion de ingreso
    int pos;                        // exponentes y coeficientes del polinomio
    unsigned int *exponente;
    int *coeficiente;
} pol;

typedef struct node {           //Nodo tipo arbol
    pol valor_polinomio;
    struct node *izq;
    struct node *der;

} tNodo;

typedef struct abb {        //Arbol incluyendo nodo raiz y tamano
    tNodo *raiz;
    int Size;
} tABB;

/*****
* void crear_arbol
******
* Crea arbol completo (niveles llenos) segun la cantidad de nodos necesitados
******
* Input:
* tNodo * base : nodo que se asigna a cada padre, incluyendo a la raiz
* unsigned int cant_hijos : cantidad de hijos posibles de la base actual
******
* Returns:
* void
*****/
void crear_arbol(tNodo *base, unsigned int cant_hijos){
    if (cant_hijos == 0 ){                          //Si no hay hijos
        base->izq = NULL;
        base->der = NULL;
        return;
    }
    base->izq = (tNodo *)malloc(sizeof(tNodo));
    cant_hijos = cant_hijos/2-1;
    base->der = (tNodo *)malloc(sizeof(tNodo));     //Iterar sobre hijos
    crear_arbol(base->izq, cant_hijos);
    crear_arbol(base->der, cant_hijos);
}

/*****
* unsigned int power
******
* Calcular potencia de un numero
******
* Input:
* int x : base de la potencia
* unsigned int y : exponente de la potencia
******
* Returns:
* unsigned int, valor final de la potencia
*****/
unsigned int power(int x, unsigned int y)
{
    if( y == 0)
        return 1;
    else if (y%2 == 0)
        return power(x, y/2)*power(x, y/2);
    else
        return x*power(x, y/2)*power(x, y/2);
}

/*****
* tABB initialize
******
* Inicializar la lista vacia segun la cantidad de polinomios (nodos) necesitados
******
* Input:
* int cantPol : cantidad de polinomios, se necesitan para calcular el maximo de hijos a crear
******
* Returns:
* tABB, arbol ya creado vacia e inicializado
*****/
tABB initialize(int cantPol){
    tABB tree;
    tree.raiz = (tNodo *)malloc(sizeof(tNodo));
    int i;
    unsigned int h = 0;
    unsigned int nodos = 0;
    while (cantPol > nodos){                    //Calcular hijos de cada nivel
        nodos += power(2, h++);
    }
    crear_arbol(tree.raiz, nodos-1);
    return tree;
}

/*****
* void asignar
******
* Asignar el polinomio segun su posicion para una busqueda eficiente (AVL)
******
* Input:
* tNodo * base : puntero a la base (o raiz) del nodo actual a asignar
* pol polinom : informacion del polinomio a guardar en cada nodo
* unsigned int half : mitad del total de polinomios para asignar a base
******
* Returns:
* void
*****/
void asignar(tNodo *base, pol polinom, unsigned int half){
    if (half == polinom.pos){                   //Si polinomio actual corresponde a la mitad
        base->valor_polinomio = polinom;        //Asignar en el lugar
        return;
    }
    else {
        if (polinom.pos < half) {               //Comparar para asignar izquierda o derecha
            half /= 2;                          // recalcular mitad
            asignar(base->izq, polinom, half);
        }
        else {
            if (half == 1){                     //Caso mitad decimal
                half+=1;
            }
            else {
                half += half / 2;
            }
            asignar(base->der, polinom, half);
        }
    }
}

/*****
* void limpiar
******
* Busca en el arbol completo los nodos vacios para eliminarlos
******
* Input:
* tNodo base : raiz de donde empezar la busqueda
******
* Returns:
* void
*****/
void limpiar(tNodo *base){
    if (base->izq != NULL) {                            //Verificar al no ser NULL
        if (base->izq->valor_polinomio.tam == 0) {      //Verificar nodo "vacio"
            free(base->izq);                            //Eliminar nodo
            base->izq = NULL;
        }
        else {
            limpiar(base->izq);
        }
    }
    if (base->der != NULL) {
        if (base->der->valor_polinomio.tam == 0) {
            free(base->der);
            base->der = NULL;
        }
        else {
            limpiar(base->der);
        }
    }
}

/*****
* void liberar_arbol
******
* Liberar memoria correspondiente al arbol
******
* Input:
* tNodo base : raiz del arbol a liberar;
******
* Returns:
* void
*****/
void liberar_arbol(tNodo *base){
    if (base == NULL){          //Caso base
        return;
    }
    liberar_arbol(base->izq);                    //Buscar
    liberar_arbol(base->der);
    free(base->valor_polinomio.coeficiente);
    free(base->valor_polinomio.exponente);
    free(base);
}

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

int main(){
    FILE *archivo = fopen("entradaPolinomio.txt", "r");        //Entrada
    int cant_pol;
    int i, tam_pol, j;
    unsigned int exp, coef, half;
    fscanf(archivo, "%d", &cant_pol);                //Leer cantidad de polinomios
    tABB arbol;
    arbol = initialize(cant_pol);
    arbol.Size = cant_pol;
    for (i=0; i<cant_pol; i++){
        fscanf(archivo, "%d", &tam_pol);
        pol polinom;                                //Crear polinomomio actual tipo pol
        polinom.tam = tam_pol;
        polinom.pos = i;
        polinom.exponente = (unsigned int *)malloc(sizeof(unsigned int)* tam_pol);
        polinom.coeficiente = (int *)malloc(sizeof(int)* tam_pol);
        for (j=0; j<tam_pol; j++){                      //Crear arreglos de exponente y coeficientes
            fscanf(archivo, "%u %u", &exp, &coef);
            polinom.exponente[j] = exp;
            polinom.coeficiente[j] = coef;
        }
        half = (unsigned int) (cant_pol / 2);
        asignar(arbol.raiz, polinom, half);             //Asignar en el arbol
    }
    limpiar(arbol.raiz);                                //Limpiar nodos inutilizados
    int a = 0;
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
    liberar_arbol(arbol.raiz);
    return 1;
}