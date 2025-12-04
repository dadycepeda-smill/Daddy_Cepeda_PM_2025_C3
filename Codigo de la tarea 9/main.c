#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ==========================================
// DEFINICIÓN DE ESTRUCTURAS DE DATOS
// ==========================================

// 1. Estructura para Lista Enlazada
struct NodoLista {
    int dato;
    struct NodoLista* siguiente;
};

// 2. Estructura para Árbol Binario
struct NodoArbol {
    int dato;
    struct NodoArbol* izquierda;
    struct NodoArbol* derecha;
};

// ==========================================
// FUNCIONES DE UTILIDAD
// ==========================================
void pausar() {
    printf("\nPresiona ENTER para volver al menu...");
    while(getchar() != '\n'); // Limpiar buffer
    getchar(); // Esperar enter
}

// ==========================================
// EJEMPLOS DE LA TAREA (PARTE 1)
// ==========================================

void ejemplo_lista_enlazada() {
    printf("\n--- 1. EJEMPLO LISTA ENLAZADA ---\n");
    // Crear nodos dinámicamente
    struct NodoLista* cabeza = (struct NodoLista*)malloc(sizeof(struct NodoLista));
    struct NodoLista* segundo = (struct NodoLista*)malloc(sizeof(struct NodoLista));
    struct NodoLista* tercero = (struct NodoLista*)malloc(sizeof(struct NodoLista));

    // Asignar datos
    cabeza->dato = 10;
    cabeza->siguiente = segundo;

    segundo->dato = 20;
    segundo->siguiente = tercero;

    tercero->dato = 30;
    tercero->siguiente = NULL;

    // Imprimir lista recorriéndola
    struct NodoLista* actual = cabeza;
    while (actual != NULL) {
        printf("Nodo: %d -> ", actual->dato);
        actual = actual->siguiente;
    }
    printf("NULL\n");
}

void ejemplo_arbol() {
    printf("\n--- 2. EJEMPLO ARBOL BINARIO ---\n");
    // Crear nodos
    struct NodoArbol* raiz = (struct NodoArbol*)malloc(sizeof(struct NodoArbol));
    struct NodoArbol* hijoIzq = (struct NodoArbol*)malloc(sizeof(struct NodoArbol));
    struct NodoArbol* hijoDer = (struct NodoArbol*)malloc(sizeof(struct NodoArbol));

    // Configurar Raíz (10) con hijos (5 y 15)
    raiz->dato = 10;
    raiz->izquierda = hijoIzq;
    raiz->derecha = hijoDer;

    hijoIzq->dato = 5;
    hijoIzq->izquierda = NULL;
    hijoIzq->derecha = NULL;

    hijoDer->dato = 15;
    hijoDer->izquierda = NULL;
    hijoDer->derecha = NULL;

    printf("       %d (Raiz)\n", raiz->dato);
    printf("      /  \\\n");
    printf("     %d    %d\n", raiz->izquierda->dato, raiz->derecha->dato);
}

void ejemplo_grafo() {
    printf("\n--- 3. EJEMPLO GRAFO (Matriz de Adyacencia) ---\n");
    // Grafo simple de 3 nodos (0, 1, 2)
    int grafo[3][3] = {
        {0, 1, 1}, // El 0 conecta con 1 y 2
        {1, 0, 0}, // El 1 conecta con 0
        {1, 0, 0}  // El 2 conecta con 0
    };

    printf("Conexiones:\n");
    for(int i=0; i<3; i++) {
        for(int j=0; j<3; j++) {
            if(grafo[i][j] == 1) {
                printf("Nodo %d esta conectado con Nodo %d\n", i, j);
            }
        }
    }
}

void ejemplo_tabla_hash() {
    printf("\n--- 4. EJEMPLO TABLA HASH ---\n");
    int tabla[10];
    // Inicializar vacía (-1)
    for(int i=0; i<10; i++) tabla[i] = -1;

    // Insertar datos (Llave % 10)
    int valor1 = 105; // 105 % 10 = índice 5
    int valor2 = 22;  // 22 % 10 = índice 2

    tabla[valor1 % 10] = valor1;
    tabla[valor2 % 10] = valor2;

    printf("Insertando 105 en indice 5...\n");
    printf("Insertando 22 en indice 2...\n");
    printf("Estado de la tabla:\n");
    for(int i=0; i<10; i++) {
        if(tabla[i] != -1) printf("[%d]: %d\n", i, tabla[i]);
        else printf("[%d]: Vacio\n", i);
    }
}

