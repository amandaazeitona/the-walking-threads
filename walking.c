/*
ALUNOS: AMANDA OLIVEIRA ALVES - 15/0116276
PROFESSOR: JEREMIAS
TRABALHO DE PROGRAMAÇÃO CONCORRENTE - 2/2019
*/


/* 
Bibliotecas:
*/
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>
#include <string.h>

#define TRUE 1          /* Verdadeiro */
#define FALSE 0         /* Falso */
#define SURVIVORS 20    /* Número inicial de sobreviventes encurralados no shopping */
#define HUMAN_FOOD 200  /* Quantidade de porções que um sobrevivente morto adulto gera */
#define KID_FOOD 100    /* Quantidade de porções que um sobrevivente morto criança gera */
#define CAR_CAPACITY 3 	/* Número de vagas no carro */
#define EAT 20          /* Quantidade de porções comidas de uma vez por cada zumbi */

/*
Cores:
*/
#define COLOR_GREEN "\x1b[32m"
#define COLOR_BLACK "\x1b[30m"
#define COLOR_YELLOW "\x1b[33m"
#define COLOR_MAGENTA "\x1b[35m"
#define COLOR_BLUE "\x1b[34m"
#define COLOR_RED "\x1b[31m"
#define COLOR_CYAN "\x1b[36m"
#define COLOR_BRIGHT_YELLOW "\x1b[93m"
#define COLOR_BRIGHT_CYAN "\x1b[96m"
#define COLOR_RESET "\x1b[0m"
#define COLOR_BRIGHT_RED "\x1b[91m"
#define COLOR_BRIGHT_GREEN "\x1b[92m"
#define COLOR_BRIGHT_BLUE "\x1b[94m"
#define COLOR_BRIGHT_MAGENTA "\x1b[95m"

/*
Sobreviventes:
Nome = nome do sobrevivente 
Sexo = sexo e idade do sobrevivente (0 - homem, 1 - mulher, 2 - criança)
Status = estado atual do sobrevivente (0 - encurralado, 1 - resgatado , 2 - morto por zumbi, 3 - morto de fome)
*/
typedef struct{
  int id;
  char name[15];
  int sex;
  int status;
} survivor_arg, *ptr_survivor_arg;

/*
Carro:
Status = estado atual do carro (0 - resgatando, 1 - para no shopping , 2 - terminou resgate)
*/
typedef struct{
  int id;
  int status;
} rescue_car_arg, *ptr_rescue_car_arg;

pthread_t survivor[SURVIVORS];                                                        /* Thread dos sobreviventes */
pthread_t car;                                                                        /* Thread do carro */
survivor_arg surv_arg[SURVIVORS];                                                     /* Argumento dos sobreviventes */
rescue_car_arg car_arg;                                                               /* Argumento do carro */
pthread_mutex_t l = PTHREAD_MUTEX_INITIALIZER;                                        /* Lock para os sobreviventes */
pthread_mutex_t line_lock = PTHREAD_MUTEX_INITIALIZER;                                /* Lock para fila do carro */
sem_t wait_car;                                                                       /* Semáforos para o carro */
int food = 0;                                                                         /* Comida */
pthread_cond_t cm = PTHREAD_COND_INITIALIZER;                                         /* Condicional para os sobreviventes homens */
pthread_cond_t cw = PTHREAD_COND_INITIALIZER;                                         /* Condicional para os sobreviventes mulheres */
int child_shopping, woman_shopping;                                                   /* Número de sobreviventes ainda no shopping */
int number_man, number_woman, number_child;                                           /* Número de sobreviventes de cada sexo e idade */
int woman_on_line, man_on_line, car_waiting;                                          /* Homens, mulheres e carro esperando */
int number_alive = SURVIVORS;                                                         /* Número de sobreviventes vivos no shopping */
int capacity = CAR_CAPACITY;                                                          /* Número de vagas restantes no carro */
int id_kill;                                                                          /* ID do sobrevivente que morreu */
int used_man_names[100], used_woman_names[100];                                       /* Vetor de nomes usados */

/*
Nomes femininos
*/
char woman_names[100][15] = {
    "Amanda",              /*0*/
    "Maria Fernanda",      /*1*/
    "Letícia",             /*2*/
    "Ariana Grande",       /*3*/
    "Marina",              /*4*/
    "Lana Del Rey",        /*5*/
    "Bella",               /*6*/
    "Jéssica",             /*7*/
    "Shoshana",  		   /*8*/
    "Leia",                /*9*/
    "Dilma",       	       /*10*/
    "Alba",                /*11*/
    "Aleteia",             /*12*/
    "Carla Koike",         /*13*/
    "Carla Castanho",      /*14*/
    "Demi Lovato",         /*15*/
    "Germana",             /*16*/
    "Maristela",           /*17*/
    "Genaína",             /*18*/
    "Florence",            /*19*/
    "Rita Lee",            /*20*/
    "Edna",                /*21*/
    "Velma",               /*22*/
    "Padmé",               /*23*/
    "Sharpay",             /*24*/
    "Joelma",              /*25*/
    "Lurdinha",            /*26*/
    "Focátia",             /*27*/
    "Mulan",               /*28*/
    "Angelina",            /*29*/
    "Rihanna",             /*30*/
    "Mia",                 /*31*/
    "Samara",              /*32*/
    "Mariana",             /*33*/
    "Hermione",            /*34*/
    "Alcione",             /*35*/
    "Margot",              /*36*/
    "Beatrix",             /*37*/
    "Vernita",             /*38*/
    "Elle",                /*39*/
    "O-Ren Ishii",         /*40*/
    "Leslie",              /*41*/
    "April",               /*42*/
    "Andréia",             /*43*/
    "Myllena",             /*44*/
    "Vitória",             /*45*/
    "Scarlett",            /*46*/
    "Eveline",             /*47*/
    "Iana",                /*48*/
    "Eurides",             /*49*/
    "Tina",                /*50*/
    "",       /*51*/
    "",     /*52*/
    "",       /*53*/
    "",    /*54*/
    "",       /*55*/
    "",    /*56*/
    "",    /*57*/
    "",    /*58*/
    "",     /*59*/
    "",     /*60*/
    "",        /*61*/
    "",    /*62*/
    "",    /*63*/
    "",   /*64*/
    "",      /*65*/
    "",        /*66*/
    "", /*67*/
    "",       /*68*/
    "",       /*69*/
    "",     /*70*/
    "",        /*71*/
    "",      /*72*/
    "",     /*73*/
    "",     /*74*/
    "",      /*75*/
    "",    /*76*/
    "",    /*77*/
    "",      /*78*/
    "",   /*79*/
    "",       /*80*/
    "",    /*81*/
    "",     /*82*/
    "",     /*83*/
    "",     /*84*/
    "",     /*85*/
    "", /*86*/
    "",        /*87*/
    "",       /*88*/
    "",      /*89*/
    "",       /*90*/
    "",     /*91*/
    "",      /*92*/
    "",     /*93*/
    "",       /*94*/
    "",    /*95*/
    "",     /*96*/
    "",      /*97*/
    "",      /*98*/
    ""     /*99*/
};

