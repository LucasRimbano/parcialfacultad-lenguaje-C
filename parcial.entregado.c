#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct farmaco {
    int codigo_producto;
    char nombre[50];
    float precio;
    int cantidad_disponible;
    int cantidad_vendida;
};

/* ------------------- HELPERS DE ENTRADA ------------------- */

void limpiar_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {
        /* descartar */
    }
}

void leer_linea(char *buffer, int tamanio) {
    if (fgets(buffer, tamanio, stdin) != NULL) {
        buffer[strcspn(buffer, "\n")] = '\0';  // eliminar '\n'
    }
}

/* ------------------- 1. INGRESAR PRODUCTOS ------------------- */

struct farmaco *ingresar_farmacos(struct farmaco *inventario, int *tamanio) {
    int opcion = 0;

    do {
        struct farmaco *temp = realloc(inventario, (*tamanio + 1) * sizeof(struct farmaco));
        if (temp == NULL) {
            printf("No se pudo asignar memoria...\n");
            return inventario;  // devolvemos el puntero anterior
        }
        inventario = temp;

        struct farmaco *nuevo = &inventario[*tamanio];

        printf("\n=== Alta de farmaco #%d ===\n", *tamanio + 1);

        printf("Codigo de producto: ");
        scanf("%d", &nuevo->codigo_producto);
        limpiar_buffer();

        printf("Nombre del farmaco: ");
        leer_linea(nuevo->nombre, sizeof(nuevo->nombre));

        printf("Precio: ");
        scanf("%f", &nuevo->precio);
        limpiar_buffer();

        printf("Cantidad disponible: ");
        scanf("%d", &nuevo->cantidad_disponible);
        limpiar_buffer();

        nuevo->cantidad_vendida = 0;  // al inicio, nada vendido

        (*tamanio)++;

        printf("\nSi quiere seguir ingresando farmacos ingrese cualquier numero menos 9.\n");
        printf("Si quiere terminar de ingresar farmacos ingrese 9.\n");
        printf("Opcion: ");
        scanf("%d", &opcion);
        limpiar_buffer();

    } while (opcion != 9);

    return inventario;
}

/* ------------------- 2. IMPRIMIR INVENTARIO ------------------- */

void imprimir_farmacos(struct farmaco *inventario, int tamanio) {
    if (tamanio == 0) {
        printf("\nNo hay farmacos cargados.\n");
        return;
    }

    printf("\n=== Inventario de farmacos ===\n");
    for (int i = 0; i < tamanio; i++) {
        printf("\nFarmaco #%d (posicion %d)\n", i + 1, i);
        printf("Codigo: %d\n", inventario[i].codigo_producto);
        printf("Nombre: %s\n", inventario[i].nombre);
        printf("Precio: $%.2f\n", inventario[i].precio);
        printf("Cantidad disponible: %d\n", inventario[i].cantidad_disponible);
        printf("Cantidad vendida: %d\n", inventario[i].cantidad_vendida);
    }
    printf("\n");
}

/* ------------------- 3. MODIFICAR POR NOMBRE ------------------- */

void modificar_farmaco_por_nombre(struct farmaco *inventario, int tamanio) {
    if (tamanio == 0) {
        printf("No hay farmacos para modificar.\n");
        return;
    }

    char nombre_buscar[50];
    printf("Ingrese el nombre del farmaco a modificar: ");
    leer_linea(nombre_buscar, sizeof(nombre_buscar));

    int encontrado = 0;
    for (int i = 0; i < tamanio; i++) {
        if (strcmp(inventario[i].nombre, nombre_buscar) == 0) {
            printf("\nFarmaco encontrado:\n");
            printf("Codigo actual: %d\n", inventario[i].codigo_producto);
            printf("Nombre: %s\n", inventario[i].nombre);
            printf("Precio: %.2f\n", inventario[i].precio);
            printf("Cantidad disponible: %d\n", inventario[i].cantidad_disponible);
            printf("Cantidad vendida: %d\n", inventario[i].cantidad_vendida);

            printf("\nIngrese nuevos datos (deje igual si desea repetir el valor actual):\n");

            printf("Nuevo precio (actual %.2f): ", inventario[i].precio);
            scanf("%f", &inventario[i].precio);
            limpiar_buffer();

            printf("Nueva cantidad disponible (actual %d): ", inventario[i].cantidad_disponible);
            scanf("%d", &inventario[i].cantidad_disponible);
            limpiar_buffer();

            printf("Nueva cantidad vendida (actual %d): ", inventario[i].cantidad_vendida);
            scanf("%d", &inventario[i].cantidad_vendida);
            limpiar_buffer();

            printf("Farmaco modificado correctamente.\n");
            encontrado = 1;
            break;
        }
    }

    if (!encontrado) {
        printf("No se encontro ningun farmaco con el nombre '%s'.\n", nombre_buscar);
    }
}

/* ------------------- 4. REGISTRAR VENTA ------------------- */

