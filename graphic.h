#ifndef graphic
#define graphic

#include"main.h"

//Funções que manipulam a parte gráfica
void main_graphic(viagens_node **lista_viagens, estacoes_node **lista_estacoes, char *[]);
void InitEverything(int , int,TTF_Font **, TTF_Font **, SDL_Surface **, SDL_Window ** , SDL_Renderer **);
void InitSDL();
void InitFont();
SDL_Window* CreateWindow(int , int );
SDL_Renderer* CreateRenderer(int , int , SDL_Window *);
int RenderText(int, int, const char *, TTF_Font *, SDL_Color *, SDL_Renderer *);
void RenderBackground(SDL_Surface **, SDL_Renderer *);
void RenderDesign_1( SDL_Renderer *, estacoes_node * , TTF_Font *, TTF_Font *, SDL_Color *, SDL_Color *, SDL_Color *, SDL_Color *);
void RenderDesign_2( SDL_Renderer *, SDL_Surface *[], TTF_Font *, TTF_Font *, SDL_Color *, SDL_Color *, SDL_Color *);
void RenderDesign_3( SDL_Renderer *, SDL_Surface *[], TTF_Font *, TTF_Font *, SDL_Color *, SDL_Color *, SDL_Color *);

//funções para representar a linhas de rotas e as estções
void draw_station(SDL_Renderer *, SDL_Surface *, estacoes_node *);
void draw_lines(SDL_Renderer* _renderer,estacoes_node *lista_estacoes, estacoes_node *estacao_escolhida, viagens_node *, int []);

//Função que converte latitude e longitude para x e y
void converter(double lon, double lat,int *x, int *y );

//Função que conta o numero de viagens
int contar_viagens(estacoes_node *estacao_escolhida,estacoes_node *estacao_aux, viagens_node *lista_viagens, int[]);

//FUnções usadas apra ver se o utilizador clicou num botão e atualizar o botão
int check_click(int _mouseX, int _mouseY );
void check_rect(int botoes[], SDL_Renderer *_renderer);
void flag_botao(int botoes[], int p);

//Funções para manipular a lista de viagens
void refazer_lista(viagens_node **head, int botoes[],char *string, estacoes_node *head_e);
estacoes_node *find_tail(estacoes_node *lista_estacoes);

#endif