/*
Nomes masculinos
*/
char man_names[100][15] = {
    "Phelps",               /*0*/
    "Jeremias",             /*1*/
    "Thales",               /*2*/
    "Pin",                  /*3*/
    "Jader",                /*4*/
    "Ícaro",                /*5*/
    "Diegod",               /*6*/
    "Brinquedo",            /*7*/
    "Kevin",                /*8*/
    "Homem Aranha",         /*9*/
    "Ladeira",              /*10*/
    "Lula",                 /*11*/
    "Zaghetto",             /*12*/
    "Drummond",             /*13*/
    "Bruno",                /*14*/
    "Dibio",                /*15*/
    "Alchieri",             /*16*/
    "Astrix",               /*17*/
    "Vidal",                /*18*/
    "Flavio",               /*19*/
    "Gondim",               /*20*/
    "Lucero",               /*21*/
    "Li",                   /*22*/
    "Mandelli",             /*23*/
    "Pai Mei",              /*24*/
    "Lamar",                /*25*/
    "Boni",                 /*26*/
    "Vander",               /*27*/
    "Bill",                 /*28*/
    "Budd",                 /*29*/
    "Ron",                  /*30*/
    "Roberlândio",          /*31*/
    "Miguel",               /*32*/
    "Bernardo",             /*33*/
    "Rodolfo",              /*34*/
    "Jomas",                /*35*/
    "Weslei",               /*36*/
    "Arthur",               /*37*/
    "Naruto",               /*38*/
    "Mr. Darcy",            /*39*/
    "Seth",                 /*40*/
    "Ryan",                 /*41*/
    "Troy",                 /*42*/
    "Chuck",                /*43*/
    "Vincent",              /*44*/
    "Jules",                /*45*/
    "Hans",                 /*46*/
    "Andy",                 /*47*/
    "Diego",                /*48*/
    "Bolsominion",          /*49*/
    "Severino",             /*50*/
    "",        /*51*/
    "",         /*52*/
    "",           /*53*/
    "",        /*54*/
    "",         /*55*/
    "",          /*56*/
    "",          /*57*/
    "",   /*58*/
    "",          /*59*/
    "",       /*60*/
    "",         /*61*/
    "",          /*62*/
    "",            /*63*/
    "",          /*64*/
    "",           /*65*/
    "",       /*66*/
    "",    /*67*/
    "",         /*68*/
    "",        /*69*/
    "",        /*70*/
    "",   /*71*/
    "",          /*72*/
    "",   /*73*/
    "",       /*74*/
    "",           /*75*/
    "",       /*76*/
    "",        /*77*/
    "",        /*78*/
    "",          /*79*/
    "",   /*80*/
    "",     /*81*/
    "",        /*82*/
    "",           /*83*/
    "",        /*84*/
    "",          /*85*/
    "",       /*86*/
    "",         /*87*/
    "",           /*88*/
    "",         /*89*/
    "",        /*90*/
    "",        /*91*/
    "",     /*92*/
    "",         /*93*/
    "",        /*94*/
    "",       /*95*/
    "",         /*96*/
    "",     /*97*/
    "",          /*98*/
    ""            /*99*/
};

/*
Limpa a tela do terminal
*/
void clrscr(){
  system("clear");
}

/*
Imprime na tela o status de cada sobrevivente
*/
void print_status(int status){
  
  switch (status){
    case 0:
      printf(COLOR_BRIGHT_YELLOW "ENCURRALADO" COLOR_RESET);
      break;

    case 1:
      printf(COLOR_BRIGHT_GREEN "RESGATADO" COLOR_RESET);
      break;

    case 2:
      printf(COLOR_BRIGHT_RED "MORTO - Virou comida de zumbi!" COLOR_RESET);
      break;

    case 3:
      printf(COLOR_BRIGHT_MAGENTA "MORTO - Morreu de fome!" COLOR_RESET);
      break;

    default:
      break;
  }
}

/*
Inicializa os vetores de nomes
*/
void initialize_used_names(){
  int i;

  for(i=0;i<100;i++){
    used_woman_names[i] = 0;
    used_man_names[i] = 0;
  }
}
