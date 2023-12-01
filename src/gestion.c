#include "gestion.h"
#include "cajas.h"
#include "pokemon.h"
#include "hash.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#define MAX_LEER 30
#define LEER "r"
#define ESCRIBIR "w"
#define LECTURA_NOMBRE "%[^;];"

struct gestion {
    	hash_t *indice;
    	hash_t *cajas;
};

char* leer_entrada(char* cadena)
{   
    	char* texto = fgets(cadena, MAX_LEER, stdin);

    	if (texto) {
        	texto[strlen(texto)-1] = 0;
    	}
    	return texto;
}

bool cargar_indice(gestion_t *gestion, char *nombre_caja, caja_t* caja)
{   
    	FILE* archivo = fopen(nombre_caja, LEER);
    	if (!archivo) {
        	return false;
    	}
        
    	char nombre[MAX_LEER];
    	char cadena[MAX_LEER];
	while (fgets(cadena, MAX_LEER, archivo) != NULL) {

        	sscanf(cadena, LECTURA_NOMBRE, nombre);

        	hash_t *hash_nombre;

        	if (!hash_contiene(gestion->indice, nombre)) {
            		hash_nombre = hash_crear(10);
            		if (!hash_nombre) {
                		return false;
            		}
            	hash_insertar(gestion->indice, nombre, hash_nombre, NULL);

        	} else {

            		hash_nombre = hash_obtener(gestion->indice, nombre);
        	}

        	hash_insertar(hash_nombre, nombre_caja, NULL, NULL);
    	}

    	fclose(archivo);
    	return true;
}

int cargar_cajas(gestion_t *gestion, int argc, char *argv[]) 
{   
    	int cajas_cargadas = 0;
    	char* nombre_caja;

    	for (int i = 1; i < argc; i++) {

        	nombre_caja = argv[i];

        	if (hash_contiene(gestion->cajas, nombre_caja)) {
            		printf("No se pudo cargar la caja %s\n", nombre_caja);
            		continue;
        	}
        
        	caja_t *caja = caja_cargar_archivo(nombre_caja);
        	if (!caja) {
            		printf("No se pudo cargar la caja %s\n", nombre_caja);
            		continue;
        	}

        	if (caja_cantidad(caja) == 0) {
            		printf("No se pudo cargar la caja %s\n", nombre_caja);
            		caja_destruir(caja);
            		continue;
        	}

        	if (!cargar_indice(gestion, nombre_caja, caja)) {
            		caja_destruir(caja);
            		printf("No se pudo cargar la caja %s\n", nombre_caja);
            		continue;
        	}

        	hash_insertar(gestion->cajas, nombre_caja, caja, NULL);
        	cajas_cargadas++;
    	}

    	return cajas_cargadas;
}

void imprimir_pokemon(pokemon_t *pokemon)
{   
	printf("Nombre: %s | ", pokemon_nombre(pokemon));
	printf("Nivel: %d | ", pokemon_nivel(pokemon));
	printf("Ataque: %d | ", pokemon_ataque(pokemon));
	printf("Defensa: %d\n", pokemon_defensa(pokemon));
}

bool imprimir_caja(const char *clave, void *valor, void *aux)
{   
    	caja_t* caja = valor;

    	printf("\n    Nombre de la caja: %s     \n\n", clave);
    	caja_recorrer(caja, imprimir_pokemon);
    	printf("\n");
    	return true;
}

bool mostrar_caja(gestion_t *gestion)
{   
    	printf("Ingrese el nombre de la caja: ");
    	char texto[MAX_LEER];
	texto[0] = 0;
    	char* nombre = leer_entrada(texto);

    	if (!hash_contiene(gestion->cajas, nombre)) {
        	printf("No se encuentra la caja ingresada\n");
        	return true;
    	}
    	imprimir_caja(nombre, hash_obtener(gestion->cajas, nombre), NULL);
    	return true;
}

bool mostrar_todo(gestion_t *gestion) 
{   
    	hash_con_cada_clave(gestion->cajas, imprimir_caja, NULL);
    	return true;
}

bool  _mostrar_inventario(const char *clave, void *valor, void *aux)
{
    	printf("Nombre de la caja: %s | Cantidad de pokemones: %d\n", clave,
			 caja_cantidad(valor));
    	return true;
}

