/*
Segundo projeto da cadeira de Introducao aos Algoritmos e Estruturas de Dados.
Continuacao da construcao de um sistema de gestao de voos.

Nome: Pedro Afonso Saraiva Gomes
Numero: 103468
E-mail: pedroafonsogomes3@tecnico.ulisboa.pt  ou  pedroafonsogomes3@gmail.com
Data: 18/04/2022
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define N 40 /* Guarda o numero maximo de Aeroportos */
#define M 30000 /* Guarda o numero maximo de Voos */
#define DATA_STRING_MAX 17 /* Guarda o tamanho da string formatada das datas */
#define ID_MAX 4 /* Guarda tamanho dos nomes dos IDs */
#define PAIS_MAX 31 /* Guarda tamanho dos nomes dos Paises */
#define CIDADE_MAX 51 /* Guarda tamanho dos nomes das cidades */
#define CV_MAX 7 /* Guarda o tamanho dos codigos de voo */
#define RESERVA_MAX 65535 /* Guarda o tamanho dos codigos de reserva */

#define ERROR_A_ID "invalid airport ID\n" 
#define ERROR_A_MAXED "too many airports\n"
#define ERROR_A_DUPLICATED "duplicate airport\n"
#define ERROR_A_NOTFOUND ": no such airport ID\n"

#define ERROR_F_CODE "invalid flight code\n"
#define ERROR_F_DUPLICATED "flight already exists\n"
#define ERROR_F_MAXED "too many flights\n"
#define ERROR_F_DURATION "invalid duration\n"
#define ERROR_F_CAPACITY "invalid capacity\n"
#define ERROR_F_CODE_NEW ": flight does not exist\n" /* New */

#define ERROR_DATE "invalid date\n" /* Reused */

#define ERROR_R_CODE "invalid reservation code\n" /* New */
#define ERROR_R_DUPLICATED ": flight reservation already used\n" /* New */
#define ERROR_R_MAXED "too many reservations\n" /* New */
#define ERROR_R_P_NUMBER "invalid passenger number\n" /* New */

#define ERROR_E_CODE "not found\n" /* New */

#define ERROR_N_MEMORY "No memory\n" /* New */



/*----------------------- ESTRUTURAS -----------------------*/

/* Definicao do novo tipo que representam Aeroportos */
typedef struct Airport{
  char ID[ID_MAX];
  char Pais[PAIS_MAX];
  char Cidade[CIDADE_MAX];
  int Numero_Voos;
} Airport;


/* Definicao do novo tipo que representa Reservas de Voos */
typedef struct Reservation{
  char Codigo_Voo[CV_MAX];
  int Data[3];
  char *Codigo_Reserva;
  int Numero_Pessoas;
  struct Reservation *next;
} Reservation;


/* Definicao do novo tipo que representam Voos */
typedef struct Flights{
  char Codigo_Voo[CV_MAX];
  char Aeroporto_Partida[ID_MAX];
  char Aeroporto_Chegada[ID_MAX];
  int Data[3];
  int Hora[2];
  int Duracao[2];
  int Capacidade;
  int C_Data[3];
  int C_Hora[2];
  Reservation *Reservas_Cabeca;
} Flights;



/*----------------------- VARIAVEIS GLOBAIS -----------------------*/

/* Struct que representa os Aeroportos e as suas informacoes */
Airport Aeroportos[N]; 

/* Struct que representa os Voos e as suas informacoes */
Flights Voos[M]; 

/* Vetor que guarda o Dia, Mes e Ano atual, por essa ordem */
int Data[3]; 

/* Variavel que marca quantos Aeroportos existem atualmente */
int Aeroportos_Ocupados = 0; 

/* Variavel que marca quantos voos existem atualmente */
int Voos_Ocupados = 0;

/* Variavel que marca quantas reservas existem atualmente */
int Reservas_Ocupadas = 0;

int termina = 0; /* Usada para terminar o programa caso haja falta de memoria */

/*----------------------- FUNCOES AUXILIARES -----------------------*/


void conta_voos() { 
  /* A funcao nao recebe nada. A funcao calcula o numero de voos de cada 
  aeroporto e adiciona essa informacao ao respetivo aeroporto para ser usado 
  posteriormente na funcao L (lista_aeroportos). A funcao nao retorna nada. 
  Funcao criada para tornar a funcao que auxilia mais legivel. */

  int i, j;
  int res = 0;

  for (j = 0; j < Aeroportos_Ocupados; j++) {
    for (i = 0; i < Voos_Ocupados; i++) 
        if (strcmp(Voos[i].Aeroporto_Partida, Aeroportos[j].ID) == 0) res ++;
    Aeroportos[j].Numero_Voos = res;
    res = 0;
  }
  return;
}



void adiciona_chegada (int i) { 
  /* A funcao nao recebe nada. A funcao recebe um inteiro i que representa o 
  numero de um aeroporto e adiciona-lhe a sua  informacao de chegada 
  (Data e Hora), calculando sempre  se a duracao do voo faz com que o dia, 
  mes ou ano se alterem e modifica-os de acordo. A funcao nao retorna nada. 
  Funcao criada para tornar a funcao que auxilia mais legivel. */

  int d_chegada = Voos[i].Data[0];
  int m_chegada = Voos[i].Data[1];
  int a_chegada = Voos[i].Data[2];
  int h_chegada = Voos[i].Hora[0] + Voos[i].Duracao[0];
  int min_chegada = Voos[i].Hora[1] + Voos[i].Duracao[1];

  if (min_chegada >= 60) {
    h_chegada ++;
    min_chegada = min_chegada % 60;
  }

  if (h_chegada >= 24) {
    d_chegada ++;
    h_chegada = h_chegada % 24;
  }

  if (d_chegada > 31 && (((m_chegada % 2) == 1 &&  m_chegada <=6) || 
  ((m_chegada % 2 == 0) && m_chegada >= 6))) {
    m_chegada++;
    d_chegada = 1;
  }
  if (d_chegada > 30 && (m_chegada == 4 || m_chegada == 6 || 
  m_chegada == 9 || m_chegada == 11)) {
    m_chegada++;
    d_chegada = 1;
  }
  if (d_chegada > 28 && m_chegada == 2) {
    m_chegada++;
    d_chegada = 1;
  }

  if (m_chegada > 12) {
    a_chegada++;
    m_chegada = 1;
  }
    

  Voos[i].C_Data[0] = d_chegada;
  Voos[i].C_Data[1] = m_chegada;
  Voos[i].C_Data[2] = a_chegada;
  Voos[i].C_Hora[0] = h_chegada;
  Voos[i].C_Hora[1] = min_chegada;
  return;
}



