#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "malloc.h"
#include <ctype.h>
#define DIM 30
#define NOMBREARCHI "notasAlumnos.bin"
#define APROBADOS "notasAlumnosAprobados.bin"
///ESTRUCTURAS:
typedef struct registroArchivo {
   int nota;
   int legajo;
   char nombreYApellido[40];
   char materia[40];
   int idMateria;
} registroArchivo;

typedef struct notaAlumno {
   int nota;
   int legajo;
   char nombreYApellido[40];
} notaAlumno;

typedef struct nodo {
   notaAlumno dato;
   struct nodo *siguiente;
} nodo;

typedef struct celda {
    int idMateria;
    char materia[40];
    nodo * listaDeNotas;
} celda;

///PROTOTIPADO FUNCIONES:
nodo *inicLista();
nodo *crearNodo(notaAlumno nota);
nodo *agregarNodoPrincipio(nodo *lista, nodo *nodoNota);
void crearMaterias(celda materias[], int *validos);
celda cargarUnaMateria();
void crearUnaSolaMateria(celda materias[], int *validos, char materiaAgregar[]);
celda cargarUnaSolaMateria(char materiaAgregar[]);
notaAlumno cargarAlumno();
int buscarMateria(celda materias[], char materiaBuscar[], int validos);
void mostrarSoloMaterias(celda materias[], int validos);
void mostrarMateriaConNotas(celda materias[], char materiaMostrar[], int validos);
void mostrarNotasDeEsaMateria(celda materias[], int pos);
void pasarMateriasConAlumnosArchivo(celda materias[], int validos);
void mostrarArchivo();
void printfDeMostrarArchi(registroArchivo registro);
void pasarDelArchivoACeldasDeListas(celda materias[], int *validos);

int main(){
    ///DECLARACIONES
     celda materias[DIM];
     int validos = 0;

//     crearMaterias(materias, &validos);
//     printf("\nMostrando las materias cargadas: \n");
//     mostrarSoloMaterias(materias, validos);
//
//     char eleccion = 's';
//     while(eleccion == 's'){
//        agregarNotaAlumnoaMateria(materias, &validos);
//        printf("\nPulsa 's' si desea agregar otro alumno. \n");
//        fflush(stdin);
//        scanf("%c", &eleccion);
//     }
//
//     char materiaMostrar[DIM];
//     printf("Ingrese la materia a mostrar con sus notas: ");
//     fflush(stdin);
//     gets(materiaMostrar);
//     mostrarMateriaConNotas(materias, materiaMostrar, validos);
//
//     pasarMateriasConAlumnosArchivo(materias, validos);
     printf("\nMostrando el contenido del archivo.\n");
     mostrarArchivo();

     pasarDelArchivoACeldasDeListas(materias, &validos);

     char materiaMostrar2[DIM];
     printf("Ingrese la materia a mostrar con sus notas: ");
     fflush(stdin);
     gets(materiaMostrar2);
     mostrarMateriaConNotas(materias, materiaMostrar2, validos);

}
///FUNCIONES

///FUNCIONES DE NODOS:
nodo *inicLista(){
    return NULL;
}
void inicListaNotas(nodo **lista){
    *lista = NULL;
}
nodo *crearNodo(notaAlumno nota){
    nodo *aux = (nodo*)malloc(sizeof(nodo));
    aux->dato = nota;
    aux->siguiente = NULL;
    return aux;
}

nodo *agregarNodoPrincipio(nodo *lista, nodo *nodoNota){
    if(lista == NULL) lista = nodoNota;
    else{
        nodoNota->siguiente = lista;
        lista = nodoNota;
    }
    return lista;
}
//Crear materias hasta que el usuario quiera.
void crearMaterias(celda materias[], int *validos){ ///para mostrar las materias con validos - 1
    char eleccion = 's';
    while(eleccion == 's' && *validos < DIM){
        materias[*validos] = cargarUnaMateria();
        *validos = *validos + 1;

        printf("\nIngrese 's' para continuar: ");
        fflush(stdin);
        scanf("%c", &eleccion);
    }
}
//cargar una materia que nos devuelve una materia
celda cargarUnaMateria(){
        celda unaMateria;

        char eleccion = 's';
        printf("\nIngrese la materia: ");
        fflush(stdin);
        gets(unaMateria.materia);

        printf("Ingrese el ID de la Materia: ");
        fflush(stdin);
        scanf("%i", &unaMateria.idMateria);

        unaMateria.listaDeNotas = inicLista();

        printf("\nPulse 's' si esta seguro que desea cargar la materia %s: ", unaMateria.materia);
        fflush(stdin);
        scanf("%c", &eleccion);

        if(eleccion == 's'){
             return unaMateria;
        }
}

