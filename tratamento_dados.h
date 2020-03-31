#ifndef tratamento_dados
#define tratamento_dados

#include"main.h"
//Função usada para extrair informação de uma linha
int separar_parametros(viagens *, char *, estacoes_node *);
//menu de selecao de dados
void selecao_de_dados(tempo_i_f *, int [], int *);
//função usada para escolher horario
tempo_i_f *escolher_horario(tempo_i_f *);
//função usada para escolher dia da semana
int escolher_dia(int);
//função usada para imprimir a samena no ecra
void semana();
//função usada para escolher o numero de dias de semana
int escolhernumor_dias();
//função usada para escolher a duração
void escolher_duracao(int **);
//função que imprime as viagens
void imprimir_viagens(viagens_node *);
//função que imprime uma dada estação
void print_stations(estacoes_node *);
//função que imprime uma dada viagem
void print_trips(viagens_node *, int);
//função que decide que nós eliminar(filtragem)
viagens_node *selecionar( viagens_node *, tempo_i_f, int , int[]);
//função que soma posições do vetor
int somatotal(int vetor[]);
//funçãoq ue imprime os maximos,minimos e medias
void imprimir_max_min(int sair[], int entrar[], tempo_i_f intervalo_tempo);
//imprime as estações
void imprimir_estacoes( viagens_node *head_v, tempo_i_f intervalo_tempo, estacoes_node *head_e);
//função que converte dias,mes,ano para dia da semana
int converter_para_dia(data);
//função usada para imprimir as rotas
int **imprimir_rotas(viagens_node *, estacoes_node *);
//função usada para esperar o utilizador avançar
void wait();
//função usada para escolher estação
int escolher_estacao(estacoes_node *);
//função usada para contar numero de estações
int contar_estacoes(estacoes_node *);
//função usada para fazer estatistica de genero
void estatisticas_males_females(viagens_node *, FILE *);
//função usada para fazer estatisticas de idade
void estatisticas_idades(viagens_node *head_v, FILE * fp);
//função usada para fazer estatisticas de duração
void estatisticas_duracao(viagens_node *head_v, FILE * fp);
//função usada para somar posições de um vetor
int sumatorio(int idades[],int a);
//submenu das estatisticas
void estatisticas(estacoes_node *head_e, viagens_node *head_v);
//menu parte textual
void MainMenu(viagens_node **lista_viagens, estacoes_node **lista_estacoes, char *[]);
//função usada para calcular estatisticas velocidae
void estatisticas_velocidade(estacoes_node *head_e, viagens_node *head_v, FILE *fp);
//converter longitude e latitude em distancia
long double velocidade(double lon1, double lon2, double lat1, double lat2);
//função usada para esperar o utilizador avançar ou sair
int wait_or_leave();
//imprime o menu da parte textual
int menu();
//função usada para reordenar um vetor
void bubble_sort(int numero_estacoes, int **rotas, int a);
//função usada para dar free a um vetor bidimencional
void free_rotas(int **rotas, estacoes_node* head);
//função para calcular numero de rotas
void calcular_n_rotas(viagens_node *aux, int **rotas, int numero_estacoes, int );
//função apra escolher numero de rotas por página
int escolher_pag();

#endif