void lista_alfabeticamente () { 
  /* A funcao nao recebe nada. A funcao mapeia os indices dos Aeroportos com 
  base na organizacao alfabetica dos IDs usando a funcao strcmp para comparar
  os diferentes IDs e tendo em conta que @ < A < ... < Z < [] na tabela ASCII,
  guardando os indices ordenados num vector. Apos organizar os indices a funcao 
  vai dar print a informacoes dos Aeroportos seguindo a ordem dos indices que
  se encontram no vector. A funcao nao retorna nada. 
  Funcao criada para tornar a funcao que auxilia mais legivel. */

  int i, j, k;
  char menor_ID[]="[[["; /* Variavel que vai guardar o menor ID */
  char ultimo_ID[]="@@@"; /* Varival que vai guaradar o ultimo ID anexado */
  int vector_ordem[N]; /* Vector que vai guardar os indices organizados */
  int troca = 0; /* 0 se nao houve troca nos indices originais, 1 se houve */

  for (i = 0; i < Aeroportos_Ocupados; i++) {
    vector_ordem[i] = i;
  }

  for (j = 0; j < Aeroportos_Ocupados; j++) {
    for (i = 0; i < Aeroportos_Ocupados; i++) {
      if (strcmp(Aeroportos[i].ID, menor_ID) < 0 &&
      strcmp(ultimo_ID, Aeroportos[i].ID) < 0) {
        strcpy(menor_ID, Aeroportos[i].ID);
        troca = 1;
        k = i;
      }
    }
    if (troca == 0) break;
    vector_ordem[j] = k;
    strcpy(ultimo_ID, menor_ID);
    strcpy(menor_ID, "[[[");
  }
  for (i = 0; i < Aeroportos_Ocupados; i++) {
    k = vector_ordem[i];
    printf("%s %s ", Aeroportos[k].ID, Aeroportos[k].Cidade);
    printf("%s %d\n", Aeroportos[k].Pais, Aeroportos[k].Numero_Voos);
  }
}



int id_check(char ID[ID_MAX]){ 
  /* Funcao recebe um ID, e vai percorrer todos os aeroportos e comparar o ID 
  recebido com os IDs dos varios aeroportos ate encontrar um que seja igual. 
  A funcao retorna 1 se encontrar um ID igual e retorna 0 caso contario.
  Criada com o proposito de evitar duplicacao de codigo visto que eh uma
  condicao usada em 4 comandos diferentes */

  int i;

  for (i = 0; i < N; i++) {
    if (strcmp(Aeroportos[i].ID,ID) == 0) {
      break;
    }
    else if (i == (Aeroportos_Ocupados)) {
      return 0;
    }
  }
  return 1;
}



int codigo_voo_check(char Codigo_Voo[CV_MAX]) {
  /* Funcao recebe um Codigo de voo, e vai percorrer todos os caracteres e 
  verificar se sao todos ou letras maiusculas ou digitos. A funcao retorna 1 se 
  o codigo for valido, 0 se nao o for. Funcao criada para tornar a funcao que 
  auxilia mais legivel. */

  int i;

  /* Codigo de voo invalido (Primeiros elementos nao sao letras maisuclas) */
  for (i = 0; i < 2; i++) {
    if (Codigo_Voo[i] < 'A' || Codigo_Voo[i] > 'Z') {
      return 0;
    }
  }

  /* Codigo de voo invalido (Ultimos quatro elementos nao sao numeors) */
  for (i = 2; i < 6; i++) {
    if (Codigo_Voo[i] == '\0') 
      break; 
    if (Codigo_Voo[i] < '0' || Codigo_Voo[i] > '9') {
      return 0;
    }
  }
  return 1;
}

int data_check (int Dia, int Mes, int Ano) {
  /* Funcao recebe tres inteiros que representam o dia, mes e ano, e vai 
  comparar com a data atual para ver se eh valida, isto eh, se eh no futuro
  e nao ultrapassa um ano ate la. A funcao retorna 1 se a data for invalida 
  e retorna 0 se a data for valida.
  Criada com o proposito de evitar duplicacao de codigo visto que eh uma
  condicao usada em 2 comandos diferentes. */

  /* Testa se o voo eh no passado */
  if ((Ano - Data[2] < 0) || 
  ((Ano - Data[2]) == 0 && (Mes - Data[1]) < 0) ||
  ((Ano - Data[2]) == 0 && (Mes - Data[1]) == 0 && (Dia - Data[0]) < 0)) {
    printf(ERROR_DATE); 
    return 1;
  }

  /* Testa se o voo eh daqui a mais de um ano */
  if ((Ano - Data[2] > 2) || 
  ((Ano - Data[2]) == 1 && (Data[1] - Mes) > 0) ||
  ((Ano - Data[2]) == 1 && (Data[1] - Mes) == 0 && (Dia - Data[0]) > 0)) {
    printf(ERROR_DATE); 
    return 1;
  }
  return 0;
}



