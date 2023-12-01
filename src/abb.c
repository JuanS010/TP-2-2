#include "abb.h"
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>	

nodo_abb_t *nodo_crear(void *elemento) 
{
	nodo_abb_t *nodo = malloc(sizeof(nodo_abb_t));
		if (!nodo){
       		return NULL;
    	}

    	nodo->elemento = elemento;
    	nodo->derecha = NULL;
    	nodo->izquierda = NULL;
    	return nodo;
}

nodo_abb_t *nodo_buscar(nodo_abb_t *nodo, void *elemento,
			 abb_comparador comparador)
{
   	nodo_abb_t *anterior = NULL;

    	while (nodo != NULL){

    		int cmp = comparador(nodo->elemento, elemento);
    		if (cmp == 0) {
        		return nodo;
    		}

    		anterior = nodo;
    		if (cmp > 0){
        		nodo = nodo->izquierda;
    		} else {
        		nodo = nodo->derecha;
    		}
	}
	return anterior;
}

nodo_abb_t *nodo_buscar_anterior(nodo_abb_t *nodo, void *elemento, 
				abb_comparador comparador)
{
    	nodo_abb_t *anterior = NULL;

	while (nodo != NULL){

        	int cmp = comparador(nodo->elemento, elemento);
        	if (cmp == 0) {
        		return anterior;
        	}

    		anterior = nodo;
    		if (cmp > 0){
        		nodo = nodo->izquierda;
    		} else {
        		nodo = nodo->derecha;
    		}
	}

	return anterior;
}

void *nodo_borrar(nodo_abb_t *nodo)
{
	void* dato = nodo->elemento;
    	free(nodo);
    	return dato;
}

abb_t *abb_crear(abb_comparador comparador)
{
	if (!comparador) {
        	return NULL;
    	}

    	abb_t *arbol = malloc(sizeof(abb_t));
    	if (!arbol) {
       		return NULL;
    	}

	arbol->nodo_raiz = NULL;
	arbol->comparador = comparador;
	arbol->tamanio = 0;
    	return arbol;
}

abb_t *abb_insertar(abb_t *arbol, void *elemento)
{
	if (!arbol) {
        return NULL;
	}

   	nodo_abb_t *nuevo = nodo_crear(elemento);
    	if (!nuevo) {
		return NULL;
    	}

    	if (abb_vacio(arbol)) {

    		arbol->nodo_raiz = nuevo;

	} else {

        	nodo_abb_t *anterior = nodo_buscar(arbol->nodo_raiz, 
					elemento, arbol->comparador);

		int cmp = arbol->comparador(anterior->elemento, 
					nuevo->elemento);
        	if (cmp == 0) {
			nuevo->izquierda = anterior->izquierda;
            		anterior->izquierda = nuevo;
        	} else if (cmp > 0) {
        		anterior->izquierda = nuevo;
    		} else {
			anterior->derecha = nuevo;
		}
	}

    	arbol->tamanio++;
    	return arbol;
}

