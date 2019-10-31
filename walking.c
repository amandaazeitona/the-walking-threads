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
#define ADULT_FOOD 200  /* Quantidade de porções que um sobrevivente morto adulto gera */
#define KID_FOOD 100    /* Quantidade de porções que um sobrevivente morto criança gera */
#define CAR_CAPACITY 3 	/* Número de vagas no carro */
#define EAT 20          /* Quantidade de porções comidas de uma vez por cada zumbi */

/*
Cores:
*/
#define GREEN "\x1b[32m"
#define BLACK "\x1b[30m"
#define YELLOW "\x1b[33m"
#define MAGENTA "\x1b[35m"
#define BLUE "\x1b[34m"
#define RED "\x1b[31m"
#define CYAN "\x1b[36m"
#define BRIGHT_YELLOW "\x1b[93m"
#define BRIGHT_CYAN "\x1b[96m"
#define RESET "\x1b[0m"
#define BRIGHT_RED "\x1b[91m"
#define BRIGHT_GREEN "\x1b[92m"
#define BRIGHT_BLUE "\x1b[94m"
#define BRIGHT_MAGENTA "\x1b[95m"

/*
Sobreviventes:
Nome = nome do sobrevivente 
Sexo = sexo e idade do sobrevivente (0 - homem, 1 - mulher, 2 - criança)
Status = estado atual do sobrevivente (0 - encurralado, 1 - resgatado , 2 - morto por zumbi, 3 - morto de fome)
*/
typedef struct{
  int id;
  char name[20];
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
int v_man_names[50], v_woman_names[50], v_child_names[50];                            /* Vetores de nomes usados */

/*
Nomes femininos
*/
char woman_names[50][20] = {
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
};

/*
Nomes masculinos
*/
char man_names[50][20] = {
    "Severino",             /*0*/
    "Martin",               /*1*/
    "Jesus",                /*2*/
    "Karl",                 /*3*/
    "Buddha",               /*4*/
    "Maomé",                /*5*/
    "Anakin",               /*6*/
    "Ernesto",              /*7*/
    "Yuri",                 /*8*/
    "Felipe",               /*9*/
    "Frederico",            /*10*/
    "Pedro",                /*11*/
    "Gabriel",              /*12*/
    "Ítalo",                /*13*/
    "Igor",                 /*14*/
    "Gustavo",              /*15*/
    "Diones",               /*16*/
    "Alan",                 /*17*/
    "Paulo",                /*18*/
    "Vinícius",             /*19*/
    "Zeca Pagodinho",       /*20*/
    "Bob Marley",           /*21*/
    "Nicholas",             /*22*/
    "Lucas",                /*23*/
    "Vitor",                /*24*/
    "Diogo",                /*25*/
    "Andrio",               /*26*/
    "Cebolinha",            /*27*/
    "Wlad",                 /*28*/
    "Cascão",               /*29*/
    "Renan",                /*30*/
    "Marcus",               /*31*/
    "Leonardo",             /*32*/
    "Augustus",             /*33*/
    "Heitor",               /*34*/
    "Enzo",                 /*35*/
    "Sasuke",               /*36*/
    "Itachi",               /*37*/
    "Kakashi",              /*38*/
    "Adão",                 /*39*/
    "Roberto",              /*40*/
    "Rogério",              /*41*/
    "Joshua",               /*42*/
    "Caetano",              /*43*/
    "Jorge",                /*44*/
    "Jacques",              /*45*/
    "Danilo",               /*46*/
    "Joel",                 /*47*/
    "Light",                /*48*/
    "Alfredo"               /*49*/
};

/*
Nomes crianças
*/
char child_names[100][20] = {
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
    "Roberlândio",          /*23*/
    "Pai Mei",              /*24*/
	"Tina",                 /*25*/
    "Cate",                 /*26*/
    "Natalie",              /*27*/
    "Emma",                 /*28*/
    "Sandy",                /*29*/
    "Sasha",                /*30*/
    "Pabblo Vittar",        /*31*/
    "Anitta",               /*32*/
    "Larissa",              /*33*/
    "Martina",              /*34*/
    "Lurdinha",             /*35*/
    "Melissa",              /*36*/
    "Bruna",                /*37*/
    "Thainá",               /*38*/
    "Ada",                  /*39*/
    "Catarina",             /*40*/
    "Juliana",              /*41*/
    "Rayanny",              /*42*/
    "Manoela",              /*43*/
    "Ana",                  /*44*/
    "Amy",                  /*45*/
    "Sarah",                /*46*/
    "Priscila",             /*47*/
    "Luiza",                /*48*/
    "Sakura",               /*49*/
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
      printf(BRIGHT_YELLOW "ENCURRALADO" RESET);
      break;

    case 1:
      printf(BRIGHT_GREEN "RESGATADO" RESET);
      break;

    case 2:
      printf(BRIGHT_RED "MORTO - Virou comida de zumbi!" RESET);
      break;

    case 3:
      printf(BRIGHT_MAGENTA "MORTO - Morreu de fome!" RESET);
      break;

    default:
      break;
  }
}

/*
Inicializa os vetores de nomes
*/
void init_v_names(){
  int i;

  for(i=0;i<50;i++){
    v_woman_names[i] = 0;
    v_man_names[i] = 0;
    v_child_names[i] = 0;
  }
}

/*
De acordo com os índices:
i = Estado atual do sobrevivente (0 - encurralado, 1 - resgatado , 2 - morto por zumbi, 3 - morto de fome)
i + 1 = Tipo de sobrevivente (0 - homem, 1 - mulher, 2 - criança)
i + 2 = Número entre 0 e 49 que equivale ao nome do sobrevivente 

Essa função inicializa os náufragos com seus respectivos argumentos
*/
void init_survivors(){
  int name, i, j;
  int length = SURVIVORS;

  number_woman = 0;
  number_man = 0;
  number_child = 0;
  init_v_names();

  srand(time(NULL));

  for(i=0;i<length;i++){
    surv_arg[i].id = i;
    surv_arg[i].status = 0;
    surv_arg[i].sex = (rand() % 2);
    switch (surv_arg[i].sex){
      case 0:
        number_man++;
        break;

      case 1:
        number_woman++;
        break;

      case 2:
        number_child++;
        break;

      default:
        break;
    }

    name = (rand() % 50);
    j = 0;
    if(surv_arg[i].sex == 0){
    	while(v_man_names[name] == 1){
    		name = (rand() % 50);
    	}

    	while(man_names[name][j] != '\0'){
        	surv_arg[i].name[j] = man_names[name][j];
        	j++;
      	}
    	v_man_names[name] = 1;
    }
    else{
    	if(surv_arg[i].sex == 1) {
	    	while(v_woman_names[name] == 1){
	    		name = (rand() % 50);
	      	}
	    	while(woman_names[name][j] != '\0'){
	        	surv_arg[i].name[j] = woman_names[name][j];
	        	j++;
	      	}
	    	v_woman_names[name] = 1;
    	}

    	if(surv_arg[i].sex == 2) {
	    	while(v_child_names[name] == 1){
	    		name = (rand() % 50);
	      	}
	    	while(child_names[name][j] != '\0'){
	        	surv_arg[i].name[j] = child_names[name][j];
	        	j++;
	      	}
	    	v_child_names[name] = 1;
    	}
    }
    surv_arg[i].name[j] = '\0';
  }

  child_shopping = number_child;
  woman_shopping = number_woman;
}