void vender_farmaco(struct farmaco *inventario, int tamanio) {
    if (tamanio == 0) {
        printf("No hay farmacos cargados para vender.\n");
        return;
    }

    int codigo, cantidad;
    printf("Ingrese el codigo del farmaco a vender: ");
    scanf("%d", &codigo);
    limpiar_buffer();

    printf("Ingrese la cantidad a vender: ");
    scanf("%d", &cantidad);
    limpiar_buffer();

    if (cantidad <= 0) {
        printf("Cantidad invalida.\n");
        return;
    }

    int encontrado = 0;
    for (int i = 0; i < tamanio; i++) {
        if (inventario[i].codigo_producto == codigo) {
            encontrado = 1;
            if (inventario[i].cantidad_disponible < cantidad) {
                printf("No hay stock suficiente. Disponible: %d\n",
                       inventario[i].cantidad_disponible);
                return;
            }

            inventario[i].cantidad_disponible -= cantidad;
            inventario[i].cantidad_vendida += cantidad;
            float total_venta = inventario[i].precio * cantidad;

            printf("Venta realizada correctamente.\n");
            printf("Producto: %s\n", inventario[i].nombre);
            printf("Cantidad vendida: %d\n", cantidad);
            printf("Total a pagar: $%.2f\n", total_venta);
            return;
        }
    }

    if (!encontrado) {
        printf("No se encontro un farmaco con el codigo %d.\n", codigo);
    }
}

/* ------------------- 5. TOTAL RECAUDADO ------------------- */

float total_recaudado(struct farmaco *inventario, int tamanio) {
    float total = 0.0f;
    for (int i = 0; i < tamanio; i++) {
        total += inventario[i].precio * inventario[i].cantidad_vendida;
    }
    return total;
}

/* ------------------- 6. ELIMINAR FARMACO ------------------- */

void eliminar_farmaco(struct farmaco **inventario, int *tamanio) {
    if (*tamanio == 0) {
        printf("No hay farmacos para eliminar.\n");
        return;
    }

    imprimir_farmacos(*inventario, *tamanio);

    int indice_a_eliminar;
    printf("Ingrese el numero de farmaco (1..%d) que desea eliminar: ", *tamanio);
    scanf("%d", &indice_a_eliminar);
    limpiar_buffer();

    if (indice_a_eliminar < 1 || indice_a_eliminar > *tamanio) {
        printf("Indice invalido.\n");
        return;
    }

    indice_a_eliminar--;  // lo convertimos a indice del vector (0..tamanio-1)

    for (int i = indice_a_eliminar; i < *tamanio - 1; i++) {
        (*inventario)[i] = (*inventario)[i + 1];
    }

    (*tamanio)--;

    if (*tamanio == 0) {
        free(*inventario);
        *inventario = NULL;
        printf("Se elimino el ultimo farmaco. Inventario vacio.\n");
        return;
    }

    struct farmaco *temp = realloc(*inventario, (*tamanio) * sizeof(struct farmaco));
    if (temp != NULL) {
        *inventario = temp;
    } else {
        printf("Advertencia: No se pudo ajustar la memoria, pero el registro fue eliminado.\n");
    }

    printf("Farmaco eliminado correctamente.\n");
}

/* ------------------- 7. FARMACO MAS VENDIDO ------------------- */

void farmaco_mas_vendido(struct farmaco *inventario, int tamanio) {
    if (tamanio == 0) {
        printf("No hay farmacos cargados.\n");
        return;
    }

    int indice_max = 0;
    for (int i = 1; i < tamanio; i++) {
        if (inventario[i].cantidad_vendida > inventario[indice_max].cantidad_vendida) {
            indice_max = i;
        }
    }

    printf("\nFarmaco mas vendido:\n");
    printf("Codigo: %d\n", inventario[indice_max].codigo_producto);
    printf("Nombre: %s\n", inventario[indice_max].nombre);
    printf("Cantidad vendida: %d\n", inventario[indice_max].cantidad_vendida);
    printf("Precio: $%.2f\n\n", inventario[indice_max].precio);
}

/* ------------------- 8. TXT PARA PEDIDOS (stock < 5) ------------------- */

void generar_pedido_distribuidora(struct farmaco *inventario, int tamanio) {
    FILE *texto = fopen("farmacos.txt", "w");
    if (texto == NULL) {
        printf("Error al abrir farmacos.txt para escritura.\n");
        return;
    }

    fprintf(texto, "Farmacos con stock menor a 5 (pedido a distribuidora):\n\n");

    int alguno = 0;
    for (int i = 0; i < tamanio; i++) {
        if (inventario[i].cantidad_disponible < 5) {
            alguno = 1;
            fprintf(texto, "Posicion en vector: %d\n", i);
            fprintf(texto, "Codigo: %d\n", inventario[i].codigo_producto);
            fprintf(texto, "Nombre: %s\n", inventario[i].nombre);
            fprintf(texto, "Precio: %.2f\n", inventario[i].precio);
            fprintf(texto, "Disponible: %d\n", inventario[i].cantidad_disponible);
            fprintf(texto, "Vendida: %d\n", inventario[i].cantidad_vendida);
            fprintf(texto, "------------------------\n");
        }
    }

    fclose(texto);

    if (alguno) {
        printf("Se genero el archivo farmacos.txt con el pedido.\n");
    } else {
        printf("No hay farmacos con stock menor a 5. farmacos.txt se genero vacio.\n");
    }
}