void *abb_quitar(abb_t *arbol, void *elemento)
{
	if (!arbol || abb_vacio(arbol)) {
		return NULL;
	}

	nodo_abb_t *anterior;
	nodo_abb_t *nodo;
	bool izq = true;

	if (arbol->comparador(arbol->nodo_raiz->elemento, elemento) == 0) {
		
		anterior = NULL;
		nodo = arbol->nodo_raiz;

	} else {

		anterior = nodo_buscar_anterior(arbol->nodo_raiz, 
				elemento, arbol->comparador);

		if (anterior->izquierda != NULL && 
		arbol->comparador(anterior->izquierda->elemento, elemento) == 0) {
			nodo = anterior->izquierda;
		} else if (anterior->derecha != NULL && 
		arbol->comparador(anterior->derecha->elemento, elemento) == 0) {
			nodo = anterior->derecha;
			izq = false;
		} else {
			return NULL;
		}
	}
    
	void *valor = NULL;

	if (!nodo->izquierda || !nodo->izquierda) {
		nodo_abb_t *hijo = nodo->derecha;
		if (!hijo) {
			hijo = nodo->izquierda;
		}

		if (anterior == NULL) {
			arbol->nodo_raiz = hijo;
		} else if (izq) {
			anterior->izquierda = hijo;
		} else {
			anterior->derecha = hijo;
		}

		valor = nodo->elemento;
		nodo_borrar(nodo);
		arbol->tamanio--;
		return valor;

	} else {

		nodo_abb_t *anterior_reemplzo = nodo;
		nodo_abb_t *nodo_reemplazo = nodo->izquierda;
		while (nodo_reemplazo->derecha) {
			anterior_reemplzo = nodo_reemplazo;
			nodo_reemplazo = nodo_reemplazo->derecha;
		}

		if (anterior_reemplzo != nodo) {
			anterior_reemplzo->derecha = nodo_reemplazo->izquierda;
		} else {
			anterior_reemplzo->izquierda = nodo_reemplazo->izquierda;
		}

		valor = nodo->elemento;
		nodo->elemento = nodo_reemplazo->elemento;
		nodo_borrar(nodo_reemplazo);
		arbol->tamanio--;
		return valor;
	}
	return NULL;
}

void *abb_buscar(abb_t *arbol, void *elemento)
{
	if (!arbol || abb_vacio(arbol)) {
        	return NULL;
    	}

    	nodo_abb_t *nodo = nodo_buscar(arbol->nodo_raiz, 
				elemento, arbol->comparador);

	if (arbol->comparador(nodo->elemento, elemento) == 0) {
    		return nodo->elemento;
    	}
    	return NULL;
}

bool abb_vacio(abb_t *arbol)
{
	if (!arbol || !arbol->nodo_raiz) {
        	return true;
    	}
	return false;
}

size_t abb_tamanio(abb_t *arbol)
{
    	if (!arbol) {
        	return 0;
    	}
   	return arbol->tamanio;
}

void _abb_destruir(nodo_abb_t *nodo, void (*destructor)(void *)) 
{
	if (!nodo) {
		return;
	}
	_abb_destruir(nodo->izquierda, destructor);
	_abb_destruir(nodo->derecha, destructor);

	if (destructor) {
		destructor(nodo->elemento);
	}
	nodo_borrar(nodo);
	return;
}

void abb_destruir(abb_t *arbol)
{	
	if (!arbol) {
		return;
	}
	_abb_destruir(arbol->nodo_raiz, NULL);
	free(arbol);
}

void abb_destruir_todo(abb_t *arbol, void (*destructor)(void *))
{
	if (!arbol) {
		return;
	}
	_abb_destruir(arbol->nodo_raiz, destructor);
	free(arbol);
}

size_t iterador_preorden(nodo_abb_t *nodo, bool (*funcion)(void *, void *),
			 void *aux, bool* iterar)
{
	if (!nodo || !*iterar) {
		return 0;
	}
	size_t contador_nodo = 0;
	if (*iterar) {
		*iterar = funcion(nodo->elemento, aux);
		contador_nodo++;
	}
	if (*iterar) {
		contador_nodo += iterador_preorden(nodo->izquierda,
						 funcion, aux, iterar);
	}
	if (*iterar) {
		contador_nodo += iterador_preorden(nodo->derecha,
						 funcion, aux, iterar);
	}
	return contador_nodo;
}

size_t iterador_inorden(nodo_abb_t *nodo, bool (*funcion)(void *, void *),
			 void *aux, bool* iterar)
{
	if (!nodo || !*iterar) {
		return 0;
	}
	size_t contador_nodo = 0;
	if (*iterar) {
		contador_nodo += iterador_inorden(nodo->izquierda,
						 funcion, aux, iterar);
	}
	if (*iterar) {
		*iterar = funcion(nodo->elemento, aux);
		contador_nodo++;
	}
	if (*iterar) {
		contador_nodo += iterador_inorden(nodo->derecha,
						 funcion, aux, iterar);
	}
	return contador_nodo;
}

