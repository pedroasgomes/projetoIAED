/*
Primeiro projeto da cadeira de Introducao aos Algoritmos e Estruturas de Dados.
Construcao de um sistema de gestao de voos.

Nome: Pedro Afonso Saraiva Gomes
Numero: 103468
E-mail: pedroafonsogomes3@tecnico.ulisboa.pt  ou  pedroafonsogomes3@gmail.com
Data: 29/03/2022
*/


#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define N 40 /* Guarda o numero maximo de Aeroportos */
#define M 30000 /* Guarda o numero maximo de Voos */
#define DATA_STRING_MAX 17 /* Guarda o tamanho da string formatada das datas */
#define ID_MAX 4 /* Guarda tamanho dos nomes dos IDs */
#define PAIS_MAX 31 /* Guarda tamanho dos nomes dos Paises */
#define CIDADE_MAX 51 /* Guarda tamanho dos nomes das cidades */
#define CODIGO_VOO_MAX 7 /* Guarda o tamanho dos codigos de voo */

#define ERROR_A_ID "invalid airport ID\n" 
#define ERROR_A_MAXED "too many airports\n"
#define ERROR_A_DUPLICATED "duplicate airport\n"
#define ERROR_A_NOTFOUND ": no such airport ID\n"

#define ERROR_F_CODE "invalid flight code\n"
#define ERROR_F_DUPLICATED "flight already exists\n"
#define ERROR_F_MAXED "too many flights\n"
#define ERROR_F_DURATION "invalid duration\n"
#define ERROR_F_CAPACITY "invalid capacity\n"

#define ERROR_DATE "invalid date\n"

/*----------------------- ESTRUTURAS -----------------------*/

/* Definicao do novo tipo que representam Aeroportos */
typedef struct Airport{
  char ID[ID_MAX];
  char Pais[PAIS_MAX];
  char Cidade[CIDADE_MAX];
  int Numero_Voos;
} Airport;


/* Definicao do novo tipo que representam Voos */
typedef struct Flights{
  char Codigo_Voo[CODIGO_VOO_MAX];
  char Aeroporto_Partida[ID_MAX];
  char Aeroporto_Chegada[ID_MAX];
  int Data[3];
  int Hora[2];
  int Duracao[2];
  int Capacidade;
  int C_Data[3];
  int C_Hora[2];
} Flights;



/*----------------------- VARIAVEIS GLOBAIS -----------------------*/

/* Struct que representa os Aeroportos e as suas informacoes */
Airport Aeroportos[N]; 

/* Struct que representa os Voos e as suas informacoes */
Flights Voos[M]; 

/*Vetor que guarda o Dia, Mes e Ano atual, por essa ordem */
int Data[3]; 

/* Variavel que marca quantos Aeroportos existem atualmente */
int Aeroportos_Ocupados = 0; 

/* Variavel que marca quantos voos existem atualmente */
int Voos_Ocupados = 0;

/*----------------------- FUNCOES AUXILIARES -----------------------*/