int check_all_aeroporto (char ID[ID_MAX]) {
  /* Funcao que agrupa todas as condicoes para a criacao de um aeroporto. A
  funcao recebe todos os parametros que determinam a validade da criacao de
  um novo aeroporto e testa pela ordem que aparecem no enunciado. A funcao
  retorna 1 se alguma condicao nao se verificar e 0 se todas se verificarem. 
  Funcao criada para tornar a funcao que auxilia mais legivel. */

  int i;

  /* Verifica se tem mais de 40 aeroportos */
  if (Aeroportos_Ocupados == N) {
    printf(ERROR_A_MAXED);
    return 1;
  }

  /* Verifica se o ID estah em uppercase */
  for (i = 0; i < 3; i++) {
    if (ID[i] < 'A' || ID[i] > 'Z') {
      printf(ERROR_A_ID);
      return 1;
    }
  }
 
  /* Verifica se o ID eh duplicado */
  if (id_check(ID)) {
    printf(ERROR_A_DUPLICATED);
    return 1;
  }
  return 0;
}



int check_all_voo (char Codigo_Voo[CV_MAX], 
  int Dia, int Mes, int Ano,
  char ID_Partida[ID_MAX], char ID_Chegada[ID_MAX], 
  int Duracao_Hora, int Duracao_Minuto, int Capacidade) {
  /* Funcao que agrupa todas as condicoes para a criacao de um voo. A
  funcao recebe todos os parametros que determinam a validade da criacao de
  um novo voo e testa pela ordem que aparecem no enunciado. A funcao retorna
  1 se alguma condicao nao se verificar e 0 se todas se verificarem.
  Funcao criada para tornar a funcao que auxilia mais legivel. */

  int i;

  if (codigo_voo_check(Codigo_Voo) == 0) {
    printf(ERROR_F_CODE);
    return 1;
  }
  
  /* Codigo de Voo Duplicado */
  for (i = 0; i <Voos_Ocupados; i++) { 
    if (strcmp(Voos[i].Codigo_Voo, Codigo_Voo) == 0 &&
    (Voos[i].Data[0]==Dia && Voos[i].Data[1]==Mes && Voos[i].Data[2]==Ano)) {
      printf(ERROR_F_DUPLICATED);
      return 1;
    }
  }

  /* IDD de Partida nao existe */
  if (id_check(ID_Partida) == 0) {
    printf("%s", ID_Partida);
    printf(ERROR_A_NOTFOUND);
    return 1;
  }

  /* IDD de Chegada nao existe */
  if (id_check(ID_Chegada) == 0) {
    printf("%s", ID_Chegada);
    printf(ERROR_A_NOTFOUND);
    return 1;
  }

  /* Voos ultrapassam o numero limite M */
  if (Voos_Ocupados == M) {
    printf(ERROR_F_MAXED);
    return 1;
  }

  if (data_check(Dia, Mes, Ano)) {
    return 1;
  } 

  /* Duracao do Voo ultrapassam o limite de 12 horas */
  if ((Duracao_Hora > 12) || (Duracao_Hora == 12 && Duracao_Minuto > 0)) {
    printf(ERROR_F_DURATION);
    return 1;
  }

  /* Capacidade do Voo nao esta dentro dos padroes (Pelo menos 10) */
  if (Capacidade < 10) {
    printf(ERROR_F_CAPACITY);
    return 1;
  }
  return 0;
}


int check_part_add_reserva (char Codigo_Voo[CV_MAX], int Dia, int Mes, int Ano){
  /* A funcao recebe uma string que representa o codigo de voo, e tres inteiros
  que representam o dia, mes e ano respetivamente. A funcao vai verificar se
  o codigo de voo recebido esta associado a algum voo criado com a respetiva
  data associada. A funcao retorna 1 se as condicoes nao se verificarem, retorna
  0 se as conidcoes se verificarem. 
  Funcao criada para tornar a funcao que auxilia mais legivel. */

  int i;

  /* Verifica se ha voos criados */
  if (Voos_Ocupados == 0) {
    printf("%s", Codigo_Voo);
    printf(ERROR_F_CODE_NEW);
      return 1;
  }

  /* Verifica se ha voos criados com a data recebida */
  for (i = 0; i < Voos_Ocupados; i++) {

    if (strcmp(Voos[i].Codigo_Voo, Codigo_Voo) == 0 && Voos[i].Data[0] == Dia &&
    Voos[i].Data[1] == Mes && Voos[i].Data[2] == Ano) {
      break;
    }
    if ((Voos_Ocupados-1) == i) {
      printf("%s", Codigo_Voo);
      printf(ERROR_F_CODE_NEW);
      return 1;
    }
 }

  /* Verifica a data */
  if (data_check(Dia, Mes, Ano)) {
    return 1;
  }
  return 0; 
}

