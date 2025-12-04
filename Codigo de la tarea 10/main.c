#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// --- Definición de Estructuras ---

// 2. Estructura para los datos del libro
typedef struct {
    char codigo[20];
    char titulo[100];
    char autor[100];
    char descripcion[200];
    int ano;
    float precio;
} Libro;

// 7. Estructura para la Lista Enlazada (Nodo)
typedef struct Nodo {
    Libro dato;
    struct Nodo* siguiente;
} Nodo;

// --- Prototipos de Funciones ---
void agregarLibro(Nodo** cabeza);
void mostrarLista(Nodo* cabeza);
void guardarEnArchivo(Nodo* cabeza);
void cargarDesdeArchivo(Nodo** cabeza);
void buscarLibro(Nodo* cabeza);
void liberarMemoria(Nodo* cabeza);
void limpiarBuffer();

// --- Función Principal ---
int main() {
    Nodo* lista = NULL; // Inicio de la lista enlazada
    int opcion;

    // 3. El programa lee la lista completa del disco al iniciar
    cargarDesdeArchivo(&lista);

    do {
        printf("\n--- GESTION DE LIBROS (TAREA 10) ---\n");
        printf("1. Agregar un libro\n");
        printf("2. Mostrar todos los libros\n");
        printf("3. Buscar libro (Titulo o Autor)\n");
        printf("4. Guardar y Salir\n");
        printf("Seleccione una opcion: ");
        scanf("%d", &opcion);
        limpiarBuffer(); // Consumir el 'enter' que queda en el buffer

        switch(opcion) {
            case 1:
                agregarLibro(&lista);
                break;
            case 2:
                mostrarLista(lista);
                break;
            case 3:
                buscarLibro(lista);
                break;
            case 4:
                // 1. Grabar la lista en memoria (al disco)
                guardarEnArchivo(lista);
                printf("Datos guardados. Saliendo...\n");
                break;
            default:
                printf("Opcion invalida.\n");
        }
    } while(opcion != 4);

    liberarMemoria(lista);
    return 0;
}

// --- Implementación de Funciones ---

// 6. El programa permite agregar libros
// 5. Todo realizado en memoria dinámica (malloc)
void agregarLibro(Nodo** cabeza) {
    // Reservamos memoria para un nuevo nodo
    Nodo* nuevoNodo = (Nodo*)malloc(sizeof(Nodo));

    if (nuevoNodo == NULL) {
        printf("Error: No hay memoria suficiente.\n");
        return;
    }

    printf("\n--- Nuevo Libro ---\n");
    printf("Codigo: ");
    fgets(nuevoNodo->dato.codigo, 20, stdin);
    nuevoNodo->dato.codigo[strcspn(nuevoNodo->dato.codigo, "\n")] = 0; // Quitar salto de linea

    printf("Titulo: ");
    fgets(nuevoNodo->dato.titulo, 100, stdin);
    nuevoNodo->dato.titulo[strcspn(nuevoNodo->dato.titulo, "\n")] = 0;

    printf("Autor: ");
    fgets(nuevoNodo->dato.autor, 100, stdin);
    nuevoNodo->dato.autor[strcspn(nuevoNodo->dato.autor, "\n")] = 0;

    printf("Descripcion: ");
    fgets(nuevoNodo->dato.descripcion, 200, stdin);
    nuevoNodo->dato.descripcion[strcspn(nuevoNodo->dato.descripcion, "\n")] = 0;

    printf("Año: ");
    scanf("%d", &nuevoNodo->dato.ano);

    printf("Precio: ");
    scanf("%f", &nuevoNodo->dato.precio);
    limpiarBuffer();

    // Insertar al inicio de la lista (más rápido y fácil)
    nuevoNodo->siguiente = *cabeza;
    *cabeza = nuevoNodo;

    printf("Libro agregado exitosamente.\n");
}