size_t iterador_postorden(nodo_abb_t *nodo, bool (*funcion)(void *, void *),
			 void *aux, bool* iterar)
{
	if (!nodo || !*iterar) {
		return 0;
	}
	size_t contador_nodo = 0;
	if (*iterar) {
		contador_nodo += iterador_postorden(nodo->izquierda,
						 funcion, aux, iterar);
	}
	if (*iterar) {
		contador_nodo += iterador_postorden(nodo->derecha,
						 funcion, aux, iterar);
	}
	if (*iterar) {
		*iterar = funcion(nodo->elemento, aux);
		contador_nodo++;
	}
	return contador_nodo;
}

size_t abb_con_cada_elemento(abb_t *arbol, abb_recorrido recorrido,
			     bool (*funcion)(void *, void *), void *aux)
{	
	if (!arbol || !funcion){
		return 0;
	}

	bool iterar = true;
	switch (recorrido) {

		case PREORDEN:
			return iterador_preorden(arbol->nodo_raiz, 
						funcion, aux, &iterar);

		case INORDEN:
			return iterador_inorden(arbol->nodo_raiz, 
						funcion, aux, &iterar);

		case POSTORDEN:
			return iterador_postorden(arbol->nodo_raiz,
						 funcion, aux, &iterar);
	}
	return 0;
} 

void abb_recorrer_preorden(nodo_abb_t *nodo, void **array,
			 size_t tamanio_array, size_t *contador)
{
	if (!nodo || *contador == tamanio_array) {
		return;
	}

	if (*contador >= tamanio_array) {
		return;
	}		

	array[(*contador)] = nodo->elemento;
	(*contador)++;

	abb_recorrer_preorden(nodo->izquierda, array, tamanio_array, contador);

	abb_recorrer_preorden(nodo->derecha, array, tamanio_array, contador);
	
	return;
}

void abb_recorrer_inorden(nodo_abb_t *nodo, void **array, size_t tamanio_array,
				 size_t *contador)
{
	if (!nodo || *contador == tamanio_array) {
		return;
	}

	abb_recorrer_inorden(nodo->izquierda, array, tamanio_array, contador);

	if (*contador >= tamanio_array) {
		return;
	}		

	array[(*contador)] = nodo->elemento;
	(*contador)++;

	abb_recorrer_inorden(nodo->derecha, array, tamanio_array, contador);
	
	return;
}

void abb_recorrer_postorden(nodo_abb_t *nodo, void **array,
				 size_t tamanio_array, size_t *contador)
{
	if (!nodo || *contador == tamanio_array) {
		return;
	}

	abb_recorrer_postorden(nodo->izquierda, array, tamanio_array, contador);

	abb_recorrer_postorden(nodo->derecha, array, tamanio_array, contador);
	
	if (*contador >= tamanio_array) {
		return;
	}		

	array[(*contador)] = nodo->elemento;
	(*contador)++;

	return;
}

size_t abb_recorrer(abb_t *arbol, abb_recorrido recorrido, void **array,
		    size_t tamanio_array)
{
	if (!arbol || !array) {
		return 0;
	}

	size_t contador = 0;
	switch (recorrido) {

		case PREORDEN:
			abb_recorrer_preorden(arbol->nodo_raiz, array,
						 tamanio_array, &contador);
			break;

		case INORDEN:
			abb_recorrer_inorden(arbol->nodo_raiz, array,
						 tamanio_array, &contador);
			break;

		case POSTORDEN:
			abb_recorrer_postorden(arbol->nodo_raiz, array,
						 tamanio_array, &contador);
			break;
	}
	return contador;
}