int check_all_add_reserva (char Codigo_Voo[CV_MAX], 
char Codigo_Reserva[RESERVA_MAX], int Dia, int Mes, int Ano, int N_Pessoas)  {
  /* A funcao recebe duas strings (uma que representa o codigo de voo e outra
  que representa o codigo da reserva) e tres inteiros (que representam o dia,
  mes ano e numero de pessoas respetivamente). A funcao integra todas as
  condicoes da funcao anterior, mas para alem disso verifica a validade do 
  codigo da reserva, da capacidade e do numero de reservas. A funcao retorna 1 
  se as condicoes nao se verificarem, retorna 0 se as conidcoes se verificarem. 
  Funcao criada para tornar a funcao que auxilia mais legivel. */

  int res = 0;
  int i;  
  int Capacidade = 0;
  int size;
  Reservation *Pointer;

  size = strlen(Codigo_Reserva);  

  /* Verifica o codigo de reserva */
  if (size < 10) {
    printf(ERROR_R_CODE);
    return 1;
  }
  for (i = 0; i < size; i++) {
    if ((Codigo_Reserva[i] < '0' || Codigo_Reserva[i] > '9') &&
    (Codigo_Reserva[i] < 'A' || Codigo_Reserva[i] > 'Z')) {
      printf(ERROR_R_CODE);
      return 1;
    }
  }

  /* Verifica o codigo de voo para a respetva data e guarda a capacidade e 
  numero de lugares ja ocupados */
  if (Voos_Ocupados == 0) {
    printf("%s", Codigo_Voo);
    printf(ERROR_F_CODE_NEW);
      return 1;
  }
 for (i = 0; i < Voos_Ocupados; i++) {
    if (strcmp(Voos[i].Codigo_Voo, Codigo_Voo) == 0 && Voos[i].Data[0] == Dia &&
    Voos[i].Data[1] == Mes && Voos[i].Data[2] == Ano) {

      Capacidade = Voos[i].Capacidade;
      Pointer = Voos[i].Reservas_Cabeca;

      while (Pointer != NULL) {
        if (Pointer->Data[0] == Dia && Pointer->Data[1] == Mes
        && Pointer->Data[2] == Ano) {
          res += Pointer->Numero_Pessoas;
        }
        Pointer = Pointer->next;
      }
      break;
    }

    if ((i + 1) == Voos_Ocupados) {
      printf("%s", Codigo_Voo);
      printf(ERROR_F_CODE_NEW);
      return 1;
    }
  }

  /* Verifica se o codigo de reserva ja existe */ 
  for (i = 0; i < Voos_Ocupados; i++) {

    Pointer = Voos[i].Reservas_Cabeca;

    while (Pointer != NULL) {
      if (strcmp(Pointer->Codigo_Reserva, Codigo_Reserva) == 0) {
        printf("%s", Codigo_Reserva);
        printf(ERROR_R_DUPLICATED);
        return 1;
      }
      Pointer = Pointer->next;
    }
  }

  /*Verifica se nao ultrapassa a capacidade */
  if (res + N_Pessoas > Capacidade) {
    printf(ERROR_R_MAXED);
    return 1;
  }

  /* Verifica a data */
  if (data_check(Dia, Mes, Ano)) {
    return 1;
  } 

  /*Verifica o numero de pessoas que ocupam a reserva */
  if (N_Pessoas < 1) {
    printf(ERROR_R_P_NUMBER);
    return 1;
  }

  return 0;
}



int check_all_elimina_reserva (char *Codigo) {
  /* A funcao um ponteiro para uma string. A funcao vai verificar o tamanho da
  string e verificar se a string representa um codigo de voo se o tamanho for
  igual ou menor a 6, ou se representa um codigo de reserva se for igual ou
  maior a 10. Se nao fo. A funcao retorna 1 se nao existir, retorna 0 se 
  existir um codigo de voo ou reserva igual ao input. 
  Funcao criada para tornar a funcao que auxilia mais legivel. */

  int i, size;
  Reservation *Point;

  size = strlen(Codigo);

  if (size > 6 && size < 10) {
    printf(ERROR_E_CODE);
      return 1;
  }

  /* Percorre todos os vos e respetivas reservas */
  for (i = 0; i < Voos_Ocupados; i++) {

    if (strcmp(Voos[i].Codigo_Voo, Codigo) == 0) {
      break;
    }
    Point = Voos[i].Reservas_Cabeca;
    while (Point != NULL) {
      if (strcmp(Point->Codigo_Reserva, Codigo) == 0) {
        return 0;
      }
      Point = Point->next;
    }
    if ((Voos_Ocupados-1) == i) {
      printf(ERROR_E_CODE);
      return 1;
    }
  }
  return 0;
}



void free_reservas(int i) {
  /* A funcao recebe um inteiro. A funcao vai usar a funcao free de forma 
  continua de modo a libertar toda a memoria alocada para as reservas do 
  respetivo voo de indice i, tanto para os codigos de reserva como para a 
  struct reserva. A funcao nao retorna nada.  
  Funcao criada para tornar as funcoes que auxilia mais legiveis. */

  Reservation *Ponteiro;
  Reservation *Aux;
  
  Ponteiro = Voos[i].Reservas_Cabeca;
  
  while (Ponteiro != NULL) {
    Aux = Ponteiro->next;
    free(Ponteiro->Codigo_Reserva);
    free(Ponteiro);
    Ponteiro = Aux;
  }
  Voos[i].Reservas_Cabeca = NULL;
  return;
}



void termina_programa() {
  /* A funcao nao recebe nada. A funcao vai dar usar a funcao free_reservas de
  forma continua de modo a libertar toda a memoria alocada, tanto para os 
  codigos de reserva como para as structs das reservas. 
  A funcao nao retorna nada.  
  Funcao criada para tornar as funcoes que auxilia mais legiveis. */

  int i;

  for (i = 0; i < Voos_Ocupados; i++) {
    free_reservas(i);
  }

  return;
}
/*----------------------- FUNCOES SECUNDARIAS -----------------------*/


void adiciona_aeroporto() {
  /* A funcao nao recebe nada. A funcao usa a funcao scanf para receber 3
  inputs, sendo que o ultimo pode conter espacos e tabulacao. Apos verificar
  se o numero de aeroportos ja foi excedido, se o ID eh duplicado ou se o ID
  nao e composto por caracteres para alem de maisculas a funcao vai guardar
  as informacoes num novo indice de aeroporto e dar print ao ID, incrementado 
  a variavel global Aeroportos_Ocupados. A funcao nao retorna nada. */
 
  char ID[ID_MAX], Pais[PAIS_MAX], Cidade[CIDADE_MAX];

  scanf("%s %s %[^\n]", ID, Pais, Cidade);

  /* Chama a funcao que verifica todas as condicoes */
  if (check_all_aeroporto(ID)) {
    return;
  }
  
  /* Adiciona os Aeroportos apos verificar condicoes */
  strcpy(Aeroportos[Aeroportos_Ocupados].ID, ID);
  strcpy(Aeroportos[Aeroportos_Ocupados].Pais, Pais); 
  strcpy(Aeroportos[Aeroportos_Ocupados].Cidade, Cidade);
  Aeroportos_Ocupados++;
  printf("airport %s\n", ID);
  return;
}



