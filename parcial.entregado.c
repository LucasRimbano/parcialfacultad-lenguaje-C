#include<stdio.h>
#include<string.h>
#include<stdlib.h>

struct pizza {
int id_pizza;
char nombre[50];
float costo;
float precio;
int cantidad_vendida;
};

struct pizza *ingresar_datos(struct pizza *inventario, int *tamanio) {
    int opcion = 0;
    do {
        struct pizza *temp = realloc(inventario, (*tamanio + 1) * sizeof(struct pizza));
        if (temp == NULL) {
            printf("No se pudo asignar memoria...\n");
            return inventario;
        }
        inventario = temp;

        printf("Pedido numero: %d \n", *tamanio + 1);
        printf("Ingrese el ID de la pizza: \n");
        scanf("%d", &inventario[*tamanio].id_pizza);
        getchar();

        printf("Ingrese su nombre: \n");
        fgets(inventario[*tamanio].nombre, sizeof(inventario[*tamanio].nombre), stdin);
        inventario[*tamanio].nombre[strcspn(inventario[*tamanio].nombre, "\n")] = '\0';
        printf("Ingrese el costo de la pizza: \n");
        scanf("%f", &inventario[*tamanio].costo);
        getchar();

        printf("Ingrese el precio de la pizza: \n");
        scanf("%f", &inventario[*tamanio].precio);
        getchar();
        printf("Ingrese la cantidad vendida: \n");
        scanf("%d" , &inventario[*tamanio].cantidad_vendida);

        (*tamanio)++;

        printf("Si quiere seguir ingresando pizzas ingrese cualquier n�mero menos el 9.\n");
        printf("Si quiere terminar de ingresar pizzas ingrese el 9.\n");
        scanf("%d", &opcion);

    } while (opcion != 9);
    return inventario;
}


void Imprimir_valores(struct pizza *inventario , int tamanio){


for (int i = 0 ; i < tamanio ; i++){

    printf("Pizza numero: %d \n" , i +1);
    printf("Su id es: %d \n", inventario[i].id_pizza);
    printf("Su nombre es: %s ",  inventario[i].nombre);
    printf("El costo x pizza es: %0.2f \n" , inventario[i].costo);
    printf("El precio x pizza es: %0.2f \n", inventario[i].precio);
    printf("La cantidad vendida es: %d \n" ,inventario[i].cantidad_vendida);

}


}
void actualizar_precio(struct pizza *inventario, int tamanio) {
    int id_a_buscar;
    int porcentaje_incremento;

    printf("Ingrese el ID de la pizza a actualizar:\n");
    scanf("%d", &id_a_buscar);
    getchar();

    printf("Ingrese el porcentaje  (entre 1 y 100):\n");
    scanf("%d", &porcentaje_incremento);
    getchar();

    if (porcentaje_incremento < 1 || porcentaje_incremento > 100) {
        printf("Error: Ingresa un porcentaje que este bien entre 1 y 100.\n");
        return;
    }

    int encontrado = 0;
    for (int i = 0; i < tamanio; i++) {
        if (inventario[i].id_pizza == id_a_buscar) {
            inventario[i].precio *= (1 + porcentaje_incremento / 100.0);
            printf("Precio actualizado con ID %d: %0.2f\n", id_a_buscar, inventario[i].precio);
            encontrado = 1;
            break;
        }
    }

    if (!encontrado) {
        printf("Error: No se encontro una pizza con el id que estaba buscando... %d.\n", id_a_buscar);
    }
}


float calcular_promedio_ventas(struct pizza *inventario, int tamanio) {
    if (tamanio == 0) return 0; // Evitar divisi�n por cero
    int suma_ventas = 0;
    for (int i = 0; i < tamanio; i++) {
        suma_ventas += inventario[i].cantidad_vendida;
    }
    return (float)suma_ventas / tamanio;
}



