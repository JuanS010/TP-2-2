#include "src/gestion.h"
#include "src/menu.h"
#include <stdlib.h>
#include <stdbool.h>

int main(int argc, char *argv[]) 
{	

	gestion_t *gestion = iniciar_gestion(argc, argv);
	if (!gestion) {
		return -1;
	}

	menu_t* menu = crear_menu(gestion);
	if (!menu) {
		salir(gestion);
		return -1;
	}

	crear_opcion(menu, "D", "Mostrar caja", mostrar_caja, gestion);
	crear_opcion(menu, "M", "Combinar cajas", combinar_cajas, gestion);
	crear_opcion(menu, "C", "Cargar otra caja", cargar_otra_caja, gestion);
	crear_opcion(menu, "B", "Buscar caja", buscar_caja, gestion);
	crear_opcion(menu, "I", "Mostrar inventario", mostrar_inventario, gestion);
	crear_opcion(menu, "T", "Mostrar todo", mostrar_todo, gestion);
	crear_opcion(menu, "Q", "Salir", salir, gestion);

	correr_menu(menu);

	destruir_menu(menu);

	return 0;
}