bool mostrar_inventario(gestion_t *gestion)
{
    	printf("INVENTARIO:\n");
    	hash_con_cada_clave(gestion->cajas, _mostrar_inventario, NULL);
    	printf("\n");
    	return true;
}

bool imprimir_cajas_pokemon(const char *clave, void *valor, void *aux)
{
    	printf("%s\n", clave);
    	return true;
}

bool buscar_caja(gestion_t *gestion)
{   
    	printf("Ingrese el nombre del pokemon: ");
    	char cadena[MAX_LEER];
    	leer_entrada(cadena);

    	if (!hash_contiene(gestion->indice, cadena)) {
        	printf("No se encontro el pokemon %s\n", cadena);
        	return true;
    	}
    	printf("\nPokemon: %s | Cantidad de cajas que lo contienen: %d\n\nCajas:\n",
		 cadena, (int)hash_cantidad(hash_obtener(gestion->indice, cadena)));
    	hash_con_cada_clave(hash_obtener(gestion->indice, cadena),
				 imprimir_cajas_pokemon, NULL);
    	printf("\n");
    	return true;
}

bool cargar_otra_caja(gestion_t *gestion)
{       
    	printf("Ingrese el nombre del archivo a cargar: ");
    	char cadena[MAX_LEER];
    	leer_entrada(cadena);

    	char *nombre[2];
    	nombre[1] = cadena;
    
    	if (cargar_cajas(gestion, 2, nombre) == 1) {
	        printf("Caja cargada correctamente.\n");
    	}
    	return true;
}

bool combinar_cajas(gestion_t* gestion)
{   
    	char caja_1[MAX_LEER];
    	char caja_2[MAX_LEER];
    	char nombre_caja[MAX_LEER];

    	printf("Ingrese el nombre de la primera caja: ");
    	leer_entrada(caja_1);

    	printf("Ingrese el nombre de la segunda caja: ");
    	leer_entrada(caja_2);

    	printf("Ingrese el nombre de la nueva caja: ");
    	leer_entrada(nombre_caja);
    
    	if (!hash_contiene(gestion->cajas, caja_1)
		 || !hash_contiene(gestion->cajas, caja_2)
		 || hash_contiene(gestion->cajas, nombre_caja)) {
        	printf("No fue posible combinar las cajas\n");
        	return true;
    	}

	caja_t *nueva_caja = caja_combinar(hash_obtener(gestion->cajas, caja_1),
				 hash_obtener(gestion->cajas, caja_2));
    	if (!nueva_caja) {
        	printf("No fue posible combinar las cajas\n");
        	return true;
    	}

    	hash_insertar(gestion->cajas, nombre_caja, nueva_caja, NULL);
    	caja_guardar_archivo(nueva_caja, nombre_caja);
    	cargar_indice(gestion, nombre_caja, nueva_caja);
    	printf("Caja cargada correctamente.\n");
    	return true;
}

void borrar_cajas(void *caja)
{
    	caja_destruir(caja);
}

void borrar_hash(void *hash)
{
    	hash_destruir(hash);
}

bool salir(gestion_t *gestion)
{
    	hash_destruir_todo(gestion->cajas, borrar_cajas);
    	hash_destruir_todo(gestion->indice, borrar_hash);
    	free(gestion);
    	return false;
}

gestion_t *iniciar_gestion(int argc, char *argv[])
{   
    	if (argc < 2) {
    		return NULL;
    	}

    	gestion_t *gestion = malloc(sizeof(gestion_t));
    	if (!gestion) {
        	return NULL;
    	}

    	gestion->indice = hash_crear(10);
    	if (!gestion->indice) {
        	free(gestion);
        	return NULL;
    	}

    	gestion->cajas = hash_crear(10);;
    	if (!gestion->cajas) {
        	hash_destruir(gestion->indice);
        	free(gestion);
        	return NULL;
    	}

	if (cargar_cajas(gestion, argc, argv) < 1) {
        	hash_destruir(gestion->indice);
        	hash_destruir(gestion->cajas);
        	free(gestion);
        	return NULL;
    	}
    	return gestion;
}