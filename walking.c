/*
ALUNOS: AMANDA OLIVEIRA ALVES - 15/0116276
PROFESSOR: JEREMIAS
TRABALHO DE PROGRAMAÇÃO CONCORRENTE - 2/2019
*/


/* 
Bibliotecas
*/
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>
#include <string.h>

#define TRUE 1          /* Verdadeiro */
#define FALSE 0         /* Falso */
#define SURVIVORS 17    /* Número inicial de sobreviventes encurralados no shopping */
#define HUMAN_FOOD 200  /* Quantidade de porções que um sobrevivente morto adulto gera */
#define KID_FOOD 100    /* Quantidade de porções que um sobrevivente morto criança gera */
#define EAT 20          /* Quantidade de porções comidas de uma vez por cada sobrevivente */
#define BOAT_CAPACITY 3 /* Número de vagas no carro */

/*
Cores
*/
#define COLOR_BLACK "\x1b[30m"
#define COLOR_RED "\x1b[31m"
#define COLOR_GREEN "\x1b[32m"
#define COLOR_YELLOW "\x1b[33m"
#define COLOR_BLUE "\x1b[34m"
#define COLOR_MAGENTA "\x1b[35m"
#define COLOR_CYAN "\x1b[36m"
#define COLOR_BRIGHT_RED "\x1b[91m"
#define COLOR_BRIGHT_GREEN "\x1b[92m"
#define COLOR_BRIGHT_YELLOW "\x1b[93m"
#define COLOR_BRIGHT_BLUE "\x1b[94m"
#define COLOR_BRIGHT_MAGENTA "\x1b[95m"
#define COLOR_BRIGHT_CYAN "\x1b[96m"
#define COLOR_RESET "\x1b[0m"