#ifndef GESTION_H_
#define GESTION_H_

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

typedef struct gestion gestion_t;

/*
 Inicializa la gestion de cajas con todas sus estructuras.
 Recibe los parametros del programa con las cajas a cargar.
*/
gestion_t *iniciar_gestion(int argc, char *argv[]);

/*
 Muestra el contenido de todas las cajas disponibles.
*/
bool mostrar_todo(gestion_t *gestion);

/*
 Muestra un listado con los nombres de cajas disponibles y la cantidad de pokemones de cada caja.
*/
bool mostrar_inventario(gestion_t *gestion);

/*
  Pide un nombre de archivo e intenta cargar una caja. 
  Si el archivo ya fue cargado una vez, no se permite cargarlo nuevamente 
  (no puede haber dos cajas con el mismo nombre)
*/
bool cargar_otra_caja(gestion_t *gestion);

/*
 Pide dos nombres de cajas y un nombre de archivo. Combina las cajas y las guarda a un archivo. 
 La nueva caja generada también queda cargada en el sistema. 
 Tener en cuenta que el nombre de la nueva caja no puede ser repetido.
*/
bool combinar_cajas(gestion_t* gestion);

/*
 Pide un nombre de caja y muestra el contenido de la caja por pantalla
*/
bool mostrar_caja(gestion_t *gestion);

/*
 Pide un nombre de pokemon y muestra la cantidad de cajas que contienen dicho pokemon 
 y la lista con los nombres de las cajas.
*/
bool buscar_caja(gestion_t *gestion);

/*
 Sale del programa, liberando toda la memoria de la estructura gestion.
*/
bool salir(gestion_t *gestion);

#endif