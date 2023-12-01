#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "gestion.h"

typedef struct menu menu_t;

/*
 Crea un menu y lo devuelve, en caso de error devuelve null.
 Recibe una estructura de gestion de cajas
 y genera un menu para controlar la misma.
*/
menu_t *crear_menu(gestion_t *gestion);

/*
 Crea una opcion para el menu.
 Recibe el caracter a ingresar para la opcion, su descripcion 
 y la funcion que se llama al seleccionarla.
 En caso de no recibir algun parametro devuelve null
*/
menu_t *crear_opcion(menu_t *menu, char *let, char *desc, bool (*f)(gestion_t*), gestion_t* gst);

/*
 Destruye la estructura del menu y todas las opciones generadas para la misma.
*/
void destruir_menu(menu_t *menu);

/*
 Empeiza el funcionamiento del menu hasta que una de las opciones devuelva false,
 caso contrario seguira funcionando continuamente.
*/
void correr_menu(menu_t *menu);



