#include <stdio.h>

typedef struct nodo {
	int** polinomio;
	struct nodo *sig;
} tNodo;

typedef struct lista {
	tNodo *head;
	tNodo *tail;
	tNodo *curr;
	unsigned int listSize;
	unsigned int posActual;
} tLista;

int main() {

	FILE *archivo;
	archivo = fopen("entradaPolinomio.txt", "r");
	int num_polinomios, i, num_monomios, grado, coef;
	tLista lista;
	fscanf(archivo, "%d", &num_polinomios);
	lista.listSize = num_polinomios;
	lista.posActual = 0;
	for (lista.posActual; i==num_polinomios; lista.posActual++){
		fscanf(archivo, "%d", &num_monomios);
		int** listaMonomios = (int **)malloc(sizeof(int)*num_monomios*2);
		for (j=0; j==num_monomios; j++){
			fscanf(archivo, "%d %d", &grado, &coef);
			listaMonomios[j][0] = grado;
			listaMonomios[j][1] = coef;
		}
		if (i==0){
			tNodo nodo;
			nodo.polinomio = listaMonomios;
			lista.head = nodo;
		}
		else {
			tNodo nodo_actual;
			nodo_actual.polinomio = listaMonomios;
			if (i!=num_polinomios){
				nodo.sig = nodo_actual;
			}
			else {
				nodo.sig = NULL;
			}
		}
		if (i==num_polinomios){
			lista.tail = nodo_actual;
		}
	}
	return 0;
}
