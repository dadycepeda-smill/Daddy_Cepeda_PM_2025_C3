#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// --- Estructuras ---
typedef struct {
    char codigo[20];
    char titulo[100];
    char autor[100];
    char descripcion[200];
    int ano;
    float precio;
} Libro;

typedef struct Nodo {
    Libro dato;
    struct Nodo* siguiente;
} Nodo;

// --- Prototipos ---
void agregarLibro(Nodo** cabeza);
void mostrarLista(Nodo* cabeza);
void guardarEnArchivo(Nodo* cabeza);
void cargarDesdeArchivo(Nodo** cabeza);
void buscarLibro(Nodo* cabeza);      // Cumple requisito 1
void ordenarPorPrecio(Nodo* cabeza); // Cumple requisito 2 (BubbleSort)
void liberarMemoria(Nodo* cabeza);
void limpiarBuffer();

// --- Main ---
int main() {
    Nodo* lista = NULL;
    int opcion;

    cargarDesdeArchivo(&lista);

    do {
        printf("\n--- GESTION DE LIBROS (TAREA ACTUALIZADA) ---\n");
        printf("1. Agregar un libro\n");
        printf("2. Mostrar lista de libros (Requisito 3)\n");
        printf("3. Buscar por Titulo o Autor (Requisito 1)\n");
        printf("4. Ordenar lista por PRECIO - BubbleSort (Requisito 2)\n");
        printf("5. Guardar y Salir\n");
        printf("Seleccione: ");
        scanf("%d", &opcion);
        limpiarBuffer();

        switch(opcion) {
            case 1: agregarLibro(&lista); break;
            case 2: mostrarLista(lista); break;
            case 3: buscarLibro(lista); break;
            case 4:
                ordenarPorPrecio(lista);
                mostrarLista(lista); // Mostramos la lista automáticamente tras ordenar
                break;
            case 5:
                guardarEnArchivo(lista);
                printf("Guardado. Adios!\n");
                break;
            default: printf("Opcion invalida.\n");
        }
    } while(opcion != 5);

    liberarMemoria(lista);
    return 0;
}

// --- Implementaciones ---

void agregarLibro(Nodo** cabeza) {
    Nodo* nuevo = (Nodo*)malloc(sizeof(Nodo));
    if (!nuevo) return;

    printf("\n--- Nuevo Libro ---\n");
    printf("Codigo: "); fgets(nuevo->dato.codigo, 20, stdin);
    nuevo->dato.codigo[strcspn(nuevo->dato.codigo, "\n")] = 0;

    printf("Titulo: "); fgets(nuevo->dato.titulo, 100, stdin);
    nuevo->dato.titulo[strcspn(nuevo->dato.titulo, "\n")] = 0;

    // Requisito 1: Aseguramos tener el campo autor bien definido
    printf("Autor: "); fgets(nuevo->dato.autor, 100, stdin);
    nuevo->dato.autor[strcspn(nuevo->dato.autor, "\n")] = 0;

    printf("Descripcion: "); fgets(nuevo->dato.descripcion, 200, stdin);
    nuevo->dato.descripcion[strcspn(nuevo->dato.descripcion, "\n")] = 0;

    printf("Año: "); scanf("%d", &nuevo->dato.ano);
    printf("Precio: "); scanf("%f", &nuevo->dato.precio);
    limpiarBuffer();

    nuevo->siguiente = *cabeza;
    *cabeza = nuevo;
    printf("Libro agregado.\n");
}

void mostrarLista(Nodo* cabeza) {
    if (cabeza == NULL) {
        printf("\nLista vacia.\n");
        return;
    }
    Nodo* temp = cabeza;
    printf("\n--- Lista de Libros ---\n");
    while (temp != NULL) {
        printf("Precio: $%.2f | Titulo: %s | Autor: %s\n",
               temp->dato.precio, temp->dato.titulo, temp->dato.autor);
        temp = temp->siguiente;
    }
}