void lista_aeroportos() {
  /* A funcao nao recebe nada. A funcao vai primeiro correr a funcao 
  conta_voos, posteriormente vai usar a funcao getchar para saber qual o char 
  que vem apos o scanf dos IDs. Se o char for o espaco vai continuar, se for 
  o \n vai parar. Se o programa nao correr uma unica vez (usando a variavel
  count como refernecia) o programa vai por sua vez correr o programa 
  lista_alfabeticamente. A funcao nao retorna nada. */

  int i;
  int count = 0; /* Conta o numero de vezes que o programa deu scan a IDs */
  char car; /* Vai guardar os chars que se encontram ao fim dos IDs */
  char ID[ID_MAX]; 

  conta_voos();

  while ((car = getchar()) != '\n') {
    
    fgets(ID, ID_MAX, stdin);
    count++; /*verifica se o programa deu scan a pelo menos um IDD*/

    for (i = 0; i < Aeroportos_Ocupados; i++) {
      if (strcmp(Aeroportos[i].ID,ID) == 0) {
        printf("%s %s ", Aeroportos[i].ID, Aeroportos[i].Cidade);
        printf("%s %d\n", Aeroportos[i].Pais, Aeroportos[i].Numero_Voos);
        break;
      }
      else if (i == (Aeroportos_Ocupados - 1)) {
        printf("%s", ID);
        printf(ERROR_A_NOTFOUND);
      }
    }
  }
  if (count == 0) {
    lista_alfabeticamente();
  }
  return; 
}



void adiciona_voo() {
  /* A funcao nao recebe nada. A funcao vai ver se apos o comando vem o \n,
  se sim a funcao vai dar printf a todos os voos e respetivas informacoes.
  Caso contrario vai dar scanf a todos os parametros e apos verificar todas
  as condicoes necessarias vai a funcao vai guardar as informacoes num novo 
  indice de voo, incrementado a variavel global Voos_Ocupados. A funcao
  neste condicao nao da printf a nada. A funcao nao retorna nada. */

  int i;
  char car; /* Vai guardar os chars que se encontra apos o comando */
  char Codigo_Voo[CV_MAX], ID_Partida[ID_MAX], ID_Chegada[ID_MAX];
  int Dia, Mes, Ano, Hora, Minuto, Duracao_Hora, Duracao_Minuto, Capacidade;

  if ((car = getchar()) == '\n') {
    for (i = 0; i < Voos_Ocupados; i++) {
      printf("%s ", Voos[i].Codigo_Voo);
      printf("%s %s ", Voos[i].Aeroporto_Partida, Voos[i].Aeroporto_Chegada);
      printf("%02d-%02d-%d ",Voos[i].Data[0],Voos[i].Data[1],Voos[i].Data[2]);
      printf("%02d:%02d\n",Voos[i].Hora[0],Voos[i].Hora[1]);
    }
    return;
  }

  /* Le os parametros do comando */
  scanf("%s %s %s", Codigo_Voo, ID_Partida, ID_Chegada);
  scanf("%d-%d-%d %d:%d", &Dia, &Mes, &Ano, &Hora, &Minuto);
  scanf("%d:%d %d", &Duracao_Hora, &Duracao_Minuto, &Capacidade);

  /* Chama a funcao que verifica todas as condicoes */
  if (check_all_voo(Codigo_Voo, Dia, Mes, Ano, ID_Partida, ID_Chegada, 
  Duracao_Hora, Duracao_Minuto, Capacidade)) {
    return;
  }

  /* Adiciona as informacoes dos Voos ao respetivo indice */
  strcpy(Voos[Voos_Ocupados].Codigo_Voo, Codigo_Voo);
  strcpy(Voos[Voos_Ocupados].Aeroporto_Partida, ID_Partida); 
  strcpy(Voos[Voos_Ocupados].Aeroporto_Chegada, ID_Chegada);
  Voos[Voos_Ocupados].Data[0] = Dia;
  Voos[Voos_Ocupados].Data[1] = Mes;
  Voos[Voos_Ocupados].Data[2] = Ano;
  Voos[Voos_Ocupados].Hora[0] = Hora;
  Voos[Voos_Ocupados].Hora[1] = Minuto;
  Voos[Voos_Ocupados].Duracao[0] = Duracao_Hora;
  Voos[Voos_Ocupados].Duracao[1] = Duracao_Minuto;
  Voos[Voos_Ocupados].Capacidade = Capacidade;
  adiciona_chegada(Voos_Ocupados);
  Voos_Ocupados++;
  return;
}