/* ------------------- 9. GUARDAR EN BINARIO ------------------- */

void guardar_en_binario(struct farmaco *inventario, int tamanio) {
    FILE *archivo = fopen("farmacos.dat", "wb");
    if (archivo == NULL) {
        printf("Error al abrir farmacos.dat para escritura.\n");
        return;
    }

    size_t escritos = fwrite(inventario, sizeof(struct farmaco), tamanio, archivo);
    fclose(archivo);

    if (escritos == (size_t)tamanio) {
        printf("Inventario guardado correctamente en farmacos.dat\n");
    } else {
        printf("Hubo un error al escribir en farmacos.dat\n");
    }
}

/* ------------------- 10. CARGAR DESDE BINARIO ------------------- */

struct farmaco *cargar_desde_binario(struct farmaco *inventario, int *tamanio) {
    FILE *archivo = fopen("farmacos.dat", "rb");
    if (archivo == NULL) {
        printf("No se pudo abrir farmacos.dat. Quizas no exista aun.\n");
        return inventario;
    }

    fseek(archivo, 0, SEEK_END);
    long bytes = ftell(archivo);
    if (bytes < 0) {
        printf("Error al obtener el tamano del archivo.\n");
        fclose(archivo);
        return inventario;
    }
    rewind(archivo);

    int cantidad = (int)(bytes / sizeof(struct farmaco));
    if (cantidad == 0) {
        printf("El archivo farmacos.dat esta vacio.\n");
        fclose(archivo);
        return inventario;
    }

    struct farmaco *nuevo = malloc(cantidad * sizeof(struct farmaco));
    if (nuevo == NULL) {
        printf("No se pudo asignar memoria para cargar los datos.\n");
        fclose(archivo);
        return inventario;
    }

    size_t leidos = fread(nuevo, sizeof(struct farmaco), cantidad, archivo);
    fclose(archivo);

    if (leidos != (size_t)cantidad) {
        printf("Error al leer los datos desde farmacos.dat\n");
        free(nuevo);
        return inventario;
    }

    // Liberamos el inventario anterior y reemplazamos
    free(inventario);
    inventario = nuevo;
    *tamanio = cantidad;

    printf("Se cargaron %d farmacos desde farmacos.dat\n", *tamanio);
    return inventario;
}

/* ------------------- PROGRAMA PRINCIPAL ------------------- */

int main() {
    struct farmaco *inventario = NULL;
    int tamanio = 0;
    int opcion_menu = 0;

    do {
        printf("\n=========== MENU FARMACIA ===========\n");
        printf("1. Ingresar farmacos\n");
        printf("2. Imprimir inventario\n");
        printf("3. Modificar farmaco por nombre\n");
        printf("4. Registrar venta\n");
        printf("5. Mostrar total recaudado\n");
        printf("6. Eliminar un farmaco\n");
        printf("7. Mostrar farmaco mas vendido\n");
        printf("8. Generar archivo farmacos.txt (stock < 5)\n");
        printf("9. Guardar inventario en binario (farmacos.dat)\n");
        printf("10. Cargar inventario desde binario (farmacos.dat)\n");
        printf("0. Salir\n");
        printf("Seleccione una opcion: ");
        scanf("%d", &opcion_menu);
        limpiar_buffer();

        switch (opcion_menu) {
            case 1:
                inventario = ingresar_farmacos(inventario, &tamanio);
                break;
            case 2:
                imprimir_farmacos(inventario, tamanio);
                break;
            case 3:
                modificar_farmaco_por_nombre(inventario, tamanio);
                break;
            case 4:
                vender_farmaco(inventario, tamanio);
                break;
            case 5: {
                float total = total_recaudado(inventario, tamanio);
                printf("Total recaudado en ventas: $%.2f\n", total);
                break;
            }
            case 6:
                eliminar_farmaco(&inventario, &tamanio);
                break;
            case 7:
                farmaco_mas_vendido(inventario, tamanio);
                break;
            case 8:
                generar_pedido_distribuidora(inventario, tamanio);
                break;
            case 9:
                guardar_en_binario(inventario, tamanio);
                break;
            case 10:
                inventario = cargar_desde_binario(inventario, &tamanio);
                break;
            case 0:
                printf("Saliendo del programa...\n");
                break;
            default:
                printf("Opcion invalida.\n");
                break;
        }

    } while (opcion_menu != 0);

    free(inventario);
    return 0;
}
