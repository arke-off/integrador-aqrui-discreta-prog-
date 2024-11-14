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

//Un par de variables extra, basicamente para comodidad mia (ademas, no queda tan lindo asi nomas).
#define TAMANO 3
#define MAX_VIDA 9
#define PUNTOS_EXTRA 5
int preguntas = 1;

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
    int i = 0, j = 0;
    int tambor[TAMANO][TAMANO];
    char nombre[50];
    
    srand(time(NULL));
    balas_aleatorias(tambor);
	printf(MAIN ); //Seteamos gracias a colors.h el color principal.
    //Primer Instancia.
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
    printf("Si respondes bien, apuntas al enemigo. Sino, te apuntas a vos mismo.\nLa bala puede o no salir.\n");
    printf(LINE"=================================================================================\n"MAIN);
    asciiArt();
       
    printf("\nPresione ENTER para continuar...");
    getchar();

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

            if (tambor[i][j] == 1) { //Hay bala en el tambor.
                vida_ia--;
                printf(ACCEPT "\nEl disparo acerto, la vida del contrincante es ahora de %i.\n" MAIN, vida_ia);
                Beep(450, 1500); 
            } else {
                printf(REJECT "\nEl disparo no salio...\n" MAIN);
                Beep(750, 500); 
            }

            //Se anaden puntos y reseten los errores consecutivos.
            puntos += (turno < 3) ? 10 : 5; // Menos puntos despues del 3er turno

        } else { //El jugador responde incorrectamente.
            printf(REJECT "Su respuesta fue incorrecta, apuntando a usted...\n" MAIN);
            temporizadorGatillo();
            fflush(stdin);

            if (tambor[i][j] == 1) { //Hay bala en el tambor.
                vida_usuario--;
                printf(REJECT "\nEl disparo acerto, tu vida es ahora de %i.\n" MAIN, vida_usuario);
                Beep(450, 1500); 
            } else {
                printf(ACCEPT "\nEl disparo no salio...\n" MAIN);
                Beep(750, 800); 
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

	 preguntas++; //Incremeta parar pasar a la siguiente pregunta.
    } while (vida_usuario > 0 && vida_ia > 0);

    if (vida_usuario > 0) {
        printf(MAIN "\n\nFelicidades ");
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

//Guarda la puntuacion en un archivo de tabla de puntos.
void guardar_puntos(const char* nombre, int puntos) {
    FILE* archivo = fopen("tabla_de_puntos.txt", "a");
    if (archivo == NULL) {
        printf("Error al abrir el archivo de puntuacion.\n");
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

    // Imprime cada linea de los displays de ambos jugadores en paralelo
    int i;
    for (i = 0; i < 3; i++) {
        printf(PLAYER"\t%s\t\t\t"MAIN, segmentos[vida_usuario][i]);
        printf(MACHINE"%s\n"MAIN, segmentos[vida_ia][i]);
    }
}

void temporizadorGatillo(){
	int i;
	char temporizadorVector[15]="3...2...1...\n";
	for (i=0;	i<12; i++){
		printf("%c",temporizadorVector[i]);
        if (temporizadorVector[i] == '3' || temporizadorVector[i] == '2' || temporizadorVector[i] == '1') {
            Beep(750, 400); 
            //Sleep(100);
        }
	}
}

void pantallaCompleta(){
    keybd_event(VK_MENU, 0, 0, 0);      // Presiona la tecla Alt
    keybd_event(VK_RETURN, 0, 0, 0);    // Presiona la tecla Enter
    keybd_event(VK_RETURN, 0, KEYEVENTF_KEYUP, 0);  // Suelta la tecla Enter
    keybd_event(VK_MENU, 0, KEYEVENTF_KEYUP, 0);    // Suelta la tecla Alt   
}

void asciiArt(){
    printf(RESET"  (                                 _\n");
    printf("   )                               /=>\n");
    printf("  (  +____________________/\/\___ / /|\n");
    printf("   .''._____________'._____      / /|/\n");
    printf("  : () :              :\ ----\|    \ )\n");
    printf("   '..'______________.'0|----|      \\\n");
    printf("                    0_0/____/        \\\n");
    printf("                        |----    /----\\\n");
    printf("                       || -\\ --|      \\\n");
    printf("                       ||   || ||\      \\\n");
    printf("                        \\____// '|      \\\n");
    printf(" Truth or Shoot!               .'/       |\n");
    printf("                              .:/        |\n");
    printf("                              :/_________|\n");
    printf("                                           \n"MAIN);
}

int pregunta() {
	//Aca estan todas las preguntas
    //	int preguntas = 2    ;
	int respuesta = 1;
	switch (preguntas) {
	
		case 1: 
            printf(MAIN"Cual puede operarse por producto de matrices?.\n");
            printf(RESET"[1] 2x3 x 1x3.\n[2] 3x2 x 2x3\n[3] 1x3 x 1x3\n[4] 4x4 x 2x2.\n"MAIN);
            do {
                printf("Respuesta: "RESET);
                scanf("%i", &respuesta);
                if (respuesta == 2) {
                    return 1;
                } else if ((respuesta == 1) || (respuesta == 3) || (respuesta == 4)) {
                    return 2;
                } else {
                    printf(MAIN"Opcion no disponible, por favor, elija una opcion entre las disponibles.\n");
                }
                fflush(stdin);
            } while ((respuesta < 1) || (respuesta > 4));
            break;
	
		case 2: 
			printf(MAIN"Que valor es 12 en Hexadecimal?.\n");
			printf(RESET"[1] A.\n[2] B.\n[3] C.\n[4] D.\n"MAIN);
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
		
        case 3: 
			printf(MAIN"Los numeros racionales con la suma forman:\n");
			printf(RESET"[1] Grupo.\n[2] Monoide.\n[3] Ninguno.\n[4] Grupo abeliano.\n"MAIN);
			do {
				printf("Respuesta: "RESET);
				scanf("%i", &respuesta);
				if (respuesta == 4) {
					return 1;
				} else if ((respuesta == 1) || (respuesta == 2) || (respuesta == 3)) {
					return 2;
				} else {
					printf(MAIN"Opcion no disponible, por favor, elija una opcion entre las disponibles.\n");
				}
				fflush(stdin);
			} while ((respuesta < 1) || (respuesta > 4));
			break;

        case 4: 
			printf(MAIN"Que es modularizar un codigo?.\n");
			printf(RESET"[1] Integrar funciones.\n[2] Integrar recursividad.\n[3] Integrar punteros.\n[4] Integrar librerias.\n"MAIN);
			do {
				printf("Respuesta: "RESET);
				scanf("%i", &respuesta);
				if (respuesta == 1) {
					return 1;
				} else if ((respuesta == 4) || (respuesta == 2) || (respuesta == 3)) {
					return 2;
				} else {
					printf(MAIN"Opcion no disponible, por favor, elija una opcion entre las disponibles.\n");
				}
				fflush(stdin);
			} while ((respuesta < 1) || (respuesta > 4));
			break;

        case 5: 
			printf(MAIN"Que par de simbolos se utilizan en condiciones logicas?.\n");
			printf(RESET"[1] <+.\n[2] !=.\n[3] +>.\n[4] =-.\n"MAIN);
			do {
				printf("Respuesta: "RESET);
				scanf("%i", &respuesta);
				if (respuesta == 2) {
					return 1;
				} else if ((respuesta == 4) || (respuesta == 3) || (respuesta == 1)) {
					return 2;
				} else {
					printf(MAIN"Opcion no disponible, por favor, elija una opcion entre las disponibles.\n");
				}
				fflush(stdin);
			} while ((respuesta < 1) || (respuesta > 4));
			break;

		case 6: 
            printf(MAIN"Que es una matriz y como se define en terminos de filas y columnas?.\n");
            printf(RESET"[1] Una matriz es un conjunto de ecuaciones lineales.\n[2] Una matriz es una tabla rectangular de elementos organizada en filas y columnas.\n[3] Una matriz es una funcion que solo puede tomar valores enteros.\n[4] Una matriz es una estructura de datos que almacena numeros en un solo vector.\n"MAIN);
            do {
                printf("Respuesta: "RESET);
                scanf("%i", &respuesta);
                if (respuesta == 2) {
                    return 1;
                } else if ((respuesta == 1) || (respuesta == 3) || (respuesta == 4)) {
                    return 2;
                } else {
                    printf(MAIN"Opcion no disponible, por favor, elija una opcion entre las disponibles.\n");
                }
                fflush(stdin);
            } while ((respuesta < 1) || (respuesta > 4));
            break;

        case 7: 
            printf(MAIN"Como se realiza la multiplicacion de matrices y bajo que condiciones es posible?.\n");
            printf(RESET"[1] Solo cuando ambas matrices son simetricas.\n[2] Solo cuando ambas matrices tienen el mismo numero de filas.\n[3] Solo cuando el numero de columnas de la primera coincide con el numero de filas de la segunda.\n[4] Solo cuando ambas matrices tienen dimensiones identicas.\n"MAIN);
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

        case 8: 
            printf(MAIN"Que es una matriz identidad y cual es su importancia en el algebra lineal?.\n");
            printf(RESET"[1] Es una matriz que tiene todos sus elementos iguales a cero.\n[2] Es una matriz cuadrada con unos en la diagonal principal y ceros en el resto.\n[3] Es una matriz que tiene todos sus elementos positivos.\n[4] Es una matriz utilizada solo en grafos dirigidos.\n"MAIN);
            do {    
                printf("Respuesta: "RESET);
                scanf("%i", &respuesta);
                if (respuesta == 2) {
                    return 1;
                } else if ((respuesta == 1) || (respuesta == 3) || (respuesta == 4)) {
                    return 2;
                } else {
                    printf(MAIN"Opcion no disponible, por favor, elija una opcion entre las disponibles.\n");
                }
                fflush(stdin);
            } while ((respuesta < 1) || (respuesta > 4));
            break;

        case 9: 
            printf(MAIN"En que consiste la teoria de grafos y cuales son sus principales aplicaciones?.\n");
            printf(RESET"[1] En estudiar operaciones con matrices de gran tamano.\n[2] En definir ecuaciones diferenciales y funciones de segundo orden.\n[3] En la resolucion de problemas de estadistica avanzada.\n[4] En estudiar las relaciones y conexiones entre elementos llamados vertices y aristas.\n"MAIN);
            do {    
                printf("Respuesta: "RESET);
                scanf("%i", &respuesta);
                if (respuesta == 4) {
                    return 1;
                } else if ((respuesta == 1) || (respuesta == 2) || (respuesta == 3)) {
                    return 2;
                } else {
                    printf(MAIN"Opcion no disponible, por favor, elija una opcion entre las disponibles.\n");
                }
                fflush(stdin);
            } while ((respuesta < 1) || (respuesta > 4));
            break;
        
        case 10: 
            printf(MAIN"Que es un grafo dirigido y como se diferencia de un grafo no dirigido?.\n");
            printf(RESET"[1] Un grafo dirigido tiene pesos en sus aristas.\n[2] Un grafo dirigido tiene mas nodos que un grafo no dirigido.\n[3] Un grafo dirigido tiene aristas que van en un solo sentido, mientras que en un grafo no dirigido no hay direcciones.\n[4] Un grafo dirigido solo puede tener ciclos, mientras que un grafo no dirigido no puede tener ciclos.\n"MAIN);
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
        
        case 11: 
            printf(MAIN"Que es una matriz de adyacencia y como se usa en la representacion de grafos?.\n");
            printf(RESET"[1] Es una matriz que muestra las conexiones entre los vertices de un grafo.\n[2] Es una matriz que describe los valores de cada nodo.\n[3] Es una matriz que solo se usa en grafos no dirigidos.\n[4] Es una matriz que se utiliza para realizar operaciones algebraicas entre grafos.\n"MAIN);
            do {    
                printf("Respuesta: "RESET);
                scanf("%i", &respuesta);
                if (respuesta == 1) {
                    return 1;
                } else if ((respuesta == 2) || (respuesta == 3) || (respuesta == 4)) {
                    return 2;
                } else {
                    printf(MAIN"Opcion no disponible, por favor, elija una opcion entre las disponibles.\n");
                }
                fflush(stdin);
            } while ((respuesta < 1) || (respuesta > 4));
            break;

        case 12: 
            printf(MAIN"Como se define una estructura algebraica y cuales son algunos ejemplos comunes (como grupos o anillos)?.\n");
            printf(RESET"[1] Como un conjunto con una operacion binaria que cumple ciertas propiedades.\n[2] Como una funcion entre varios conjuntos.\n[3] Como un conjunto de datos organizados en filas y columnas.\n[4] Como una lista de matrices cuadradas.\n"MAIN);
            do {    
                printf("Respuesta: "RESET);
                scanf("%i", &respuesta);
                if (respuesta == 1) {
                    return 1;
                } else if ((respuesta == 2) || (respuesta == 3) || (respuesta == 4)) {
                    return 2;
                } else {
                    printf(MAIN"Opcion no disponible, por favor, elija una opcion entre las disponibles.\n");
                }
                fflush(stdin);
            } while ((respuesta < 1) || (respuesta > 4));
            break;

        case 13: 
            printf(MAIN"Que significa que un grafo sea conexo y como se determina?.\n");
            printf(RESET"[1] Que cada nodo esta conectado a al menos dos aristas.\n[2] Que existe un camino entre cualquier par de vertices del grafo.\n[3] Que todos los vertices estan numerados consecutivamente.\n[4] Que todos los nodos tienen el mismo grado.\n"MAIN);
            do {    
                printf("Respuesta: "RESET);
                scanf("%i", &respuesta);
                if (respuesta == 2) {
                    return 1;
                } else if ((respuesta == 1) || (respuesta == 3) || (respuesta == 4)) {
                    return 2;
                } else {
                    printf(MAIN"Opcion no disponible, por favor, elija una opcion entre las disponibles.\n");
                }
                fflush(stdin);
            } while ((respuesta < 1) || (respuesta > 4));
            break;

        case 14: 
            printf(MAIN"Que es un subgrafo y como se relaciona con el grafo principal?.\n");
            printf(RESET"[1] Es un conjunto de vertices que no tienen aristas.\n[2] Es un grafo independiente, sin conexion al grafo principal.\n[3] Es un grafo que contiene todos los nodos y aristas del grafo original.\n[4] Es una parte del grafo original, formada por un subconjunto de vertices y aristas.\n"MAIN);
            do {    
                printf("Respuesta: "RESET);
                scanf("%i", &respuesta);
                if (respuesta == 4) {
                    return 1;
                } else if ((respuesta == 1) || (respuesta == 2) || (respuesta == 3)) {
                    return 2;
                } else {
                    printf(MAIN"Opcion no disponible, por favor, elija una opcion entre las disponibles.\n");
                }
                fflush(stdin);
            } while ((respuesta < 1) || (respuesta > 4));
            break;

        case 15: 
            printf(MAIN"Como funciona un display de 7 segmentos y para que se usa?.\n");
            printf(RESET"[1] Usando pantallas LCD para mostrar texto.\n[2] Usando 7 LEDs para representar letras.\n[3] Usando LEDs para mostrar caracteres numericos.\n[4] Usando luces de neon para ilustrar numeros binarios.\n"MAIN);
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

        case 16: 
            printf(MAIN"Cual es la ventaja de representar numeros en binario en sistemas digitales?.\n");
            printf(RESET"[1] La rapidez en la conversion a decimal.\n[2] La simplicidad de manipulacion y compatibilidad con el hardware.\n[3] La facilidad para realizar operaciones de logica difusa.\n[4] La precision en calculos de punto flotante.\n"MAIN);
            do {    
                printf("Respuesta: "RESET);
                scanf("%i", &respuesta);
                if (respuesta == 2) {
                    return 1;
                } else if ((respuesta == 1) || (respuesta == 3) || (respuesta == 4)) {
                    return 2;
                } else {
                    printf(MAIN"Opcion no disponible, por favor, elija una opcion entre las disponibles.\n");
                }
                fflush(stdin);
            } while ((respuesta < 1) || (respuesta > 4));
            break;

        case 17: 
            printf(MAIN"Que es un arbol en teoria de grafos y en que se diferencia de otros tipos de grafos?.\n");
            printf(RESET"[1] Un arbol es un grafo sin ciclos y conexo.\n[2] Un arbol es un grafo dirigido.\n[3] Un arbol es un grafo en el que todos los vertices tienen el mismo grado.\n[4] Un arbol es un grafo sin conexiones.\n"MAIN);
            do {    
                printf("Respuesta: "RESET);
                scanf("%i", &respuesta);
                if (respuesta == 1) {
                    return 1;
                } else if ((respuesta == 2) || (respuesta == 3) || (respuesta == 4)) {
                    return 2;
                } else {
                    printf(MAIN"Opcion no disponible, por favor, elija una opcion entre las disponibles.\n");
                }
                fflush(stdin);
            } while ((respuesta < 1) || (respuesta > 4));
            break;

        case 18: 
            printf(MAIN"Como se convierte un numero decimal a binario? Explica el procedimiento...\n");
            printf(RESET"[1] Dividiendo sucesivamente por 2 y tomando los residuos.\n[2] Multiplicando por 10 hasta llegar a 0.\n[3] Sumando 2 en cada paso.\n[4] Multiplicando sucesivamente por 2 y sumando 1.\n"MAIN);
            do {    
                printf("Respuesta: "RESET);
                scanf("%i", &respuesta);
                if (respuesta == 1) {
                    return 1;
                } else if ((respuesta == 2) || (respuesta == 3) || (respuesta == 4)) {
                    return 2;
                } else {
                    printf(MAIN"Opcion no disponible, por favor, elija una opcion entre las disponibles.\n");
                }
                fflush(stdin);
            } while ((respuesta < 1) || (respuesta > 4));
            break;

        case 19: 
            printf(MAIN"Como se convierte un numero binario a decimal?\n");
            printf(RESET"[1] Multiplicando cada bit por potencias de 10 y sumando.\n[2] Sumando cada bit consecutivo hasta obtener el valor.\n[3] Multiplicando cada bit por potencias de 2 y sumando los resultados.\n[4] Restando cada bit de la base 16.\n"MAIN);
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

        case 20: 
            printf(MAIN"Cual es el proceso para convertir un numero hexadecimal a decimal?\n");
            printf(RESET"[1] Multiplicar cada digito por potencias de 16 y sumar los resultados.\n[2] Multiplicar cada digito por potencias de 10 y sumar.\n[3] Sumar los digitos consecutivamente.\n[4] Dividir cada digito entre 16.\n"MAIN);
            do {    
                printf("Respuesta: "RESET);
                scanf("%i", &respuesta);
                if (respuesta == 1) {
                    return 1;
                } else if ((respuesta == 2) || (respuesta == 3) || (respuesta == 4)) {
                    return 2;
                } else {
                    printf(MAIN"Opcion no disponible, por favor, elija una opcion entre las disponibles.\n");
                }
                fflush(stdin);
            } while ((respuesta < 1) || (respuesta > 4));
            break;

        case 21: 
            printf(MAIN"Por que el sistema hexadecimal usa 16 simbolos y cuales son?\n");
            printf(RESET"[1] Usa los digitos 1 a 16.\n[2] Usa los digitos 0-9 y letras A-F.\n[3] Usa los numeros del 1 al 8 y letras G-M.\n[4] Usa los digitos 0-9 y letras A-F.\n"MAIN);
            do {    
                printf("Respuesta: "RESET);
                scanf("%i", &respuesta);
                if (respuesta == 4) {
                    return 1;
                } else if ((respuesta == 1) || (respuesta == 2) || (respuesta == 3)) {
                    return 2;
                } else {
                    printf(MAIN"Opcion no disponible, por favor, elija una opcion entre las disponibles.\n");
                }
                fflush(stdin);
            } while ((respuesta < 1) || (respuesta > 4));
            break;

        case 22: 
            printf(MAIN"Como se representa el numero 255 en binario y en hexadecimal?\n");
            printf(RESET"[1] En binario como 11111111 y en hexadecimal como FF.\n[2] En binario como 11110000 y en hexadecimal como EE.\n[3] En binario como 10101010 y en hexadecimal como AA.\n[4] En binario como 00001111 y en hexadecimal como 0F.\n"MAIN);
            do {    
                printf("Respuesta: "RESET);
                scanf("%i", &respuesta);
                if (respuesta == 1) {
                    return 1;
                } else if ((respuesta == 2) || (respuesta == 3) || (respuesta == 4)) {
                    return 2;
                } else {
                    printf(MAIN"Opcion no disponible, por favor, elija una opcion entre las disponibles.\n");
                }
                fflush(stdin);
            } while ((respuesta < 1) || (respuesta > 4));
            break;

        case 23: 
            printf(MAIN"Cual es la representacion en display de 7 segmentos del numero hexadecimal 'A'?\n");
            printf(RESET"[1] Segmentos a, b, c, e, f, g encendidos.\n[2] Segmentos a, b, c, e, f, g encendidos.\n[3] Segmentos a, b, d, e, g encendidos.\n[4] Segmentos c, e, f encendidos.\n"MAIN);
            do {    
                printf("Respuesta: "RESET);
                scanf("%i", &respuesta);
                if (respuesta == 1) {
                    return 1;
                } else if ((respuesta == 2) || (respuesta == 3) || (respuesta == 4)) {
                    return 2;
                } else {
                    printf(MAIN"Opcion no disponible, por favor, elija una opcion entre las disponibles.\n");
                }
                fflush(stdin);
            } while ((respuesta < 1) || (respuesta > 4));
            break;

        case 24: 
            printf(MAIN"Que significan los terminos 'bits' y 'bytes' en la informatica?\n");    
            printf(RESET"[1] Bit es una unidad de 2 digitos y byte una unidad de 4 digitos.\n[2] Bit es una medida de velocidad y byte de cantidad.\n[3] Bit representa numeros y byte letras.\n[4] Bit es una unidad de 1 o 0, y byte es un conjunto de 8 bits.\n"MAIN);
            do {    
                printf("Respuesta: "RESET);
                scanf("%i", &respuesta);
                if (respuesta == 4) {
                    return 1;
                } else if ((respuesta == 1) || (respuesta == 2) || (respuesta == 3)) {
                    return 2;
                } else {
                    printf(MAIN"Opcion no disponible, por favor, elija una opcion entre las disponibles.\n");
                }
                fflush(stdin);
            } while ((respuesta < 1) || (respuesta > 4));
            break;

        case 25: 
            printf(MAIN"Cual es el rango de valores representables en un numero binario de 8 bits?\n");    
            printf(RESET"[1] De -256 a 255.\n[2] De 0 a 255.\n[3] De -128 a 128.\n[4] De 1 a 128.\n"MAIN);
            do {    
                printf("Respuesta: "RESET);
                scanf("%i", &respuesta);
                if (respuesta == 2) {
                    return 1;
                } else if ((respuesta == 1) || (respuesta == 3) || (respuesta == 4)) {
                    return 2;
                } else {
                    printf(MAIN"Opcion no disponible, por favor, elija una opcion entre las disponibles.\n");
                }
                fflush(stdin);
            } while ((respuesta < 1) || (respuesta > 4));
            break;

        case 26: 
            printf(MAIN"Que diferencia hay entre una funcion void y una que devuelve un valor en C?\n");    
            printf(RESET"[1] Una funcion void no tiene parametros.\n[2] Una funcion void no devuelve ningun valor.\n[3] Una funcion void solo realiza operaciones matematicas.\n[4] Una funcion void es solo para imprimir valores.\n"MAIN);
            do {    
                printf("Respuesta: "RESET);
                scanf("%i", &respuesta);
                if (respuesta == 2) {
                    return 1;
                } else if ((respuesta == 1) || (respuesta == 3) || (respuesta == 4)) {
                    return 2;
                } else {
                    printf(MAIN"Opcion no disponible, por favor, elija una opcion entre las disponibles.\n");
                }
                fflush(stdin);
            } while ((respuesta < 1) || (respuesta > 4));
            break;

        case 27: 
            printf(MAIN"Como se usa el operador logico && en una condicion if?\n");    
            printf(RESET"[1] Para representar una condicion de negacion.\n[2] Para evaluar si alguna condicion es verdadera.\n[3] Para realizar una asignacion multiple.\n[4] Para evaluar si ambas condiciones son verdaderas.\n"MAIN);
            do {    
                printf("Respuesta: "RESET);
                scanf("%i", &respuesta);
                if (respuesta == 4) {
                    return 1;
                } else if ((respuesta == 1) || (respuesta == 2) || (respuesta == 3)) {
                    return 2;
                } else {
                    printf(MAIN"Opcion no disponible, por favor, elija una opcion entre las disponibles.\n");
                }
                fflush(stdin);
            } while ((respuesta < 1) || (respuesta > 4));
            break;

        case 28: 
            printf(MAIN"Para que sirve la funcion printf() y como se usa?\n");    
            printf(RESET"[1] Para declarar variables en C.\n[2] Para realizar calculos matematicos complejos.\n[3] Para imprimir texto o valores en la consola.\n[4] Para definir estructuras.\n"MAIN);
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
        
        case 29: 
            printf(MAIN"Que es un operador logico y cuales son los mas comunes en C?\n");    
            printf(RESET"[1] Son operadores para realizar operaciones matematicas.\n[2] Son operadores para definir funciones.\n[3] Son operadores para modificar valores de variables.\n[4] Son operadores como &&, ||, y ! para evaluar condiciones logicas.\n"MAIN);
            do {    
                printf("Respuesta: "RESET);
                scanf("%i", &respuesta);
                if (respuesta == 4) {
                    return 1;
                } else if ((respuesta == 1) || (respuesta == 2) || (respuesta == 3)) {
                    return 2;
                } else {
                    printf(MAIN"Opcion no disponible, por favor, elija una opcion entre las disponibles.\n");
                }
                fflush(stdin);
            } while ((respuesta < 1) || (respuesta > 4));
            break;

        case 30: 
            printf(MAIN"Como se declara e inicializa una funcion en C que recibe un entero como parametro?\n");    
            printf(RESET"[1] int nombreFuncion(void).\n[2] void nombreFuncion(int parametro).\n[3] char nombreFuncion(int parametro).\n[4] struct nombreFuncion(int parametro).\n"MAIN);
            do {    
                printf("Respuesta: "RESET);
                scanf("%i", &respuesta);
                if (respuesta == 2) {
                    return 1;
                } else if ((respuesta == 1) || (respuesta == 3) || (respuesta == 4)) {
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
}




