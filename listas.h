#ifndef listas
#define listas

#include"main.h"

//Funções usadas para leitura dos ficheiros
viagens_node *leitura_v(char [], estacoes_node*);
estacoes_node *leitura_e(char []);
//Função suada para criar memória para um novo nó
viagens_node *GetNewNode_trips (viagens);
//Função usada para adicionar um nó na lista ordenadamenta
viagens_node *insert_ordered_trips( viagens_node *, viagens, viagens_node **);
//Função suada para criar memória para um novo nó
estacoes_node *GetNewNode_stations(estacoes);
//Função usada para adicionar uma cauda na lista
estacoes_node *insert_tail_stations( estacoes_node *, estacoes, estacoes_node **);
//função que retorna ponteiro para um nó da lista de estações dado o id
estacoes_node *procurar(estacoes_node *head, int i);
//função que dá free à lista de estações
void free_estacoes(estacoes_node *lista_estacoes);
//função que apaga um nó na lista de estações
void apagar_no(viagens_node **, viagens_node **);
//função que dá free à lista de viagens
void free_viagens(viagens_node *head);

#endif
