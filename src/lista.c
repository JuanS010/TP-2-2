#include "lista.h"
#include <stddef.h>
#include <stdlib.h>
#define ERROR_VOID NULL
#define ERROR_N 0

nodo_t* nodo_crear_l(void* elemento)
{
	nodo_t *nodo = malloc(sizeof(nodo_t));
	if (!nodo) {
        	return ERROR_VOID;
    	}
    	nodo->elemento = elemento;
    	nodo->siguiente = NULL;
    	return nodo;
}

void nodo_destruir(nodo_t *nodo)
{
	free(nodo);
}

nodo_t* nodo_buscar_l(nodo_t* actual, int indice, size_t posicion)
{
	while (indice != posicion) {
		actual = actual->siguiente;
		indice++;
	}
	return actual;
}

lista_t *lista_crear()
{
    	lista_t *lista = malloc(sizeof(lista_t));
    	if (!lista) {
        	return ERROR_VOID;
    	}

    	lista->nodo_inicio = NULL;
    	lista->nodo_fin = NULL;
    	lista->cantidad = 0;
    	return lista;
}

lista_t *lista_insertar(lista_t *lista, void *elemento)
{	
	if (!lista) {
		return ERROR_VOID;
	}

    	nodo_t *nodo = nodo_crear_l(elemento);
    	if (!nodo) {
        	return ERROR_VOID;
    	}

    	if (lista_vacia(lista)) {
        	lista->nodo_inicio = nodo;
    	} else {	
        	lista->nodo_fin->siguiente = nodo;
    	}

    	lista->nodo_fin = nodo;
    	lista->cantidad++;
    	return lista;
}

lista_t *lista_insertar_en_posicion(lista_t *lista, void *elemento,
				    size_t posicion)
{
	if (!lista) {
		return ERROR_VOID;
	}

	if (lista_vacia(lista) || posicion >= lista_tamanio(lista)) {
		return lista_insertar(lista, elemento);
	}

	nodo_t *nuevo = nodo_crear_l(elemento);
	if (!nuevo) {
		return ERROR_VOID;
	}

	if (posicion == 0) {

		nuevo->siguiente = lista->nodo_inicio;
		lista->nodo_inicio = nuevo;

	} else {

		nodo_t *actual = lista->nodo_inicio;
		int indice = 1;
		actual = nodo_buscar_l(actual, indice, posicion);

		nuevo->siguiente = actual->siguiente;
		actual->siguiente = nuevo;
	}

	lista->cantidad++;
	return lista;
}

void *lista_quitar(lista_t *lista)
{
	if (!lista || lista_vacia(lista)) {
		return ERROR_VOID;
	}

	nodo_t *borrar;

	if (lista_tamanio(lista) == 1) {
		
		borrar = lista->nodo_fin;
		lista->nodo_inicio = NULL;
		lista->nodo_fin = NULL;

	} else {

		int indice = 0;
		nodo_t *actual = lista->nodo_inicio;
		actual = nodo_buscar_l(actual, indice, lista->cantidad-2);
		borrar = lista->nodo_fin;
		lista->nodo_fin = actual;
		actual->siguiente = NULL;	
	}
	
	void *elemento = borrar->elemento;
	nodo_destruir(borrar);
	lista->cantidad--;
	return elemento;
}

void *lista_quitar_de_posicion(lista_t *lista, size_t posicion)
{	
	if (!lista || lista_vacia(lista)) {
		return ERROR_VOID;
	}
	if (posicion >= lista->cantidad-1) {
		return lista_quitar(lista);
	}

	nodo_t *borrar;
	if (posicion == 0) {

		borrar = lista->nodo_inicio;
		lista->nodo_inicio = borrar->siguiente;

	} else {

		int indice = 1;
		nodo_t *actual = lista->nodo_inicio;
		actual = nodo_buscar_l(actual, indice, posicion);
		borrar = actual->siguiente;
		actual->siguiente = borrar->siguiente;
	}
	void *elemento = borrar->elemento;
	nodo_destruir(borrar);
	lista->cantidad--;
	return elemento;	
}

void *lista_elemento_en_posicion(lista_t *lista, size_t posicion)
{
	if (!lista || posicion >= lista->cantidad){
		return ERROR_VOID;
	}

	if (posicion >= lista->cantidad-1) {
		return lista_ultimo(lista);
	}
	
	nodo_t *actual = lista->nodo_inicio;
	int indice = 0;
	actual = nodo_buscar_l(actual, indice, posicion);

	return actual->elemento;
}

void *lista_buscar_elemento(lista_t *lista, int (*comparador)(void *, void *),
			    void *contexto)
{	
	if (!lista || !comparador) {
		return ERROR_VOID;
	}

	nodo_t *actual = lista->nodo_inicio;
	while (actual && comparador(actual->elemento, contexto) != 0) {
		actual = actual->siguiente;
	}

	if (!actual) {
		return ERROR_VOID;
	}
	return actual->elemento;
}

void *lista_primero(lista_t *lista)
{
	if (!lista || lista_vacia(lista)) {
		return ERROR_VOID;
	}
	return lista->nodo_inicio->elemento;
}

void *lista_ultimo(lista_t *lista)
{
	if (!lista || lista_vacia(lista)) {
		return ERROR_VOID;
	}
	return lista->nodo_fin->elemento;
}

bool lista_vacia(lista_t *lista)
{
	if (!lista || !lista->nodo_inicio) {
		return true;
	}
	return false;
}

size_t lista_tamanio(lista_t *lista)
{
	if (!lista) {
		return ERROR_N;
	}
	return lista->cantidad;
}

void lista_destruir(lista_t *lista)
{	
	lista_destruir_todo(lista, NULL);
}

void lista_destruir_todo(lista_t *lista, void (*funcion)(void *))
{
	if (!lista) {
		return;
	}
   	 while (!lista_vacia(lista)) {
        	void *dato = lista_quitar_de_posicion(lista, 0);
        	if (funcion) {
            		funcion(dato);
        	}
    	}
    	free(lista);
}

lista_iterador_t *lista_iterador_crear(lista_t *lista)
{
	if (!lista) {
		return ERROR_VOID;
	}

	lista_iterador_t *iterador = malloc(sizeof(lista_iterador_t));
	if (!iterador) {
		return ERROR_VOID;
	}

	iterador->lista = lista;
	iterador->corriente = lista->nodo_inicio;
	return iterador;
}

bool lista_iterador_tiene_siguiente(lista_iterador_t *iterador)
{
	if (!iterador || !iterador->corriente) {
		return false;
	}
	return true;
}

bool lista_iterador_avanzar(lista_iterador_t *iterador)
{
	if (!iterador || !iterador->corriente) {
		return false;
	}

	iterador->corriente = iterador->corriente->siguiente;

	if (iterador->corriente) {
		return true;
	}
	return false;
}

void *lista_iterador_elemento_actual(lista_iterador_t *iterador)
{
	if (!iterador || !iterador->corriente) {
		return ERROR_VOID;
	}

	return iterador->corriente->elemento;
}

void lista_iterador_destruir(lista_iterador_t *iterador)
{
	if (!iterador) {
		return;
	}
	free(iterador);
}

size_t lista_con_cada_elemento(lista_t *lista, bool (*funcion)(void *, void *),
			       void *contexto)
{	
	if (!lista || !funcion) {
		return ERROR_N;
	}

    	nodo_t *actual = lista->nodo_inicio;
	size_t contador = 0;
    	while (actual) {
		contador++;
		if (!funcion(actual->elemento, contexto)) {
			return contador;
		}
        	actual = actual->siguiente;
    	}
	return contador;
}
