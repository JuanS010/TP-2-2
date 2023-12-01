#include "pokemon.h"
#include <stdlib.h>
#include <stdio.h>
#define MAX_NOMBRE_POKEMON 30
#define PARAMETROS 4 
#define LECTURA_CSV "%[^;];%i;%i;%i\n"

struct _pokemon_t {
		char nombre[MAX_NOMBRE_POKEMON];
		int nivel;
		int ataque;
		int defensa;
};

pokemon_t *pokemon_crear_desde_string(char *string)
{
		if (!string) {
				return NULL;
		}

		pokemon_t *pokemon = malloc(sizeof(pokemon_t));
		if (!pokemon) {
				return NULL;
		}
		if (sscanf(string, LECTURA_CSV, pokemon->nombre, &pokemon->nivel, 
					&pokemon->ataque, &pokemon->defensa) != PARAMETROS) {
				pokemon_destruir(pokemon);
				return NULL;
		}
		return pokemon;
}

int pokemon_nivel(pokemon_t *pokemon)
{	
		if (!pokemon) {
				return 0;
		}
		return pokemon->nivel;
}

int pokemon_ataque(pokemon_t *pokemon)
{	
		if (!pokemon) {
				return 0;
		}
		return pokemon->ataque;
}

int pokemon_defensa(pokemon_t *pokemon)
{	
		if (!pokemon) {
				return 0;
		}
		return pokemon->defensa;
}

const char *pokemon_nombre(pokemon_t *pokemon)
{	
		if (!pokemon) {
				return NULL;
		}
		return pokemon->nombre;
}

void pokemon_destruir(pokemon_t *pokemon)
{	
		if (pokemon) {
				free(pokemon);
		}
}
