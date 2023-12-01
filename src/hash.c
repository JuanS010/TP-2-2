#include "hash.h"
#include "lista.h"
#include "stdlib.h"
#include "string.h"
#define CAPACIDAD_MINIMA 3
#define FACTOR_CARGA 0.7

typedef struct par{
	char* clave;
    	void* valor;
} par_t;

struct hash{
    	lista_t** tabla;
    	size_t capacidad;
    	size_t cantidad;
};

unsigned int Jenkins(const char *str)
{	
	size_t len = strlen(str);
    	unsigned int hash, i;
    	for(hash = i = 0; i < len; ++i)
    	{
		hash += (unsigned int)str[i];
        	hash += (hash << 10);
        	hash ^= (hash >> 6);
    	}
    	hash += (hash << 3);
    	hash ^= (hash >> 11);
    	hash += (hash << 15);
    	return hash;
}

par_t *par_crear(const char *clave, void *valor)
{
    	par_t* par = malloc(sizeof(par_t));
    	if (!par){
        	return NULL;
    	}

	char* clave_copia = malloc(sizeof(char)*(strlen(clave)+1));
	if (!clave_copia){
		return NULL;
	}

    	strcpy(clave_copia, clave);
	par->clave = clave_copia;
    	par->valor = valor;
    	return par;
}

void *par_destruir(par_t *par)
{
	void* valor = par->valor;
    	free(par->clave);
    	free(par);
	return valor;
}

int par_contiene_clave(void* par, void* clave)
{
	par_t *p = par;
	char *c = (char*)clave;
	return strcmp(p->clave, c);
}

par_t* par_buscar(const hash_t *hash, const char *clave)
{
	size_t posicion = Jenkins(clave) % hash->capacidad;
	lista_t *lista = hash->tabla[posicion];
	return lista_buscar_elemento(lista, par_contiene_clave, (void*)clave);
}

void redimensionar(hash_t* hash)
{	
    	if (hash->cantidad/hash->capacidad < FACTOR_CARGA){
        	return;
    	}

    	size_t nueva_capacidad = (hash->capacidad*2) - 1;
    	lista_t** nueva_tabla = malloc(sizeof(lista_t*) * nueva_capacidad);
    	if (!nueva_tabla){
        	return;
    	}

    	for (int i = 0; i < nueva_capacidad; i++){
        	nueva_tabla[i] = lista_crear();
    	}

    	for (int i = 0; i < hash->capacidad; i++){

        	while (!lista_vacia(hash->tabla[i])){
            		par_t *par = lista_quitar_de_posicion(hash->tabla[i], 0);
            		size_t nueva_pos = Jenkins(par->clave) % nueva_capacidad;
            		lista_insertar(nueva_tabla[nueva_pos], par);
        	}

        	lista_destruir(hash->tabla[i]);
    	}

    	free(hash->tabla);
    	hash->tabla = nueva_tabla;
    	hash->capacidad = nueva_capacidad;
    	return;
}

hash_t *hash_crear(size_t capacidad)
{
	hash_t *hash = malloc(sizeof(hash_t));
	if (!hash) {
		return NULL;
	}

	if (capacidad < CAPACIDAD_MINIMA) {
		hash->capacidad = CAPACIDAD_MINIMA;
	} else {
		hash->capacidad = capacidad;
	}

	hash->tabla = malloc(sizeof(lista_t*) * hash->capacidad);
	if (!hash->tabla) {
		free(hash);
		return NULL;
	}

    	for (int i = 0; i < hash->capacidad; i++){
        	hash->tabla[i] = lista_crear();
    	}

	hash->cantidad = 0;
	return hash;
}


hash_t *hash_insertar(hash_t *hash, const char *clave, void *elemento,
		      void **anterior)
{
	if (!hash || !clave) {
		return NULL;
	}

	par_t* par = par_buscar(hash, clave);

	if (!par) {

		size_t posicion = Jenkins(clave) % hash->capacidad;
		par_t* par_nuevo = par_crear(clave, elemento);
		lista_insertar(hash->tabla[posicion], par_nuevo);
		hash->cantidad++;
		if (anterior) {
			*anterior = NULL;
		}

	} else {

		if (anterior) {
			*anterior = par->valor;
		}
		par->valor = elemento;
	}

	redimensionar(hash);
	return hash;
}

bool busqueda_par(void* par, void* clave)
{
	par_t *p = par;
	char *c = (char*)clave;;
	return strcmp(p->clave, c) != 0;
}

void *hash_quitar(hash_t *hash, const char *clave)
{
	if (!hash || !clave) {
		return NULL;
	}

	size_t pos_tabla = Jenkins(clave) % hash->capacidad;
	lista_t *lista = hash->tabla[pos_tabla];

	size_t pos_lista = 
	lista_con_cada_elemento(lista, busqueda_par, (void*)clave);
	if (pos_lista == 0) {
		return NULL;
	}
	pos_lista--;

	if (par_contiene_clave(
	lista_elemento_en_posicion(lista, pos_lista), (void*)clave) != 0) {
		return NULL;
	}
	hash->cantidad--;
	return par_destruir(lista_quitar_de_posicion(lista, pos_lista));
}

void *hash_obtener(hash_t *hash, const char *clave)
{
	if (!hash || !clave) {
		return NULL;
	}

	par_t* par = par_buscar(hash, clave);
    	if (!par){
        	return NULL;
    	}
    	return par->valor;
}

bool hash_contiene(hash_t *hash, const char *clave)
{	
	if (!hash || !clave) {
		return false;
	}

	par_t* par = par_buscar(hash, clave);
   	return par != NULL;
}

size_t hash_cantidad(hash_t *hash)
{
	if (!hash) {
		return 0;
	}
	return hash->cantidad;
}

void hash_destruir(hash_t *hash)
{
	hash_destruir_todo(hash, NULL);
}

void hash_destruir_todo(hash_t *hash, void (*destructor)(void *))
{
	if (!hash) {
		return;
	}

	for (int i = 0; i < hash->capacidad; i++) {

		while (!lista_vacia(hash->tabla[i])) {

			par_t *par = lista_quitar_de_posicion(hash->tabla[i], 0);
			void* valor = par_destruir(par);
			if (destructor) {
				destructor(valor);
			}
		}
		lista_destruir(hash->tabla[i]);
	}
	free(hash->tabla);
	free(hash);
}

size_t hash_con_cada_clave(hash_t *hash,
			bool (*f)(const char *clave, void *valor, void *aux),
			void *aux)
{
	if (!hash || !f) {
		return 0;
	}

	size_t contador = 0;

	for (int i = 0; i < hash->capacidad; i++) {
		
		if (lista_vacia(hash->tabla[i])) {
			continue;
		}

		lista_iterador_t *iter = lista_iterador_crear(hash->tabla[i]);

		while (lista_iterador_tiene_siguiente(iter)) {

			contador++;
			par_t *par = lista_iterador_elemento_actual(iter);
			if (!f(par->clave, par->valor, aux)) {
				lista_iterador_destruir(iter);
				return contador;
			}
			lista_iterador_avanzar(iter);
		}
		lista_iterador_destruir(iter);
	}
	return contador;
}