//crear una sola materia
void crearUnaSolaMateria(celda materias[], int *validos, char materiaAgregar[]){
     materias[*validos] = cargarUnaSolaMateria(materiaAgregar);
    *validos = *validos + 1;
}
//print para crear una sola materia
celda cargarUnaSolaMateria(char materiaAgregar[]){
    celda unaMateria;
    strcpy(unaMateria.materia, materiaAgregar);

    printf("Ingrese el ID de la Materia: ");
    fflush(stdin);
    scanf("%i", &unaMateria.idMateria);

    unaMateria.listaDeNotas = inicLista();

    return unaMateria;
}

//agregar alumno a materia
void agregarNotaAlumnoaMateria(celda materias[], int *validos){
    char materiaAlumno[DIM];
    char eleccion = 's'; /// VALIDACION PARA CREAR LA MATERIA
    printf("Materia a cargargarle el alumno: "); ///Pedimos la materia para luego cargarle la nota del alumno
    fflush(stdin);
    gets(materiaAlumno);

    int posMateria;
    posMateria = buscarMateria(materias,materiaAlumno, validos); ///Buscamos la materia para cargar el alumno

    notaAlumno notaAgregar = cargarAlumno();///Cargamos los datos de la nota del alumno
    nodo *auxNota = crearNodo(notaAgregar); ///Creamos el nodo con la nota del alumno

    if(posMateria == -1){///Es porque la materia no existe, hay que crearla

        printf("La materia no existe. Pulsa 's' si desea agregarla: ");
        fflush(stdin);
        scanf("%c", &eleccion);
        if(eleccion == 's'){ ///VALIDACION PARA VERIFICAR SI QUIERE CREAR UNA NUEVA MATERIA
            crearUnaSolaMateria(materias, validos, materiaAlumno);
            materias[*validos].listaDeNotas = agregarNodoPrincipio(materias[*validos].listaDeNotas, auxNota);
            }
        }else{///Es porque encontro la materia y tiene la posicion en la cual le insertaremos la nota del alumno
            materias[posMateria].listaDeNotas = agregarNodoPrincipio(materias[posMateria].listaDeNotas, auxNota);
            printf("\nAlumno cargado con exito.\n");
        }

}

//crear alumno
notaAlumno cargarAlumno(){
    notaAlumno nota;
    printf("Nombre: ");
    fflush(stdin);
    gets(nota.nombreYApellido);

    printf("Legajo: ");
    fflush(stdin);
    scanf("%i", &nota.legajo);
    do{
        printf("Nota: ");
        fflush(stdin);
        scanf("%i", &nota.nota);
        if(nota.nota < 1 || nota.nota > 10) printf("\nLa nota no es valida. Ingrese otra nuevamente.\n");
    }while(nota.nota < 1 || nota.nota > 10);


    return nota;
}

//Buscar materia
int buscarMateria(celda materias[], char materiaBuscar[], int validos){
    int pos = -1;
    for(int i = 0; i < validos; i++){
        if(strcmp(materias[i].materia, materiaBuscar) == 0){
            return i;
        }
    }
    return pos;
}

//mostrar solo materias
void mostrarSoloMaterias(celda materias[], int validos){
    for(int i = 0; i < validos; i++){
        printf("\n-------------------------------------------------------------------------\n");
        printf("Materia: %s\n", materias[i].materia);
        printf("ID Materia: %i", materias[i].idMateria);
        printf("\n-------------------------------------------------------------------------\n");
    }
}

//mostrar materia con notas
void mostrarMateriaConNotas(celda materias[], char materiaMostrar[], int validos){
    int pos = buscarMateria(materias, materiaMostrar, validos);
    if(pos == -1) printf("\nLo siento. Esa materia no se encuentra en nuestros registros.\n");
    else{
        printf("\n--------------------------------------------\n");
        printf("Materia: %s\n", materias[pos].materia);
        printf("ID Materia: %i", materias[pos].idMateria);
        printf("\n--------------------------------------------\n");
        mostrarNotasDeEsaMateria(materias, pos);
    }
}

