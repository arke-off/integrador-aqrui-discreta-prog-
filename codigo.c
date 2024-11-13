/*
)El juego consta de 3 instancias
1er instancia (a)solicitud de nombre    (b)saludo con explicacion reglas	(c)mostrar vidas 7S
2da instancia (a)elegir opcion          (b)mostrar acierto,probar disparo   (c)mostrar progreso de vidas
3ra instancia (a)anuncio de resultado   (b)mostrar vidas en 0               (c)solicitar iniciar/salir
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <windows.h>
#include "colors.h" //Libreria de colores modificada.

//Un par de variables extra, básicamente para comodidad mía (ademas, no queda tan lindo asi nomas).
#define TAMANO 3
#define MAX_VIDA 9
#define PUNTOS_EXTRA 5

void pantallaCompleta();
int pregunta();
void balas_aleatorias(int tambor[TAMANO][TAMANO]);
void mostrar_vida_7_segmentos(int vida_usuario, int vida_ia);
void guardar_puntos(const char* nombre, int puntos);
void temporizadorGatillo();

main() {
	
	pantallaCompleta();
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
	printf(MAIN ); //Seteamos gracias a colors.h el color principal.
//Primer Instancia
//Se solicita el nombre del jugador.
    printf("Ingrese su ");
    printf(PLAYER "nombre " MAIN);
    printf("para iniciar el juego: " PLAYER);
    gets(nombre);
    system("cls");
    printf(LINE"=================================================================================\n"MAIN);
    printf(MAIN "\t\t\t\tBienvenido ");
	printf(PLAYER "%s" MAIN, nombre);
	printf("!\n" MAIN);
    printf("\nEste juego (");
    printf(PLAYER "usuario" MAIN);
    printf(" VS ");
    printf(MACHINE "maquina" MAIN);
    printf(") consta de responder preguntas.\n");
    printf("Ambos comienzan con 5 vidas.\n");
    printf("Si uno de los 2 responde bien entonces apunta al enemigo y ""prueba disparar"".\n");
    printf(LINE"=================================================================================\n"MAIN);
       
     printf("\nPresione ENTER para continuar...");
    getchar();
    //comentario
    do {
    	system("cls");
	    printf(LINE"=================================================================================\n"MAIN);
        
		 //Mostrar vidas en formato 7 segmentos.
        printf(PLAYER"\nVida del usuario:\t" MAIN);
        printf(MACHINE "Vida del contrincante:\n" MAIN);
        mostrar_vida_7_segmentos(vida_usuario, vida_ia);
        printf("\n");
        acertar = pregunta();

        if (acertar == 1) { //El jugador responde correctamente.
        	printf(ACCEPT "Su respuesta fue correcta, apuntando al contrincante.\n" MAIN);
            temporizadorGatillo();
            fflush(stdin);
			//getchar(); //en vez de presionar enter realizar una cuenta regresiva

            if (tambor[i][j] == 1) { //Hay bala en el tambor.
                vida_ia--;
                printf(ACCEPT "\nEl disparo acerto, la vida del contrincante es ahora de %i.\n" MAIN, vida_ia);
            } else {
                printf(REJECT "\nEl disparo no salio...\n" MAIN);
            }

            //Se anaden puntos y reseten los errores consecutivos.
            puntos += (turno < 3) ? 10 : 5; // Menos puntos despues del 3er turno
            aciertos_consecutivos++;
            errores_consecutivos = 0;

            //Vida extra para el jugador despues de 3 aciertos consecutivos.
            if (aciertos_consecutivos == 3 && vida_usuario < MAX_VIDA) {
                vida_usuario++;
                printf(PLAYER "Bonus! Has ganado 1 vida adicional.\n" MAIN);
                aciertos_consecutivos = 0;
            }
        } else { //El jugador responde incorrectamente.
            printf(REJECT "Su respuesta fue incorrecta, apuntando a usted...\n" MAIN);
            temporizadorGatillo();
            fflush(stdin);

            if (tambor[i][j] == 1) { //Hay bala en el tambor.
                vida_usuario--;
                printf(REJECT "\nEl disparo acerto, tu vida es ahora de %i.\n" MAIN, vida_usuario);
            } else {
                printf(ACCEPT "\nEl disparo no salio...\n" MAIN);
            }

            //Se resetean los aciertos consecutivos y suman los errores.
            aciertos_consecutivos = 0;
            errores_consecutivos++;

            //Vida extra para el contrincante despues de 3 errores consecutivos.
            if (errores_consecutivos == 3 && vida_ia < MAX_VIDA) {
                vida_ia++;
                printf(MACHINE "El contrincante gana 1 vida adicional!\n" MAIN);
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
            printf(RESET"\nSe ha recargado el tambor para el proximo turno.\n"MAIN);
        }
		printf(LINE"\n=================================================================================\n"MAIN);
		printf(MAIN "\nPresione ENTER para continuar...");
        getchar();
		
    } while (vida_usuario > 0 && vida_ia > 0);

    if (vida_usuario > 0) {
        printf(MAIN "Felicidades ");
        printf(PLAYER "%s", nombre);
        printf(MAIN "! Ganaste.\n");
        puntos += PUNTOS_EXTRA; //Puntos extra por ganar.
    } else {
        printf(MAIN "Lo siento, has perdido.\n");
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

//Funcion para mostrar un numero en formato de 7 segmentos.
void mostrar_vida_7_segmentos(int vida_usuario, int vida_ia) {
    const char* segmentos[10][3] = {
        {" _ ", "| |", "|_|"}, // 0
        {"   ", "  |", "  |"}, // 1
        {" _ ", " _|", "|_ "}, // 2
        {" _ ", " _|", " _|"}, // 3
        {"   ", "|_|", "  |"}, // 4
        {" _ ", "|_ ", " _|"}, // 5
        {" _ ", "|_ ", "|_|"}, // 6
        {" _ ", "  |", "  |"}, // 7
        {" _ ", "|_|", "|_|"}, // 8
        {" _ ", "|_|", " _|"}  // 9
    };

    // Imprime cada l�nea de los displays de ambos jugadores en paralelo
    int i;
    for (i = 0; i < 3; i++) {
        printf(PLAYER"\t%s\t\t\t"MAIN, segmentos[vida_usuario][i]);
        printf(MACHINE"%s\n"MAIN, segmentos[vida_ia][i]);
    }
}




int pregunta() {
	//Acá están todas las preguntas, están abajo del todo porque su fiaca bajar tanto una y otra vez para arreglar alguna boludés en las otras funciones.
	int pregunta = rand()%2;
	int respuesta = 1;
	switch (pregunta) {
		case 1: 
			printf(MAIN"En que continente se encuentra Espana?.\n");
			printf(RESET"[1] Oceania.\n[2] Asia.\n[3] Europa.\n[4] America del Sur.\n"MAIN);
			do {
				printf("Respuesta: "RESET);
				scanf("%i", &respuesta);
				if (respuesta == 3) {
					return 1;
				} else if ((respuesta == 1) || (respuesta == 2) || (respuesta == 4)) {
					return 2;
				} else {
					printf(MAIN"Opcion no disponible, por favor, elija una opcion entre las disponibles.\n");
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

void temporizadorGatillo(){
	int i;
	char temporizadorVector[15]="3...2...1...\n";
	for (i=0;	i<12; i++){
		printf("%c",temporizadorVector[i]);
		Sleep(200);
	}
}

void pantallaCompleta(){
    keybd_event(VK_MENU, 0, 0, 0);      // Presiona la tecla Alt
    keybd_event(VK_RETURN, 0, 0, 0);    // Presiona la tecla Enter
    keybd_event(VK_RETURN, 0, KEYEVENTF_KEYUP, 0);  // Suelta la tecla Enter
    keybd_event(VK_MENU, 0, KEYEVENTF_KEYUP, 0);    // Suelta la tecla Alt   
}