void lista_voo_partida() {
  /* A funcao nao recebe nada. A funcao vai dar scan a um ID e verificar se
  este existe. Posteriormente vai criar um vetor com todos os indices de
  voo com ID, igual ao recebido, seguidos e vai conta-los. Apos essa operacao
  vai correr o "algoritmo" de ordenacao. O algoritmo comeca por transfromar o
  indice i na sua respetiva representacao no vector de ordenacao. De seguida
  vai transformar a data numa string organizada de informacao menos especifca
  para mais especifica, ou seja, ano, mes, dia, hora, minuto. Apos isso vai 
  correr uma especie de selection sort e por fim vai correr um ciclo diferente 
  para dar print ah informacao pela ordem do vector.
  A funcao nao retorna nada. */

  int i, j, k;
  int h = 0;
  char ID_Partida[ID_MAX];
  char data_voo[DATA_STRING_MAX]; /* Guarda a data com formato de string */
  char menor_ID[DATA_STRING_MAX]="9999-12-31 23:59"; /* Guarda a menor data */
  char ultimo_ID[DATA_STRING_MAX]="0000-00-00 00:00"; /* Guarda a ultima data */
  int vector_ordem[M]; /* Vector que guarda os indices dos Voos com ID igual */
  int troca = 0; /* 0 se nao houve troca nos indices originais, 1 se houve */
  int aux; /* Guarda informacao temporariamente para ordenar o vector */
  int min; /* Guarda o indice que corresponde ao voo com data menor */

  scanf("%s", ID_Partida);

  /* Validacao do ID */
  if (id_check(ID_Partida) == 0) {
    printf("%s", ID_Partida);
    printf(ERROR_A_NOTFOUND);
    return;
  }

  /* Cria um vetor com os indices seguidos dos voos que tem o ID pretendido */
  for (i = 0; i < Voos_Ocupados; i++) {
    if (strcmp(Voos[i].Aeroporto_Partida, ID_Partida) == 0) {
      vector_ordem[h] = i;
      h++;
    }
  }

  /* Ordenacao dos indices do vetor baseando-se na data do voo */
  for (j = 0; j < h; j++) {
    for (i = j; i < h; i++) {
      k = vector_ordem[i];
      sprintf(data_voo, "%04d-%02d-%02d %02d:%02d", Voos[k].Data[2], 
      Voos[k].Data[1],Voos[k].Data[0],Voos[k].Hora[0],Voos[k].Hora[1]);

      if ((strcmp(data_voo, menor_ID)<0) && (strcmp(ultimo_ID, data_voo)<0)) {        
        strcpy(menor_ID, data_voo);
        troca = 1;
        min = i;
      }
    }
    if (troca == 0) break;
    aux = vector_ordem[min];
    vector_ordem[min] = vector_ordem[j];
    vector_ordem[j] = aux;
    strcpy(ultimo_ID, menor_ID);
    strcpy(menor_ID, "9999-12-31 23:59");
  }

  /* Printf dos voos seguindo a ordem dos indices no vector */
  for (i = 0; i < h; i++){
    k = vector_ordem[i],
    printf("%s ", Voos[k].Codigo_Voo);
    printf("%s ", Voos[k].Aeroporto_Chegada);
    printf("%02d-",Voos[k].Data[0]);
    printf("%02d-%d ",Voos[k].Data[1], Voos[k].Data[2]);
    printf("%02d:%02d\n",Voos[k].Hora[0], Voos[k].Hora[1]);
  }
  return;
}



void lista_voo_chegada() {
  /* A funcao nao recebe nada. A funcao vai dar scan a um ID e verificar se
  este existe. Posteriormente vai criar um vetor com todos os indices de
  voo com ID, igual ao recebido, seguidos e vai conta-los. Apos essa operacao
  vai correr o "algoritmo" de ordenacao. O algoritmo comeca por transfromar o
  indice i na sua respetiva representacao no vector de ordenacao. De seguida
  vai transformar a data numa string organizada de informacao menos especifca
  para mais especifica, ou seja, ano, mes, dia, hora, minuto. Apos isso vai 
  correr uma especie de selection sort e por fim vai correr um ciclo diferente 
  para dar print ah informacao pela ordem do vector.
  A funcao nao retorna nada. */

  int i, j, k;
  int h = 0;
  char ID_Chegada[ID_MAX];
  char data_voo[DATA_STRING_MAX]; /* Guarda a data com formato de string */
  char menor_ID[DATA_STRING_MAX]="9999-12-31 23:59"; /* Guarda a menor data */
  char ultimo_ID[DATA_STRING_MAX]="0000-00-00 00:00"; /* Guarda a ultima data */
  int vector_ordem[M]; /* Vector que guarda os indices dos Voos com ID igual */
  int troca = 0; /* 0 se nao houve troca nos indices originais, 1 se houve */
  int aux; /* Guarda informacao temporariamente para ordenar o vector */
  int min; /* Guarda o indice que corresponde ao voo com data menor */

  scanf("%s", ID_Chegada);

  /* Validacao do ID */
  if (id_check(ID_Chegada) == 0) {
    printf("%s", ID_Chegada);
    printf(ERROR_A_NOTFOUND);
    return;
  }

  /* Cria um vetor com os indices seguidos dos voos que tem o ID pretendido */
  for (i = 0; i < Voos_Ocupados; i++) {
    if (strcmp(Voos[i].Aeroporto_Chegada, ID_Chegada) == 0) {
      vector_ordem[h] = i;
      h++;
    }
  }

  /* Ordenacao dos indices do vetor baseando-se na data do voo */
  for (j = 0; j < h; j++) {
    for (i = j; i < h; i++) {
      k = vector_ordem[i];
      sprintf(data_voo, "%04d-%02d-%02d %02d:%02d",Voos[k].C_Data[2], 
      Voos[k].C_Data[1],Voos[k].C_Data[0],Voos[k].C_Hora[0], Voos[k].C_Hora[1]);

      if ((strcmp(data_voo, menor_ID)<0) && (strcmp(ultimo_ID, data_voo)<0)) {        
        strcpy(menor_ID, data_voo);
        troca = 1;
        min = i;
      }
    }
    if (troca == 0) break;
    aux = vector_ordem[min];
    vector_ordem[min] = vector_ordem[j];
    vector_ordem[j] = aux;
    strcpy(ultimo_ID, menor_ID);
    strcpy(menor_ID, "9999-12-31 23:59");
  }

  /* Printf dos voos seguindo a ordem dos indices no vector */
  for (i = 0; i < h; i++){
    k = vector_ordem[i],
    printf("%s ", Voos[k].Codigo_Voo);
    printf("%s ", Voos[k].Aeroporto_Partida);
    printf("%02d-",Voos[k].C_Data[0]);
    printf("%02d-%d ",Voos[k].C_Data[1], Voos[k].C_Data[2]);
    printf("%02d:%02d\n",Voos[k].C_Hora[0], Voos[k].C_Hora[1]);
  }
  return;
}