// REQUISITO 1: Búsqueda por Autor (y título)
void buscarLibro(Nodo* cabeza) {
    char busqueda[100];
    int encontrados = 0;

    printf("Ingrese el Autor (o titulo) a buscar: ");
    fgets(busqueda, 100, stdin);
    busqueda[strcspn(busqueda, "\n")] = 0;

    Nodo* temp = cabeza;
    printf("\n--- Resultados ---\n");
    while (temp != NULL) {
        // Buscamos si el texto ingresado aparece en el Autor O en el Título
        if (strstr(temp->dato.autor, busqueda) != NULL || strstr(temp->dato.titulo, busqueda) != NULL) {
            printf("ENCONTRADO -> Autor: %s | Titulo: %s | Precio: %.2f\n",
                   temp->dato.autor, temp->dato.titulo, temp->dato.precio);
            encontrados++;
        }
        temp = temp->siguiente;
    }
    if (encontrados == 0) printf("No se encontraron coincidencias.\n");
}

// REQUISITO 2: BubbleSort por Precio
void ordenarPorPrecio(Nodo* cabeza) {
    if (cabeza == NULL) return; // Lista vacía, nada que ordenar

    int intercambiado;
    Nodo* actual;
    Nodo* ultimo = NULL; // Marca hasta dónde la lista ya está ordenada

    printf("\nOrdenando lista por precio (BubbleSort)...\n");

    do {
        intercambiado = 0;
        actual = cabeza;

        // Recorremos la lista hasta llegar al último nodo ordenado
        while (actual->siguiente != ultimo) {

            // CRITERIO: Si el precio actual es mayor al siguiente, INTERCAMBIAMOS
            if (actual->dato.precio > actual->siguiente->dato.precio) {

                // Intercambio de DATOS (Swapping)
                Libro temp = actual->dato;
                actual->dato = actual->siguiente->dato;
                actual->siguiente->dato = temp;

                intercambiado = 1; // Marcamos que hubo un cambio para repetir el ciclo
            }
            actual = actual->siguiente;
        }
        // El último elemento visitado ya está en su posición final
        ultimo = actual;

    } while (intercambiado); // Repetimos hasta que no hagamos ningún cambio en una pasada
}

void guardarEnArchivo(Nodo* cabeza) {
    FILE* archivo = fopen("libros_db.txt", "w");
    if (!archivo) return;

    Nodo* temp = cabeza;
    while (temp != NULL) {
        fprintf(archivo, "%s|%s|%s|%s|%d|%.2f\n",
                temp->dato.codigo, temp->dato.titulo, temp->dato.autor,
                temp->dato.descripcion, temp->dato.ano, temp->dato.precio);
        temp = temp->siguiente;
    }
    fclose(archivo);
}

void cargarDesdeArchivo(Nodo** cabeza) {
    FILE* archivo = fopen("libros_db.txt", "r");
    if (!archivo) return;

    char linea[500];
    while (fgets(linea, sizeof(linea), archivo)) {
        Nodo* nuevo = (Nodo*)malloc(sizeof(Nodo));
        char *token = strtok(linea, "|");
        strcpy(nuevo->dato.codigo, token);
        token = strtok(NULL, "|"); strcpy(nuevo->dato.titulo, token);
        token = strtok(NULL, "|"); strcpy(nuevo->dato.autor, token);
        token = strtok(NULL, "|"); strcpy(nuevo->dato.descripcion, token);
        token = strtok(NULL, "|"); nuevo->dato.ano = atoi(token);
        token = strtok(NULL, "|"); nuevo->dato.precio = atof(token);

        nuevo->siguiente = *cabeza;
        *cabeza = nuevo;
    }
    fclose(archivo);
}

void liberarMemoria(Nodo* cabeza) {
    Nodo* temp;
    while (cabeza != NULL) {
        temp = cabeza;
        cabeza = cabeza->siguiente;
        free(temp);
    }
}

void limpiarBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}
