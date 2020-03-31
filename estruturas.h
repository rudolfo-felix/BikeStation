#ifndef estruturas
#define estruturas


typedef struct{
  int dia;
  int mes;
  int ano;
}data;

typedef struct{
  int hora;
  int minuto;
}tempo;

typedef struct{
  int hora_i;
  int hora_f;
}tempo_i_f;

typedef struct{
  char letra_st;
  int numero_st;
}estacao_abreviada;

typedef struct{
  int id_estacao;
  estacao_abreviada cod_estacao;
  char nome_estacao[MAX_STRING];
  char municipio_estacao[MAX_STRING];
  double lat_estacao;
  double lon_estacao;
  char status[MAX_STRING];
}estacoes;

typedef struct node_e{
  estacoes info;
  struct node_e *next;
  struct node_e *prev;
}estacoes_node;

typedef struct{
  int id_viagem;
  int duracao;
  data data_inicial;
  tempo tempo_inicial;
  data data_final;
  tempo tempo_final;
  estacoes_node *id_inicial;
  estacoes_node *id_final;
  estacao_abreviada id_bike;
  int usuario;
  int ano_nascimento;
  int genero;
}viagens;

typedef struct node{
  viagens info;
  struct node *next;
  struct node *prev;
}viagens_node;

#endif