//mostrar notas de esa materia
void mostrarNotasDeEsaMateria(celda materias[], int pos){ ///RECURSIVA
    if(materias[pos].listaDeNotas == NULL) return;
    if(materias[pos].listaDeNotas != NULL){
        printf("\n--------------------------------------------\n");
        printf("Alumno: %s\n", materias[pos].listaDeNotas->dato.nombreYApellido);
        printf("Legajo: %i\n", materias[pos].listaDeNotas->dato.legajo);
        printf("Nota: %i", materias[pos].listaDeNotas->dato.nota);
        printf("\n--------------------------------------------\n");
        mostrarNotasDeEsaMateria(materias[pos].listaDeNotas->siguiente, pos);
    }
}

void pasarMateriasConAlumnosArchivo(celda materias[], int validos) {
    registroArchivo registro;
    FILE *punteroArchi = fopen(NOMBREARCHI, "ab");
    int i = 0;

    if (punteroArchi != NULL) {
        while (i < validos) {
            /// Recorrer la lista de notas y escribir cada nota en el archivo
            nodo *notaActual = materias[i].listaDeNotas;
            while (notaActual != NULL) {
                strcpy(registro.materia, materias[i].materia);
                registro.idMateria = materias[i].idMateria;

                strcpy(registro.nombreYApellido, notaActual->dato.nombreYApellido);
                registro.legajo = notaActual->dato.legajo;
                registro.nota = notaActual->dato.nota;

                fwrite(&registro, sizeof(registroArchivo), 1, punteroArchi);

                notaActual = notaActual->siguiente;
            }
            i++;
        }
        printf("\nInformacion transferida al archivo con exito.\n");
        fclose(punteroArchi);
    } else {
        printf("\nError al abrir el archivo.\n");
    }
}

void mostrarArchivo(){
    registroArchivo registro;
    FILE *punteroArchi = fopen(NOMBREARCHI, "rb");
    if(punteroArchi != NULL){
        while(!feof(punteroArchi)){
            fread(&registro,sizeof(registroArchivo),1,punteroArchi);
            if(!feof(punteroArchi)){
                printfDeMostrarArchi(registro);
            }
        }
        fclose(punteroArchi);
    }else printf("\nError al abrir el archivo.\n");
}

void printfDeMostrarArchi(registroArchivo registro){
    printf("\n------------------------------------------------\n");
    printf("Materia: %s\n", registro.materia);
    printf("ID Materia: %i\n\n", registro.idMateria);
    printf("Nombre: %s\n", registro.nombreYApellido);
    printf("Legajo: %i\n", registro.legajo);
    printf("Nota: %i", registro.nota);
    printf("\n------------------------------------------------\n");
}

void pasarDelArchivoACeldasDeListas(celda materias[], int *validos){
    registroArchivo registro;
    FILE *punteroArchi = fopen(NOMBREARCHI, "rb");
    if(punteroArchi != NULL){
        while(!feof(punteroArchi)){
            fread(&registro,sizeof(registroArchivo),1, punteroArchi);
            if(!feof(punteroArchi)){
///Pasamos los datos que contiene en arreglo de materias para luego dentro de cada indice de materias ingresarle los alumnos.
                strcpy(materias[*validos].materia,registro.materia);
                materias[*validos].idMateria = registro.idMateria;
                materias[*validos].listaDeNotas = inicLista();
                while(strcmp(materias[*validos].materia, registro.materia) == 0){
                    nodo *aux = NULL;
                    strcpy(aux->dato.nombreYApellido, registro.nombreYApellido);
                    aux->dato.legajo = registro.legajo;
                    aux->dato.nota = registro.nota;

                    materias[*validos].listaDeNotas = agregarNodoPrincipio(materias[*validos].listaDeNotas, aux);
                }
                *validos += 1;
            }
        }
    }else printf("\nError al abrir el archivo.\n");
    printf("\nArreglo de materias con notas cargado correctamente.\n");
}

void pasarAUnArchivoLasNotasAprobadas(celda materias[], int validos){
    registroArchivo registro;
    FILE *punteroArchi = fopen(APROBADOS, "ab");
    int i = 0;
    if(punteroArchi != NULL){
        while(i < validos){
            nodo *auxNotas = materias[i].listaDeNotas;
            while( auxNotas != NULL){
                if(auxNotas->dato.nota >= 6){
                strcpy(registro.materia, materias[i].materia);
                registro.idMateria = materias[i].idMateria;

                strcpy(registro.nombreYApellido, auxNotas->dato.nombreYApellido);
                registro.legajo = auxNotas->dato.legajo;
                registro.nota = auxNotas->dato.nota;
                fwrite(&registro, sizeof(registroArchivo), 1, punteroArchi);
                }
                auxNotas = auxNotas->siguiente;
            }
            i++;
        }
        fclose(punteroArchi);
    }else printf("\nError al abrir el archivo.\n");
}