void avanca_data() {
  /* Funcao nao recebe nada. A funcao vai dar scanf a 3 numeros que representam
  o dia, mes e ano respetivamente. Apos receber vai verificar se o input eh
  valido e se tal acontecer vai atribuir esses valores ah variavel global que 
  define a data presente, e termina por dar print ah nova data. A funcao 
  nao retorna nada. */

  int Dia, Mes, Ano;
  scanf("%d-%d-%d", &Dia, &Mes, &Ano);
  

  /* Verfica se a data estah dentro dos limites pretendido */
  if ((Ano > 2023) || (Ano - Data[2] < 0) || 
      ((Ano - Data[2]) == 0 && (Mes - Data[1]) < 0) ||
      ((Ano - Data[2]) == 0 && (Mes - Data[1]) == 0 && (Dia - Data[0]) < 0)) {
        printf(ERROR_DATE); 
        return;
  }
  /* Atualizacao dos valores do dia, mes e ano na variavel global da data */
  Data[0] = Dia;
  Data[1] = Mes;
  Data[2] = Ano;
  printf("%02d-%02d-%d\n", Data[0],Data[1],Data[2]);
  return;  
}



void adiciona_reserva() {
  /* A funcao nao recebe nada. A funcao incialmente da scaf a dois inputs (um
  codigo de voo e uma data). Se o proximo char for um \n a funcao vai dar print
  a lista de reservas para o voo do dia recebido. Caso contrario a funcao vai
  dar mais dois scans para receber o Codigo de reservas e o numero de pessoas.
  De seguida vai verificar a validade dos inputs e de seguida comecar a alocar
  a memoria necessaria para criar a reserva. De seguida vai usar a fuuncao
  strcmp para ordenar alfabeticamente a lista na posicao devida e alterar
  os ponteiros necessarios. 
  A funcao nao retorna nada.*/

  char Codigo_Voo[CV_MAX];
  char CR_Holder[RESERVA_MAX];
  int N_Pessoas;
  int Dia, Mes, Ano;
  char car;
  int i, j, size;
  char *Codigo_Dinamico;

  Reservation *Reserva_Nova;
  Reservation *Point_Atual;
  Reservation *Point_Aux;

  scanf("%s", Codigo_Voo);
  scanf("%d-%d-%d", &Dia, &Mes, &Ano);

  if ((car = getchar()) != '\n') {
 
    scanf("%s %d", CR_Holder, &N_Pessoas);

    if (check_all_add_reserva (Codigo_Voo,CR_Holder,Dia,Mes,Ano,N_Pessoas)) {
      return;
    }
    
    size = strlen(CR_Holder);

    /* Aloca memoria para o codigo */
    Codigo_Dinamico = (char*) malloc(sizeof(char)*(size+1));
    if (Codigo_Dinamico == NULL) {
      termina_programa();
      termina = 1;
      return;
    }

    /* Passa o codigo recebido no scan para a memoria alocada dinamicamente */
    strcpy(Codigo_Dinamico, CR_Holder);

    /* Aloca memoria para a reserva */
    Reserva_Nova = (Reservation*) malloc(sizeof(Reservation));
    if (Reserva_Nova == NULL) {
      free(Codigo_Dinamico);
      termina_programa();
      termina = 1;
      return;
    }

    /* Associa os parametros ah struct nova */
    strcpy(Reserva_Nova->Codigo_Voo, Codigo_Voo);
    Reserva_Nova->Data[0] = Dia;
    Reserva_Nova->Data[1] = Mes;
    Reserva_Nova->Data[2] = Ano;
    Reserva_Nova->Codigo_Reserva = Codigo_Dinamico;
    Reserva_Nova->Numero_Pessoas = N_Pessoas;
    Reserva_Nova->next = NULL;

    for (i = 0; i < Voos_Ocupados; i++ ) {

      if ((strcmp(Voos[i].Codigo_Voo, Codigo_Voo) == 0) && 
      Voos[i].Data[0]==Dia && Voos[i].Data[1]==Mes && Voos[i].Data[2]==Ano) {
        
        Point_Atual = Voos[i].Reservas_Cabeca; 

        /* Verifica se o voo tem cabeca reserva ou se eh alfabeticamente antes
        que a cabeca atual */
        if (Voos[i].Reservas_Cabeca == NULL ||
        strcmp(Voos[i].Reservas_Cabeca->Codigo_Reserva, Codigo_Dinamico) > 0) {
          Reserva_Nova->next = Voos[i].Reservas_Cabeca;
          Voos[i].Reservas_Cabeca = Reserva_Nova;
          return;
        }

        /* Ordena a reserva alfabeticamente nas posicoes restantes */
        for (j = 0; j <= Voos[i].Capacidade; j++) {
          /* Coloca-se no fim da lista ja que eh alfabeticamente posterior */
          if (Point_Atual->next == NULL) {
            Point_Atual->next = Reserva_Nova;
            return;
          } 
          /* Troca com uma reserva existente */
          if (strcmp(Point_Atual->next->Codigo_Reserva, Codigo_Dinamico) > 0) {
            Point_Aux = Point_Atual->next;
            Point_Atual->next = Reserva_Nova;
            Reserva_Nova->next = Point_Aux;
            return;
          }
          Point_Atual = Point_Atual->next;
        }
      }
    }
  }

  /* Lista as reservas */
  if (car == '\n') {
    if (check_part_add_reserva(Codigo_Voo, Dia, Mes, Ano)) {
    return;
    }
    for (i = 0; i < Voos_Ocupados; i++) {
      if (strcmp(Voos[i].Codigo_Voo, Codigo_Voo) == 0 && Dia == Voos[i].Data[0]
      && Mes == Voos[i].Data[1] && Ano == Voos[i].Data[2]) {
        Point_Atual = Voos[i].Reservas_Cabeca;
        while (Point_Atual != NULL) {
          printf("%s ", Point_Atual->Codigo_Reserva);
          printf("%d\n", Point_Atual->Numero_Pessoas);
          Point_Atual = Point_Atual->next;
        }
        return;
      }
    }
  }
}