void ejemplo_pila() {
    printf("\n--- 5. EJEMPLO PILA (Stack - LIFO) ---\n");
    int pila[5];
    int tope = -1;

    // Push (Apilar)
    printf("Push 10\n"); pila[++tope] = 10;
    printf("Push 20\n"); pila[++tope] = 20;
    printf("Push 30\n"); pila[++tope] = 30;

    // Pop (Desapilar)
    printf("Sacando elemento (Pop): %d\n", pila[tope--]); // Sale 30
    printf("Sacando elemento (Pop): %d\n", pila[tope--]); // Sale 20
    printf("Queda en el tope: %d\n", pila[tope]);
}

void ejemplo_cola() {
    printf("\n--- 6. EJEMPLO COLA (Queue - FIFO) ---\n");
    int cola[5];
    int frente = 0, final = -1, items = 0;

    // Enqueue
    printf("Encolando Cliente 1...\n");
    cola[++final] = 1; items++;
    printf("Encolando Cliente 2...\n");
    cola[++final] = 2; items++;

    // Dequeue
    printf("Atendiendo (Dequeue): Cliente %d\n", cola[frente++]); items--;
    printf("Atendiendo (Dequeue): Cliente %d\n", cola[frente++]); items--;
}

// ==========================================
// PARTE 2: EJEMPLO FUNCIONAL COMPLETO
// ==========================================

#define MAX_HISTORIAL 50
#define LARGO_URL 100

// Variables globales para el navegador
char historial[MAX_HISTORIAL][LARGO_URL];
int tope_historial = -1;
char pagina_actual[LARGO_URL] = "";

void visitar(char* url) {
    if (strlen(pagina_actual) > 0) {
        tope_historial++;
        strcpy(historial[tope_historial], pagina_actual);
    }
    strcpy(pagina_actual, url);
    printf(" [Navegador] Entrando a: %s\n", pagina_actual);
}

void atras() {
    if (tope_historial >= 0) {
        strcpy(pagina_actual, historial[tope_historial]);
        tope_historial--;
        printf(" [Navegador] << Regresando a: %s\n", pagina_actual);
    } else {
        printf(" [Navegador] X No hay historial previo.\n");
    }
}

void ejemplo_funcional_navegador() {
    printf("\n--- 7. EJEMPLO FUNCIONAL: NAVEGADOR WEB (USANDO PILAS) ---\n");
    // Resetear variables por si se ejecuta varias veces
    tope_historial = -1;
    strcpy(pagina_actual, "");

    printf("1. El usuario abre Google...\n");
    visitar("www.google.com");

    printf("2. El usuario busca videos y va a YouTube...\n");
    visitar("www.youtube.com");

    printf("3. El usuario entra a un repositorio en GitHub...\n");
    visitar("www.github.com");

    printf("\n--- El usuario presiona el boton 'ATRAS' ---\n");
    atras(); // Debe volver a YouTube

    printf("\n--- El usuario presiona el boton 'ATRAS' otra vez ---\n");
    atras(); // Debe volver a Google

    printf("\n--- El usuario presiona 'ATRAS' una vez mas ---\n");
    atras(); // Historial vacío o página de inicio
}

// ==========================================
// MAIN (MENU PRINCIPAL)
// ==========================================
int main() {
    int opcion;

    do {
        // Limpiar pantalla (opcional, system("cls") es para Windows)
        // system("cls");

        printf("\n=========================================\n");
        printf(" TAREA 9: ESTRUCTURAS DE DATOS (EN C)\n");
        printf("=========================================\n");
        printf("1. Lista Enlazada\n");
        printf("2. Arbol\n");
        printf("3. Grafo\n");
        printf("4. Tabla Hash\n");
        printf("5. Pila\n");
        printf("6. Cola\n");
        printf("7. Ejemplo Funcional (Navegador Web)\n");
        printf("0. Salir\n");
        printf("=========================================\n");
        printf("Elige una opcion: ");
        scanf("%d", &opcion);

        switch(opcion) {
            case 1: ejemplo_lista_enlazada(); pausar(); break;
            case 2: ejemplo_arbol(); pausar(); break;
            case 3: ejemplo_grafo(); pausar(); break;
            case 4: ejemplo_tabla_hash(); pausar(); break;
            case 5: ejemplo_pila(); pausar(); break;
            case 6: ejemplo_cola(); pausar(); break;
            case 7: ejemplo_funcional_navegador(); pausar(); break;
            case 0: printf("Saliendo...\n"); break;
            default: printf("Opcion no valida.\n"); pausar(); break;
        }

    } while(opcion != 0);

    return 0;
}