int id_check(char ID[ID_MAX]){ 
  /* Funcao recebe um ID, e vai percorrer todos os aeroportos e comparar o ID 
  recebido com os IDs dos varios aeroportos ate encontrar um que seja igual. 
  A funcao retorna 1 se encontrar um ID igual e retorna 0 caso contario */

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



void adiciona_chegada (int i) { 
  /* A funcao nao recebe nada. A funcao recebe um inteiro i que representa o 
  numero de um aeroporto e adiciona-lhe a sua  informacao de chegada 
  (Data e Hora), calculando sempre  se a duracao do voo faz com que o dia, 
  mes ou ano se alterem e modifica-os de acordo. A funcao nao retorna nada. */

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
  if (d_chegada > 31 && (m_chegada == 1 || m_chegada == 3 || m_chegada == 5 ||
  m_chegada == 7 || m_chegada == 8 || m_chegada == 10 || m_chegada == 12)) {
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

void conta_voos() { 
  /* A funcao nao recebe nada. A funcao calcula o numero de voos de cada 
  aeroporto e adiciona essa informacao ao respetivo aeroporto para ser usado 
  posteriormente na funcao L (lista_aeroportos). A funcao nao retorna nada. */

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

void lista_alfabeticamente () { 
  /* A funcao nao recebe nada. A funcao mapeia os indices dos Aeroportos com 
  base na organizacao alfabetica dos IDs usando a funcao strcmp para comparar
  os diferentes IDs e tendo em conta que @ < A < ... < Z < [] na tabela ASCII,
  guardando os indices ordenados num vector. Apos organizar os indices a funcao 
  vai dar print a informacoes dos Aeroportos seguindo a ordem dos indices que
  se encontram no vector. A funcao nao retorna nada. */

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


/*----------------------- FUNCOES SECUNDARIAS -----------------------*/



void adiciona_aeroporto() {
  /* A funcao nao recebe nada. A funcao usa a funcao scanf para receber 3
  inputs, sendo que o ultimo pode conter espacos e tabulacao. Apos verificar
  se o numero de aeroportos ja foi excedido, se o ID eh duplicado ou se o ID
  nao e composto por caracteres para alem de maisculas a funcao vai guardar
  as informacoes num novo indice de aeroporto e dar print ao ID, incrementado 
  a variavel global Aeroportos_Ocupados. A funcao nao retorna nada. */
 
  int i;
  char ID[ID_MAX], Pais[PAIS_MAX], Cidade[CIDADE_MAX];

  scanf("%s %s %[^\n]", ID, Pais, Cidade);

  /* Verifica se tem mais de 40 aeroportos */
  if (Aeroportos_Ocupados == N) {
    printf(ERROR_A_MAXED);
    return;
  }
  /* Verifica se o IDD estah em uppercase */
  for (i = 0; i < ID_MAX; i++) {
    if (islower(ID[i])) {
      printf(ERROR_A_ID);
      return;
    }
  }
  /* Verifica se o IDD eh duplicado */
  if (id_check(ID)) {
    printf(ERROR_A_DUPLICATED);
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
  char Codigo_Voo[CODIGO_VOO_MAX], ID_Partida[ID_MAX], ID_Chegada[ID_MAX];
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

  scanf("%s %s %s", Codigo_Voo, ID_Partida, ID_Chegada);
  scanf("%d-%d-%d %d:%d", &Dia, &Mes, &Ano, &Hora, &Minuto);
  scanf("%d:%d %d", &Duracao_Hora, &Duracao_Minuto, &Capacidade);

  /* Codigo de voo invalido (Primeiros elementos nao sao letras maisuclas) */
  for (i = 0; i < 2; i++) {
    if (Codigo_Voo[i] < 'A' || Codigo_Voo[i] > 'Z') {
      printf(ERROR_F_CODE);
      return;
    }
  }

  /* Codigo de voo invalido (Ultimos quatro elementos nao sao numeors) */
  for (i = 2; i < 6; i++) {
    if (Codigo_Voo[i] == 0) 
      break; 
    if (Codigo_Voo[i] < '0' || Codigo_Voo[i] > '9') {
      printf(ERROR_F_CODE);
      return;
    }
  }
  
  /* Codigo de Voo Duplicado */
  for (i = 0; i <Voos_Ocupados; i++) { 
    if (strcmp(Voos[i].Codigo_Voo, Codigo_Voo) == 0 &&
    (Voos[i].Data[0]==Dia && Voos[i].Data[1]==Mes && Voos[i].Data[2]==Ano)) {
      printf(ERROR_F_DUPLICATED);
      return;
    }
  }

  /* IDD de Partida ou Chegada nao existe */
  if (id_check(ID_Partida) == 0) {
    printf("%s", ID_Partida);
    printf(ERROR_A_NOTFOUND);
    return;
  }
  if (id_check(ID_Chegada) == 0) {
    printf("%s", ID_Chegada);
    printf(ERROR_A_NOTFOUND);
    return;
  }

  /* Voos ultrapassam o numero limite M */
  if (Voos_Ocupados == M) {
    printf(ERROR_F_MAXED);
    return;
  }

  /* Testa se o voo eh no passado */
  if ((Ano - Data[2] < 0) || 
  ((Ano - Data[2]) == 0 && (Mes - Data[1]) < 0) ||
  ((Ano - Data[2]) == 0 && (Mes - Data[1]) == 0 && (Dia - Data[0]) < 0)) {
    printf(ERROR_DATE); 
    return;
  }

  /* Testa se o voo eh daqui a mais de um ano */
  if ((Ano - Data[2] > 2) || 
  ((Ano - Data[2]) == 1 && (Data[1] - Mes) > 0) ||
  ((Ano - Data[2]) == 1 && (Data[1] - Mes) == 0 && (Dia - Data[0]) > 0)) {
    printf(ERROR_DATE); 
    return;
  }

  /* Duracao do Voo ultrapassam o limite de 12 horas */
  if ((Duracao_Hora > 12) || (Duracao_Hora == 12 && Duracao_Minuto > 0)) {
    printf(ERROR_F_DURATION);
    return;
  }

  /* Capacidade do Voo nao esta dentro dos padroes (entre 10 e 100) */
  if (Capacidade > 100 || Capacidade < 10) {
    printf(ERROR_F_CAPACITY);
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
  verificar se a string eh menor que a menor guardada ate ao momento e se 
  eh maior que a ultima guardada. Se for vai guardar o seu indice e vai alterar
  o valor da troca para 1. Apos repetir este processo ate ao fim ou ate o valor
  da troca nao alterar de valor numa iteracao completa a funcao para o ciclo
  e vai correr um ciclo diferente para dar print ah informacao pela ordem do
  vector. A funcao nao retorna nada. */

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
  verificar se a string eh menor que a menor guardada ate ao momento e se 
  eh maior que a ultima guardada. Se for vai guardar o seu indice e vai alterar
  o valor da troca para 1. Apos repetir este processo ate ao fim ou ate o valor
  da troca nao alterar de valor numa iteracao completa a funcao para o ciclo
  e vai correr um ciclo diferente para dar print ah informacao pela ordem do
  vector. A funcao nao retorna nada. */

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


/*----------------------- FUNCAO MAIN -----------------------*/



int main () {
  /* A funcao nao recebe nada. A funcao comporta-se como um hub onde o 
  primeiro char de cada linha eh lido e dependendo do char a funcao chama
  outra funcao que representa o proposito pretendido com a chamada da letra.
  A funcao retorna 0 apos a chamada do comando q, cujo proposito eh 
  terminar todos os processos. */


  char car;
  Data[0] = 1; /* Marca o dia referente ao inicio do programa */
  Data[1] = 1; /* Marca o mes referente ao inicio do programa */
  Data[2] = 2022; /* Marca o ano referente ao inicio do programa */

  /* Hub composto por um Loop Infinito com a invocao da respetiva funcao */
  for(;;) {

    switch (car = getchar()) {
      case 'q': 
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
    }
  }
  return 0;
}