void elimina_reserva() {
  /* A funao nao recebe nada. A funcao comeca por da scan a um codigo. De
  seguida vai ver a validade desse codigo e posteriormente vai verificar o 
  tamanho do codigo. Se este for menor ou igual a 6 vai percorrer todos os
  codigos de voo ate encontrar um correspondencia com o codigo recebido, se 
  for maior ou igual a 10 vai percorrer todas as reservas de todos os voos ate 
  encontrar uma correspondencia com o codigo recebido. Se o codigo for de um
  voo ira apagar as reservas feitas para esse voo e vai passar continuamente
  todos os voos de indice superior para "tras" e diminuir o numero de voos
  ocupados. Se o codigo corresponder a uma reserva apenas vai dar free ah
  memoria que essa reserva ocupava e alterar o ponteiro da reserva que lhe 
  antecedia.
  A funcao nao retorna nada. */ 

  int i, j;
  char Codigo[RESERVA_MAX];

  Reservation *Point;
  Reservation *Point_Aux;

  scanf("%s", Codigo);

  if (check_all_elimina_reserva(Codigo)) {
    return;
  }

  /* Apaga voos */
  if (strlen(Codigo) <= 6) {
    for (i = 0; i < Voos_Ocupados; i++) {
      if (strcmp(Voos[i].Codigo_Voo, Codigo) == 0) {
        free_reservas(i);
        for (j = i; j < (Voos_Ocupados - 1); j++) {
          strcpy(Voos[j].Codigo_Voo, Voos[j+1].Codigo_Voo);
          strcpy(Voos[j].Aeroporto_Partida, Voos[j+1].Aeroporto_Partida); 
          strcpy(Voos[j].Aeroporto_Chegada, Voos[j+1].Aeroporto_Chegada);
          Voos[j].Data[0] = Voos[j+1].Data[0];
          Voos[j].Data[1] = Voos[j+1].Data[1];
          Voos[j].Data[2] = Voos[j+1].Data[2];
          Voos[j].Hora[0] = Voos[j+1].Hora[0];
          Voos[j].Hora[1] = Voos[j+1].Hora[1];
          Voos[j].Duracao[0] = Voos[j+1].Duracao[0];
          Voos[j].Duracao[1] = Voos[j+1].Duracao[1];
          Voos[j].Capacidade = Voos[j+1].Capacidade;
          Voos[j].Reservas_Cabeca = Voos[j+1].Reservas_Cabeca; 
          Voos[j+1].Reservas_Cabeca = NULL; 
          adiciona_chegada(j);
        }
        i--;
        Voos_Ocupados--;
      }
    }
    return;   
  }

  /* Apaga reservas */
  else {
    
    for (i = 0; i < Voos_Ocupados; i++) {

      if (Voos[i].Reservas_Cabeca == NULL) {
        continue;
      }
      /* Apaga se for a cabeca do voo */
      if (strcmp(Voos[i].Reservas_Cabeca->Codigo_Reserva, Codigo) == 0) {
        Point_Aux = Voos[i].Reservas_Cabeca;
        Voos[i].Reservas_Cabeca = Voos[i].Reservas_Cabeca->next;
        free(Point_Aux->Codigo_Reserva);
        free(Point_Aux);
        return;
      }
      /* Apaga qualquer outra posicao */
      Point = Voos[i].Reservas_Cabeca;
      while (Point->next != NULL) {
        if (strcmp(Point->next->Codigo_Reserva, Codigo) == 0) {
          free(Point->next->Codigo_Reserva);
          free(Point->next);
          Point->next = Point->next->next;
          return;
        }
        Point = Point->next;
      }
    }
  }
}


/*----------------------- FUNCAO MAIN -----------------------*/


int main () {
  /* A funcao nao recebe nada. A funcao comporta-se como um hub onde o 
  primeiro char de cada linha eh lido e dependendo do char a funcao chama
  outra funcao que representa o proposito pretendido com a chamada da letra.
  A funcao retorna 0 apos a chamada do comando q, cujo proposito eh 
  terminar todos os processos apagando toda a memoria alocada. */

  char car;
  Data[0] = 1; /* Marca o dia referente ao inicio do programa */
  Data[1] = 1; /* Marca o mes referente ao inicio do programa */
  Data[2] = 2022; /* Marca o ano referente ao inicio do programa */

  /* Hub composto por um Loop Infinito com a invocao da respetiva funcao */
  for(;;) {

    switch (car = getchar()) {
      case 'q':
      termina_programa();
        return 0;
      case 'a':
        adiciona_aeroporto();
        break;
      case 'l':
        lista_aeroportos();
        break;
      case 'v':
        adiciona_voo();
        break;
      case 'p':
        lista_voo_partida();
        break;
      case 'c':
        lista_voo_chegada();
        break;
      case 't':
        avanca_data();
        break;
      case 'r':
        adiciona_reserva();
        if (termina == 1) {
          printf(ERROR_N_MEMORY);
          return 0;
        }
        break;
      case 'e':
        elimina_reserva();
        break;
        
    }
  }
  return 0;
}