void eliminar_pizza(struct pizza **inventario , int *tamanio){

    Imprimir_valores(*inventario,*tamanio);
    int indice_a_eliminar = 0 ;
    printf("Ingrese en numero de pizza que quiere eliminar de la lista... \n");
    scanf("%d" , &indice_a_eliminar);
    indice_a_eliminar--;

    for (int i = indice_a_eliminar ; i < *tamanio  - 1; i ++){
             (*inventario)[i] = (*inventario)[i+1];


    }
    (*tamanio)--;

    struct pizza *temp = realloc(*inventario, (*tamanio) * sizeof(struct pizza));
    if (temp != NULL) {
        *inventario = temp;
    } else {
        printf("no se puede asignar memoria... \n");
    }
    Imprimir_valores(*inventario, *tamanio);
}


void archivos_binario(struct pizza *inventario ,  int tamanio){

    FILE *ARCHIVO_BIN;
    ARCHIVO_BIN = fopen("pizzas.dat", "wr");
    if (ARCHIVO_BIN == NULL) {
    printf("Hubo un error de carga de memoria lo siento.... \n");
    return;
    }




    fwrite(inventario, sizeof(struct pizza),tamanio,ARCHIVO_BIN);
    fclose(ARCHIVO_BIN);
    printf("Archivos  binario guardado correctamente... \n");
}


void archivo_texto (struct pizza *inventario , int tamanio){

FILE *texto ;
texto = fopen("reporte.txt", "w");
if (texto == NULL) {
    printf("Lo siento errror de la memoria ... \n");
    return;


}
 fprintf(texto,"productos estrellas: \n");

 for (int i = 0; i < tamanio; i++) {
        if (inventario[i].cantidad_vendida> 50) {


        fprintf(texto,"Numero de pizza: %d \n" ,  i +1);
        fprintf(texto ,"ID NUMERO: %d \n", inventario[i].id_pizza);
        fprintf(texto,"Su nombre es: %s \n" , inventario[i].nombre);
        fprintf(texto,"El costo es de: %0.2f \n",inventario[i].costo);
        fprintf(texto,"El precio es de: %0.2f \n", inventario[i].precio);
        fprintf(texto,"La cantidad vendida es de: %d \n" , inventario[i].cantidad_vendida);
        fprintf(texto, "------------------------- \n");
        }
    }




fclose(texto);
printf("se han cargado los productos estrellas en archivo de txt...\n");
}

int main (){
struct pizza *inventario = NULL;
int tamanio = 0 ;
 int opcion_menu = 0;


 do {
        printf("Ingrese un numero segun lo que quieras elejir... \n");
        printf("1. Ingresar datos. \n");
        printf("2. Imprimir datos. \n");
        printf("3. Actualizar precio \n");
        printf("4. Total ingresos  total promedio ventas etc... \n");
        printf("5. Eliminar un elemento. \n");
        printf("6. Imprimir pizzas en archivo texto \n");
        printf("7. Guardar elementos en binario. \n");
        printf("9. Salir del programa... \n");
        printf("Ingrese un numero segun lo que quiera.... \n");
        scanf("%d" , &opcion_menu);
        getchar();
       switch(opcion_menu){
         case 1 :
          inventario = ingresar_datos(inventario,&tamanio);
          break;
         case 2:
          Imprimir_valores(inventario,tamanio);
          break;
         case 3:
            actualizar_precio(inventario,tamanio);
            break;
//         case 4:
//            float promedio_ventas = calcular_promedio_ventas(inventario, tamanio);
            break;
         case 5:
          eliminar_pizza(&inventario,&tamanio);
          break;
         case 6:
            archivo_texto(inventario,tamanio);
            break;
         case 7:
             archivos_binario(inventario,tamanio);
            break;
         case 9:
          printf("SALIENDO DEL PROGRAMA....\n");
           break;
       }



 }while(opcion_menu != 9 );

free(inventario);
return 0;
}
