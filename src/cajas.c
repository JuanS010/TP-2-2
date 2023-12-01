#include "cajas.h"
#include "pokemon.h"
#include "abb.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#define LEER "r"
#define ESCRIBIR "w"
#define MAX_LEER 200
#define FORMATO_ESCRITURA "%s;%d;%d;%d\n"

typedef void (*funcion_recorrer)(pokemon_t*);

struct _caja_t {
	abb_t* pokemones;
};

int cmp(void* a, void* b)
{
	char* aa = a;
	char* bb = b;
	return strcmp(aa, bb);
}

caja_t *caja_cargar_archivo(const char *nombre_archivo)
{	
	if (!nombre_archivo){
		return NULL;
	}

	FILE *archivo = fopen(nombre_archivo, LEER);
	if (!archivo) {
		return NULL;
	}

	caja_t *caja = malloc(sizeof(caja_t));
	if (!caja) {
		fclose(archivo);
		return NULL;
	}

	caja->pokemones = abb_crear(cmp);
	if (!caja->pokemones) {
		free(caja);
		fclose(archivo);
		return NULL;
	}

	char cadena[MAX_LEER];
	while (fgets(cadena, MAX_LEER, archivo) != NULL) {

		pokemon_t* pokemon = pokemon_crear_desde_string(cadena);
		if (!pokemon) {
			continue;
		}

		abb_insertar(caja->pokemones, pokemon);
	}

	fclose(archivo);
	return caja;
}

bool caja_escribir(void *poke, void* file)
{	
	pokemon_t *pokemon = poke;
	FILE *archivo = file;

	fprintf(archivo, FORMATO_ESCRITURA, 
	pokemon_nombre(pokemon), 
	pokemon_nivel(pokemon), 
	pokemon_ataque(pokemon), 
	pokemon_defensa(pokemon));

	return true;
}

int caja_guardar_archivo(caja_t *caja, const char *nombre_archivo)
{
	if (!caja || !nombre_archivo) {
		return 0;
	}

	FILE *archivo = fopen(nombre_archivo, ESCRIBIR);
	if (!archivo) {
		return 0;
	}

	int contador = (int)abb_con_cada_elemento(caja->pokemones, 
				INORDEN, caja_escribir, archivo);
	fclose(archivo);
	return contador;
}

bool crear_nuevos_pokemones(void *poke, void *caja)
{
	pokemon_t *pokemon = poke;
	abb_t *pokemones = caja;

	char cadena[MAX_LEER];
	sprintf(cadena, "%s;%d;%d;%d\n", 
		pokemon_nombre(pokemon), 
		pokemon_nivel(pokemon), 
		pokemon_ataque(pokemon), 
		pokemon_defensa(pokemon));		
		pokemon_t *nuevo_pokemon = pokemon_crear_desde_string(cadena);

	abb_insertar(pokemones, nuevo_pokemon);
	return true;
}

caja_t *caja_combinar(caja_t *caja1, caja_t *caja2)
{	
	if (!caja1 || !caja2) {
		return NULL;
	}

	caja_t *caja3 = malloc(sizeof(caja_t));
	if (!caja3) {
		return NULL;
	}

	caja3->pokemones = abb_crear(cmp);
	if (!caja3->pokemones) {
		free(caja3);
		return NULL;
	}

	abb_con_cada_elemento(caja1->pokemones, POSTORDEN,
		 crear_nuevos_pokemones, caja3->pokemones);
	abb_con_cada_elemento(caja2->pokemones, POSTORDEN,
		 crear_nuevos_pokemones, caja3->pokemones);
	return caja3;
}

int caja_cantidad(caja_t *caja)
{
	if (!caja) {
		return 0;
	}
	return (int)abb_tamanio(caja->pokemones);
}

bool obtener_n(void* poke, void* n)
{	
	if (**(int**)n == 0) {
		*(void**)n = poke;
		return false;
	}
	(**(int**)n)--;
	return true;
}

pokemon_t *caja_obtener_pokemon(caja_t *caja, int n)
{	
	if (!caja || n < 0 || n >= caja_cantidad(caja)) {
		return NULL;
	}
		
	int* num = &n;
	void **aux = (void**)&num;

	abb_con_cada_elemento(caja->pokemones, INORDEN, obtener_n, aux);
	pokemon_t *pokemon = *aux;
	return pokemon;		
}


bool _funcion(void* pokemon, void* f)
{	
	funcion_recorrer* funcion = f;
	(*funcion)((pokemon_t*)pokemon);
	return true;
}

int caja_recorrer(caja_t *caja, void (*funcion)(pokemon_t *))
{	
	if (!caja || !funcion) {
		return 0;
	}

	return (int)abb_con_cada_elemento(caja->pokemones,
			 INORDEN, _funcion, &funcion);
}

void _pokemon_destruir(void* pokemon)
{	
	pokemon_t *poke = pokemon;
	pokemon_destruir(poke);
}

void caja_destruir(caja_t *caja)
{
	if (!caja) {
		return;
	}

	abb_destruir_todo(caja->pokemones, _pokemon_destruir);
	free(caja);
}