#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "hash.h"
#include "menu.h"
#include "gestion.h"
#define MAX_LEER 30

typedef struct opcion{
	char *letra;
	char *descripcion;
	bool (*funcion)(gestion_t*);
    	gestion_t *gestion;
} opcion_t;

struct menu {
    	gestion_t* gestion;
    	hash_t *opciones;
    	bool estado;
};


menu_t *crear_menu(gestion_t *gestion) 
{   
    	if (!gestion) {
        	return NULL;
    	}

    	menu_t *menu = malloc(sizeof(menu_t));
    	if (!menu) {
        	return NULL;
    	}

    	menu->opciones = hash_crear(10);
    	if (!menu->opciones) {
        	free(menu);
        	return NULL;
    	}

    	menu->gestion = gestion;
    	menu->estado = true;
    	return menu;
}

menu_t* crear_opcion(menu_t *menu, char *let, char *desc,
		 bool (*f)(gestion_t*), gestion_t *gst)
{
	if (!menu || !let || !desc || !f || !gst) {
		return NULL;
	}

    	opcion_t *opcion = malloc(sizeof(opcion_t));
    	if (!opcion) {
        	return NULL;
    	}

    	opcion->letra = let;
    	opcion->descripcion = desc;
    	opcion->funcion = f;
    	opcion->gestion = gst;

    	hash_insertar(menu->opciones, opcion->letra, opcion, NULL);
	return menu;
}

void destruir_opcion(void* opcion)
{
    	free(opcion);
}

void destruir_menu(menu_t *menu)
{	
	if (!menu) {
		return;
	}
    	hash_destruir_todo(menu->opciones, destruir_opcion);
    	free(menu);
}

bool imprimir_opcion(const char *clave, void *valor, void *aux)
{      
    	opcion_t *opcion = valor;
    	printf("      %s - %s\n", opcion->letra, opcion->descripcion);
    	return true;
}

void imprimir_menu(menu_t* menu)
{   
    	printf("\n    _____Ingresar una opcion_____\n\n");
    	hash_con_cada_clave(menu->opciones, imprimir_opcion, NULL);
    	printf("    _____________________________\n\n");
}

char* leer_cadena(char* cadena)
{   
    	char* texto = fgets(cadena, MAX_LEER, stdin);

    	if (texto) {
        	texto[strlen(texto)-1] = 0;
    	}
    	return texto;
}

void correr_menu(menu_t *menu)
{   
	if (!menu) {
		return;
	}
    	while (menu->estado) {

        	imprimir_menu(menu);

        	char texto[MAX_LEER];
        	leer_cadena(texto);
        	texto[0] = (char)toupper(texto[0]);

        	opcion_t* opcion = hash_obtener(menu->opciones, texto);
            
        	if (opcion) {
            		menu->estado = opcion->funcion(opcion->gestion);
        	} else {
            		printf("\nOpcion no disponible\n");
        	}
	}
}