void mostrarLista(Nodo* cabeza) {
    if (cabeza == NULL) {
        printf("\nLa lista esta vacia.\n");
        return;
    }

    Nodo* temp = cabeza;
    printf("\n--- Lista de Libros ---\n");
    while (temp != NULL) {
        printf("Codigo: %s | Titulo: %s | Autor: %s | Precio: %.2f\n",
               temp->dato.codigo, temp->dato.titulo, temp->dato.autor, temp->dato.precio);
        temp = temp->siguiente;
    }
}

// 4. Búsqueda por título o autor
void buscarLibro(Nodo* cabeza) {
    char busqueda[100];
    int encontrados = 0;

    printf("Ingrese Titulo o Autor a buscar: ");
    fgets(busqueda, 100, stdin);
    busqueda[strcspn(busqueda, "\n")] = 0;

    Nodo* temp = cabeza;
    printf("\n--- Resultados de Busqueda ---\n");
    while (temp != NULL) {
        // strstr busca si la cadena 'busqueda' está dentro del titulo o autor (búsqueda parcial)
        // strcasecmp no es estándar en ANSI C, así que usamos strstr básico.
        if (strstr(temp->dato.titulo, busqueda) != NULL || strstr(temp->dato.autor, busqueda) != NULL) {
            printf("ENCONTRADO -> Titulo: %s, Autor: %s, Desc: %s, Año: %d\n",
                   temp->dato.titulo, temp->dato.autor, temp->dato.descripcion, temp->dato.ano);
            encontrados++;
        }
        temp = temp->siguiente;
    }

    if (encontrados == 0) {
        printf("No se encontraron libros con ese termino.\n");
    }
}

void guardarEnArchivo(Nodo* cabeza) {
    FILE* archivo = fopen("libros_db.txt", "w"); // "w" sobrescribe el archivo
    if (archivo == NULL) {
        printf("Error al abrir el archivo para guardar.\n");
        return;
    }

    Nodo* temp = cabeza;
    while (temp != NULL) {
        // Guardamos en formato CSV (separado por | para evitar problemas con comas en texto)
        fprintf(archivo, "%s|%s|%s|%s|%d|%.2f\n",
                temp->dato.codigo,
                temp->dato.titulo,
                temp->dato.autor,
                temp->dato.descripcion,
                temp->dato.ano,
                temp->dato.precio);
        temp = temp->siguiente;
    }
    fclose(archivo);
    printf("Base de datos actualizada en 'libros_db.txt'.\n");
}

void cargarDesdeArchivo(Nodo** cabeza) {
    FILE* archivo = fopen("libros_db.txt", "r");
    if (archivo == NULL) {
        // Si no existe el archivo, no pasa nada, empezamos con lista vacía
        return;
    }

    char linea[500];
    while (fgets(linea, sizeof(linea), archivo)) {
        Nodo* nuevoNodo = (Nodo*)malloc(sizeof(Nodo));
        if (nuevoNodo == NULL) break;

        // Parsear la línea (separar por |)
        // Nota: sscanf con formato %[^\t] lee hasta encontrar el delimitador
        char *token;

        token = strtok(linea, "|");
        strcpy(nuevoNodo->dato.codigo, token);

        token = strtok(NULL, "|");
        strcpy(nuevoNodo->dato.titulo, token);

        token = strtok(NULL, "|");
        strcpy(nuevoNodo->dato.autor, token);

        token = strtok(NULL, "|");
        strcpy(nuevoNodo->dato.descripcion, token);

        token = strtok(NULL, "|");
        nuevoNodo->dato.ano = atoi(token);

        token = strtok(NULL, "|");
        nuevoNodo->dato.precio = atof(token);

        // Insertar en la lista
        nuevoNodo->siguiente = *cabeza;
        *cabeza = nuevoNodo;
    }
    fclose(archivo);
    printf("Datos cargados del disco correctamente.\n");
}

void liberarMemoria(Nodo* cabeza) {
    Nodo* temp;
    while (cabeza != NULL) {
        temp = cabeza;
        cabeza = cabeza->siguiente;
        free(temp); // Liberar la memoria de cada nodo
    }
}

void limpiarBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}
