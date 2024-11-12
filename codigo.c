#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

//Un par de variables extra, básicamente para comodidad mía (ademas, no queda tan lindo asi nomas).
#define TAMANO 3
#define MAX_VIDA 9
#define PUNTOS_EXTRA 5

int pregunta();
void balas_aleatorias(int tambor[TAMANO][TAMANO]);
void mostrar_vida_7_segmentos(int vida);
void guardar_puntos(const char* nombre, int puntos);

main() {
    int vida_usuario = 5;
    int vida_ia = 5;
    int acertar, puntos = 0, turno = 1;
    int aciertos_consecutivos = 0;
    int errores_consecutivos = 0;
    int i = 0, j = 0;
    int tambor[TAMANO][TAMANO];
    char nombre[50];
    
    srand(time(NULL));
    balas_aleatorias(tambor);

    //Se solicita el nombre del jugador.
    printf("Ingrese su nombre: ");
    gets(nombre);
    
    printf("Bienvenido %s!", nombre);
    
    do {
        printf("\nPresione ENTER para continuar...");
        getchar();
        
        acertar = pregunta();

        if (acertar == 1) { //El jugador responde correctamente.
            printf("Su respuesta fue correcta, apuntando al contrincante...\n");
            getchar();

            if (tambor[i][j] == 1) { //Hay bala en el tambor.
                vida_ia--;
                printf("El disparo acerto, la vida del contrincante es ahora de %i.\n", vida_ia);
            } else {
                printf("El disparo no salio...\n");
            }

            //Se añaden puntos y reseten los errores consecutivos.
            puntos += (turno < 3) ? 10 : 5; // Menos puntos después del 3er turno
            aciertos_consecutivos++;
            errores_consecutivos = 0;

            //Vida extra para el jugador después de 3 aciertos consecutivos.
            if (aciertos_consecutivos == 3 && vida_usuario < MAX_VIDA) {
                vida_usuario++;
                printf("Bonus! Has ganado 1 vida adicional.\n");
                aciertos_consecutivos = 0;
            }
        } else { //El jugador responde incorrectamente.
            printf("Su respuesta fue incorrecta, apuntando a usted...\n");
            getchar();

            if (tambor[i][j] == 1) { //Hay bala en el tambor.
                vida_usuario--;
                printf("El disparo acerto, tu vida es ahora de %i.\n", vida_usuario);
            } else {
                printf("El disparo no salio...\n");
            }

            //Se resetean los aciertos consecutivos y suman los errores.
            aciertos_consecutivos = 0;
            errores_consecutivos++;

            //Vida extra para el contrincante después de 3 errores consecutivos.
            if (errores_consecutivos == 3 && vida_ia < MAX_VIDA) {
                vida_ia++;
                printf("El contrincante gana 1 vida adicional!\n");
                errores_consecutivos = 0;
            }
        }

        //Avance de las posiciones i y j en el tambor.
        if (j == TAMANO - 1) {
            j = 0;
            i = (i + 1) % TAMANO;
        } else {
            j++;
        }

        //Actualizar el tambor al completarse.
        if (i == 0 && j == 0) {
            turno++;
            balas_aleatorias(tambor);
            printf("\nSe ha recargado el tambor para el proximo turno.\n");
        }

        //Mostrar vidas en formato 7 segmentos.
        printf("\nVida del usuario:\n");
        mostrar_vida_7_segmentos(vida_usuario);
        printf("Vida del contrincante:\n");
        mostrar_vida_7_segmentos(vida_ia);
		
		printf("\nPresione ENTER para continuar...");
        getchar();
		system("cls");
		
    } while (vida_usuario > 0 && vida_ia > 0);

    if (vida_usuario > 0) {
        printf("Felicidades %s! Ganaste.\n", nombre);
        puntos += PUNTOS_EXTRA; //Puntos extra por ganar.
    } else {
        printf("Lo siento, has perdido.\n");
    }

    guardar_puntos(nombre, puntos);

    return 0;
}

void balas_aleatorias(int tambor[TAMANO][TAMANO]) {
	int i, j, k = 0;
	int balas = rand()%3 + 1; //Se generan entre 1 y 3 balas de forma aleatoria.

	//Se inicializa el tambor a 0.
	for (i = 0; i < 3; i++) {
		for (j = 0; j < 3; j++) {
			tambor[i][j] = 0;
		}
	}

	//Colocamos las balas en el tambor (teniendo en cuenta cuantas balas se "generaron").
	for (k = 0; k < balas; k++) {
		//Se busca un espacio aleatorio dentro del tambor y se verifica que alli no haya una bala.
		do {
			i = rand()%3;
			j = rand()%3;
		} while (tambor[i][j] == 1);

		tambor[i][j] = 1; //Colocamos la bala en la posicion que se encontro vacia.
	}
}

//Guarda la puntuación en un archivo de tabla de puntos.
void guardar_puntos(const char* nombre, int puntos) {
    FILE* archivo = fopen("tabla_de_puntos.txt", "a");
    if (archivo == NULL) {
        printf("Error al abrir el archivo de puntuación.\n");
        return;
    }
    fprintf(archivo, "%s: %d puntos\n", nombre, puntos);
    fclose(archivo);
}

//Funcion para mostrar un número en formato de 7 segmentos.
void mostrar_vida_7_segmentos(int vida) {
    const char* segmentos[10] = {
        " _ \n| |\n|_|\n", // 0
        "   \n  |\n  |\n", // 1
        " _ \n _|\n|_ \n", // 2
        " _ \n _|\n _|\n", // 3
        "   \n|_|\n  |\n", // 4
        " _ \n|_ \n _|\n", // 5
        " _ \n|_ \n|_|\n", // 6
        " _ \n  |\n  |\n", // 7
        " _ \n|_|\n|_|\n", // 8
        " _ \n|_|\n _|\n"  // 9
    };

    printf("%s", segmentos[vida]);
}

int pregunta() {
	//Acá están todas las preguntas, están abajo del todo porque su fiaca bajar tanto una y otra vez para arreglar alguna boludés en las otras funciones.
	int pregunta = rand()%2;
	int respuesta = 1;
	switch (pregunta) {
		case 1: 
			printf("En que continente se encuentra Espana?.\n1 - Oceania.\n2 - Asia.\n3 - Europa.\n4 - America del Sur.\n");
			do {
				printf("Respuesta: ");
				scanf("%i", &respuesta);
				if (respuesta == 3) {
					return 1;
				} else if ((respuesta == 1) || (respuesta == 2) || (respuesta == 4)) {
					return 2;
				} else {
					printf("Opcion no disponible, por favor, elija una opcion entre las disponibles.\n");
				}
				fflush(stdin);
			} while ((respuesta < 1) || (respuesta > 4));
			break;
		
		default:
			return 1;
			break;
	}
	//hacer pregunta sobre display de 7 segmentos, ej "cuales variables (a, b, c, d, e, f, g) hay que encender para hacer el numero 4"
	//quien gano el mundial en el ano 2022
	//pregunta sobre mate discreta (sumas, restas, multiplicacion entre matrices)
	//determinar si una matriz tiene inversa
	//cuantas provincias tiene argentina
	//cual es la capital de "x" provincia
	//cual es la principal causa de muertes en la era actual
	//quien fue el presidente de argentina durante el periodo "x al x"
	//pregunta sobre San Martin
	//preguntas sobre la 2da guerra mundial y guerra fria
	//cuales son los operadores logicos
	//AGREGAR MAS
}
