//////////////////////////////////////////////////////
////////parte gráfica///////////////////////////////
////////////////////////////////////////////////////
#include"graphic.h"

/**
 * main_graphic: função que executa toda a parte gráfica
 * \param lista_viagens ponteiro para a cabeça da lista de viagens
 * \param lista_estacoes ponteiro para a cabeça da lista de estações
 * \param argv vetor de strings com os argumentos
 */
void main_graphic(viagens_node **lista_viagens, estacoes_node **lista_estacoes, char *argv[])
{
  SDL_Window *window = NULL;
  SDL_Renderer *renderer = NULL;
  SDL_Surface *imgs[4];//vetor das imagens carregadas
  //cores
  SDL_Color cor1 = {185, 211, 238,255}; //azul claro
  SDL_Color cor2 = {67, 110, 238,255}; //azul escuro
  SDL_Color cor3 = {255, 97, 3 ,255}; //laranja
  SDL_Color cor4 = {25, 25, 112,255}; //azul meio escuro
  ///////
  SDL_Event event;
  TTF_Font *font1 = NULL;
  TTF_Font *font2 = NULL;
  estacoes_node *aux;
  estacoes_node *tail;
  int delay = 300;
  int quit = 0;
  int width = MAP_SIZE_X;//largura da janela
  int height = MAP_SIZE_Y;//altura da janela
  int quadrado_clicado = -1;
  int mouseX, mouseY;//variaveis da posição do rato
  int botoes[9] = {0};//vetor dos botões

  tail = find_tail(*lista_estacoes);
  aux = *lista_estacoes;

  // initialize graphics
  InitEverything(width, height, &font1, &font2, imgs, &window, &renderer);

  while( quit == 0 )
  {
    // while there's events to handle
    while( SDL_PollEvent( &event ) )
    {
      if( event.type == SDL_QUIT )//esta tecla é a cruz presente no canto superior direito da janela do programa que fecha a janela
      {
        quit = 1;
      }
      else if ( event.type == SDL_KEYDOWN )
      {
        switch ( event.key.keysym.sym )
        {
          case SDLK_q: //se carregar na letra q (minúscula), a janela do jogo fecha

          quit = 1;
          break;
          case SDLK_RIGHT: //se carregar na seta para a direta
            if(aux->next == NULL)//chegou ao fim da lista
              aux = *lista_estacoes;
            else
              aux = aux->next;//avança para a estação com o ID seguinte
          break;
          case SDLK_LEFT: //se carregar na seta para a esquerda
            if(aux->prev == NULL)//chegou ao inicio da lista
              aux = tail;
            else
              aux = aux->prev;//avança para a estação com o ID anterior
          break;
        }
      }
      else if (event.type == SDL_MOUSEBUTTONDOWN)
      {
        switch(event.button.button)
        {
          case SDL_BUTTON_LEFT://se carregar no botão esquerdo do rato/touchpad
            mouseX = event.motion.x;
            mouseY = event.motion.y;
            quadrado_clicado = check_click(mouseX,mouseY);//por cima dos retangulos
            flag_botao(botoes, quadrado_clicado);//avalia se foi selecionado ou desselecionado um ou mais botões
            if(quadrado_clicado != -1 || quadrado_clicado >= 7)
              refazer_lista(lista_viagens, botoes, argv[2], *lista_estacoes);//apresenta as rotas pretendidas
          break;
          default:
          break;
        }
      }
    }

    //render do background
    RenderBackground(imgs, renderer);
    //render das rotas
    draw_lines(renderer, *lista_estacoes, aux,*lista_viagens, botoes);
    //render das estações
    draw_station(renderer, imgs[1], *lista_estacoes);
    //render do design (textos, retangulos e botões)
    RenderDesign_1(renderer, aux, font1, font2, &cor1, &cor2, &cor3, &cor4);
    RenderDesign_2(renderer, imgs, font1, font2, &cor1, &cor2, &cor3);
    RenderDesign_3(renderer, imgs, font1, font2, &cor1, &cor2, &cor3);


    //avaliação da seleção dos botões
    check_rect(botoes,renderer);

    // render in the screen all changes above
    SDL_RenderPresent(renderer);
    // add a delay
    SDL_Delay( delay );
  }


  TTF_CloseFont(font1);
  TTF_CloseFont(font2);
  SDL_FreeSurface(imgs[0]);
  SDL_FreeSurface(imgs[1]);
  SDL_FreeSurface(imgs[2]);
  SDL_FreeSurface(imgs[3]);
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
}



estacoes_node *find_tail(estacoes_node *lista_estacoes)
{
  while (lista_estacoes->next !=NULL) {
    lista_estacoes = lista_estacoes->next;
  }
  return lista_estacoes;
}

/**
 * InitEverything: Initializes the SDL2 library and all graphical components: font, window, renderer
 * \param width width in px of the window
 * \param height height in px of the window
 * \param _font font that will be used to render the text
 * \param _img surface to be created with all images
 * \param _window represents the window of the application
 * \param _renderer renderer to handle all rendering in a window
 */
void InitEverything(int width, int height, TTF_Font **_font1, TTF_Font **_font2 , SDL_Surface *_img[], SDL_Window** _window, SDL_Renderer** _renderer)
{
    InitSDL();
    InitFont();
    *_window = CreateWindow(width, height);
    *_renderer = CreateRenderer(width, height, *_window);

    // load do background(mapa)
    _img[0] = IMG_Load("mapa.png");
    if (_img[0] == NULL)
    {
        printf("Unable to load image: %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }
    //load pin
    _img[1]= IMG_Load("pin.png");
    if (_img[1] == NULL)
    {
        printf("Unable to load image: %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }
    //load da moldura dos botões
    _img[2]= IMG_Load("pre.png");
    if (_img[2] == NULL)
    {
        printf("Unable to load image: %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }
    //load do logo do programa
    _img[3]= IMG_Load("thebikestationlogo.png");
    if (_img[3] == NULL)
    {
        printf("Unable to load image: %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    //font1
    *_font1 = TTF_OpenFont("FreeSerif.ttf", 25);
    if(!*_font1)
    {
        printf("TTF_OpenFont: %s\n", TTF_GetError());
        exit(EXIT_FAILURE);
    }
    //font2
    *_font2 = TTF_OpenFont("FreeSerif.ttf", 16);
    if(!*_font2)
    {
        printf("TTF_OpenFont: %s\n", TTF_GetError());
        exit(EXIT_FAILURE);
    }
}
/////////////////////////////////////////////////////////////////////////////////////////
//////// Funções retiradas do projeto intermédio e que não sofreram alterações //////////
/////////////////////////////////////////////////////////////////////////////////////////
/**
 * InitSDL: Initializes the SDL2 graphic library
 */
void InitSDL()
{
    // init SDL library
	if ( SDL_Init( SDL_INIT_EVERYTHING ) == -1 )
	{
		printf(" Failed to initialize SDL : %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
	}
}

/**
 * InitFont: Initializes the SDL2_ttf font library
 */
void InitFont()
{
	// Init font library
	if(TTF_Init()==-1)
	{
	    printf("TTF_Init: %s\n", TTF_GetError());
        exit(EXIT_FAILURE);
	}
}

/**
 * CreateWindow: Creates a window for the application
 * \param width width in px of the window
 * \param height height in px of the window
 * \return pointer to the window created
 */
SDL_Window* CreateWindow(int width, int height)
{
    SDL_Window *window;
    // init window
	window = SDL_CreateWindow( "T H E   B I K E   S T A T I O N | B O S T O N", WINDOW_POSX, WINDOW_POSY, width, height, 0 );
    // check for error !
	if ( window == NULL )
	{
		printf("Failed to create window : %s\n", SDL_GetError());
		exit(EXIT_FAILURE);
	}
	return window;
}

/**
 * CreateRenderer: Creates a renderer for the application
 * \param width width in px of the window
 * \param height height in px of the window
 * \param _window represents the window for which the renderer is associated
 * \return pointer to the renderer created
 */
SDL_Renderer* CreateRenderer(int width, int height, SDL_Window *_window)
{
    SDL_Renderer *renderer;
    // init renderer
	renderer = SDL_CreateRenderer( _window, -1, 0 );

	if ( renderer == NULL )
	{
		printf("Failed to create renderer : %s", SDL_GetError());
        exit(EXIT_FAILURE);
	}

	// set size of renderer to the same as window
	SDL_RenderSetLogicalSize( renderer, width, height );

	return renderer;
}

/**
 * RenderText function: Renders some text on a position inside the app window
 * \param x X coordinate of the text
 * \param y Y coordinate of the text
 * \param text string with the text to be written
 * \param _font TTF font used to render the text
 * \param _color color of the text
 * \param _renderer renderer to handle all rendering in a window
 */
int RenderText(int x, int y, const char *text, TTF_Font *_font, SDL_Color *_color, SDL_Renderer* _renderer)
{
	SDL_Surface *text_surface;
	SDL_Texture *text_texture;
	SDL_Rect solidRect;

	solidRect.x = x;
	solidRect.y = y;
    // create a surface from the string text with a predefined font
	text_surface = TTF_RenderText_Blended(_font,text,*_color);
	if(!text_surface)
	{
	    printf("TTF_RenderText_Blended: %s\n", TTF_GetError());
	    exit(EXIT_FAILURE);
	}
    // create texture
	text_texture = SDL_CreateTextureFromSurface(_renderer, text_surface);
    // obtain size
	SDL_QueryTexture( text_texture, NULL, NULL, &solidRect.w, &solidRect.h );
    // render it !
	SDL_RenderCopy(_renderer, text_texture, NULL, &solidRect);
    // clear memory
	SDL_DestroyTexture(text_texture);
	SDL_FreeSurface(text_surface);
	return solidRect.h;
}
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////

/*
 * RenderBackground: loads the map
 * \param _img surfaces with the mapa_texture
 * \param _renderer renderer to handle all rendering in a window
 */
void RenderBackground(SDL_Surface *_img[], SDL_Renderer* _renderer )
{
    SDL_Texture *mapa_texture;
    SDL_Rect tableSrc, tableDest;

    // set color of renderer to some color
    SDL_SetRenderDrawColor( _renderer, 255, 255, 255, 255 );

    // clear the window
    SDL_RenderClear( _renderer );

    tableDest.x = tableSrc.x = 0;
    tableDest.y = tableSrc.y = 0;
    tableSrc.w = _img[0]->w;
    tableSrc.h = _img[0]->h;
    tableDest.w = MAP_SIZE_X;
    tableDest.h = MAP_SIZE_Y;

    //background
    mapa_texture = SDL_CreateTextureFromSurface(_renderer, _img[0]);
    SDL_RenderCopy(_renderer, mapa_texture, &tableDest,  &tableSrc);

    // destroy everything
    SDL_DestroyTexture(mapa_texture);

}

/*
 * RenderDesign_1: carrega os retangulos e texto
 * \param _font1 e _font2 font that will be used to render the text
 * \param _cor1, _cor2, _cor3, _cor4 that will define the text color
 * \param _img surfaces with the mapa_texture
 * \param _renderer renderer to handle all rendering in a window
 * \param estacao_escolhida ponteiro para a cabeça de lista de estações
 */
void RenderDesign_1( SDL_Renderer *_renderer, estacoes_node *estacao_escolhida, TTF_Font *_font1, TTF_Font *_font2, SDL_Color *_cor1, SDL_Color *_cor2, SDL_Color *_cor3, SDL_Color *_cor4)
{
  char station[1000] = {0}, station1[1000] = {0};

  estacoes_node *aux;
  aux = estacao_escolhida;

  SDL_SetRenderDrawBlendMode(_renderer,SDL_BLENDMODE_BLEND);

  SDL_SetRenderDrawColor(_renderer, _cor1->r, _cor1->g, _cor1->b, 200);
  SDL_Rect retangulo1; //retangulo grande azul
  retangulo1.x=0;
  retangulo1.y=520;
  retangulo1.w=MAP_SIZE_X;
  retangulo1.h=130;
  SDL_RenderFillRect(_renderer, &retangulo1);

  SDL_SetRenderDrawColor(_renderer, 255, 255, 255, 150 );
  SDL_Rect retangulo2; //retangulo grande branco
  retangulo2.x=0;
  retangulo2.y=0;
  retangulo2.w=MAP_SIZE_X;
  retangulo2.h=175;
  SDL_RenderFillRect(_renderer, &retangulo2);

  //texto informativo da grafica --- informa a interação no programa
  RenderText(0, 520, ":::::Selected station:", _font1, _cor2, _renderer);
  RenderText(321, 520, "Use the right arrow or the left arrow for navegation.", _font1, _cor3, _renderer);
  sprintf(station, ":::::ID - %d:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::", aux->info.id_estacao);
  RenderText(0, 550, station, _font1, _cor4, _renderer);
  sprintf(station1, ":::::%s:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::", aux->info.nome_estacao);
  RenderText(0, 580, station1, _font1, _cor4, _renderer);

  SDL_SetRenderDrawBlendMode(_renderer,SDL_BLENDMODE_NONE);
  SDL_SetRenderDrawColor(_renderer, 255, 255, 255, 255 );
}

/*
 * RenderDesign_2: carrega os retangulos, texto, logo do programa e o moldura do botão
 * \param _font1 e _font2 font that will be used to render the text
 * \param _cor1, _cor2, _cor3, _cor4 that will define the text color
 * \param _img surfaces with the mapa_texture
 * \param _renderer renderer to handle all rendering in a window
 * \param _img[] vetor das imagens que vão ser carregadas
 */
void RenderDesign_2( SDL_Renderer *_renderer, SDL_Surface *_img[], TTF_Font *_font1, TTF_Font *_font2,  SDL_Color *_cor1, SDL_Color *_cor2, SDL_Color *_cor3)
{
  SDL_Texture *rectangle_texture;
  SDL_Texture *logo_texture;
  int valor = 0;

  SDL_SetRenderDrawBlendMode(_renderer,SDL_BLENDMODE_BLEND);

  RenderText(0, 1, ":::::Select the kind(s) of trip(s):", _font1, _cor3, _renderer);

  SDL_SetRenderDrawColor(_renderer, _cor1->r, _cor1->g, _cor1->b, 255 );
  SDL_Rect retangulo3; //retangulo
  retangulo3.x=BUTTON_X;
  retangulo3.y=BUTTON_Y;
  retangulo3.w=BUTTON_W;
  retangulo3.h=BUTTON_H;

  SDL_Rect retangulo4;//posição da imagem rectangle.png
  retangulo4.y=BUTTON_Y-6;
  retangulo4.w=BUTTON_W+10;
  retangulo4.h=BUTTON_H+13;
  for (int i = 0; i < 2; i++)
  {
      retangulo3.x+=valor;// 7 blocos
      valor = SEP_BUTTON_X;
      SDL_RenderFillRect(_renderer, &retangulo3);

      retangulo4.x = retangulo3.x-5;// 7 carregamentos da imagem rectangle.png
      rectangle_texture = SDL_CreateTextureFromSurface(_renderer, _img[2]);
      SDL_RenderCopy(_renderer,rectangle_texture, NULL, &retangulo4);
      SDL_DestroyTexture(rectangle_texture);
  }
  RenderText(BUTTON_X+36, BUTTON_Y+10, "IN", _font2, _cor2, _renderer);
  RenderText(BUTTON_X+146, BUTTON_Y+10, "OUT", _font2, _cor2, _renderer);

  //logo - The Bike Station
  SDL_Rect retangulo4_1;//posição da imagem the-bike-station-logo.png
  retangulo4_1.x = BUTTON_X+610;
  retangulo4_1.y=BUTTON_Y-33;
  retangulo4_1.w=_img[3]->w/8.5;
  retangulo4_1.h=_img[3]->h/8.5;

  logo_texture = SDL_CreateTextureFromSurface(_renderer, _img[3]);
  SDL_RenderCopy(_renderer,logo_texture, NULL, &retangulo4_1);
  SDL_DestroyTexture(logo_texture);

  SDL_SetRenderDrawBlendMode(_renderer,SDL_BLENDMODE_NONE);
  SDL_SetRenderDrawColor(_renderer, 255, 255, 255, 255 );
}

/*
 * RenderDesign_3: carrega os retangulos, texto e o moldura do botão
 * \param _font1 e _font2 font that will be used to render the text
 * \param _cor1, _cor2, _cor3, _cor4 that will define the text color
 * \param _img surfaces with the mapa_texture
 * \param _renderer renderer to handle all rendering in a window
 * \param _img[] vetor das imagens que vão ser carregadas
 */
void RenderDesign_3( SDL_Renderer *_renderer, SDL_Surface *_img[], TTF_Font *_font1, TTF_Font *_font2,  SDL_Color *_cor1, SDL_Color *_cor2, SDL_Color *_cor3)
{
  SDL_Texture *rectangle_texture;
  int valor=0;
  char dias_da_semana [7][MAX_STRING] = {"    Sunday", "   Monday", "   Tuesday", "Wednesday", "  Thursday", "    Friday", "   Saturday" };

  SDL_SetRenderDrawBlendMode(_renderer,SDL_BLENDMODE_BLEND);

  SDL_SetRenderDrawColor(_renderer, _cor1->r, _cor1->g, _cor1->b, 255 );

  RenderText(0, SEP_BUTTON_Y, ":::::Select the days that you want to visualize:", _font1, _cor3, _renderer);
  SDL_Rect retangulo5;//retangulo
  retangulo5.x=BUTTON_X;
  retangulo5.y=BUTTON_Y+SEP_BUTTON_Y;
  retangulo5.w=BUTTON_W;
  retangulo5.h=BUTTON_H;

  SDL_Rect retangulo6;//posição da imagem rectangle.png
  retangulo6.x=retangulo5.x;
  retangulo6.y=retangulo5.y-6;
  retangulo6.w=retangulo5.w+10;
  retangulo6.h=retangulo5.h+13;
  for (int i = 0; i < 7; i++)
  {
      retangulo5.x+=valor;// 7 blocos
      valor = SEP_BUTTON_X;
      SDL_RenderFillRect(_renderer, &retangulo5);

      retangulo6.x = retangulo5.x-5;// 7 carregamentos da imagem rectangle.png
      rectangle_texture = SDL_CreateTextureFromSurface(_renderer, _img[2]);
      SDL_RenderCopy(_renderer,rectangle_texture, NULL, &retangulo6);
      SDL_DestroyTexture(rectangle_texture);

      //texto dos dias da semana
      RenderText(retangulo5.x+5, retangulo5.y+10, dias_da_semana[i], _font2, _cor2, _renderer);
  }

  SDL_SetRenderDrawBlendMode(_renderer,SDL_BLENDMODE_NONE);
  SDL_SetRenderDrawColor(_renderer, 255, 255, 255, 255 );
}


/*
 * converter: converte longitude e latitude em x e y
 * \param lat e lon latitude e longitude a serem convertidos
 * \param x e y inteiro que guardam as conversões
 */
void converter(double lon, double lat,int *x, int *y )
{
  double var_lon, valor_lon;
  double var_lat, valor_lat;
  var_lon = +INF_DIR_LON_EST - SUP_ESQ_LON_EST;
  var_lat = -INF_DIR_LAT_EST + SUP_ESQ_LAT_EST;
  valor_lat = SUP_ESQ_LAT_EST - lat;
  valor_lon =  -SUP_ESQ_LON_EST + lon;
  //calculado com regra 3 simples a partir das variasções de lat e long e pixeis
  *x = (MAP_SIZE_X*valor_lon)/var_lon;
  *y = (MAP_SIZE_Y*valor_lat)/var_lat;
}

/*
 * draw_station: faz load de um pin para marcar cada estação existente
 * \param _pin carrega a imagem do pino da estação
 * \param _renderer renderer to handle all rendering in a window
 * \param head ponteiro para a cabeça de lista de estações
 */
void draw_station(SDL_Renderer* _renderer, SDL_Surface *_pin, estacoes_node *head)
{
  SDL_Rect station;
  SDL_Texture* pin_station;
  estacoes_node *aux = head;
  aux = head;
  int x,y;
  //percorre a lista de estações
  while (aux != NULL)
  {
    //converte latitude e longitudem em x e y
    converter(aux->info.lon_estacao, aux->info.lat_estacao,&x, &y );
    station.x = x-10;
    station.y = y-15;
    station.w = 20;
    station.h = 20;
    //em cada estação mete um pino
    pin_station = SDL_CreateTextureFromSurface(_renderer, _pin);
    SDL_RenderCopy(_renderer,pin_station, NULL, &station);
    aux=aux->next;
    SDL_DestroyTexture(pin_station);
  }
}

/*
 * draw_lines: desenha as rotas de cada estação (uma de cada vez)
 * \param _img surfaces with the pin_station
 * \param _renderer renderer to handle all rendering in a window
 * \param lista_estacoes ponteiro para a cabeça de lista de estações
 * \param estacao_escolhida ponteiro para a cabeça de lista de estações
 * \param lista_viagens ponteiro para a cabeça de lista de viagens
 * \param botoes[] vetor do valor dos botões
 */
void draw_lines(SDL_Renderer* _renderer,estacoes_node *lista_estacoes, estacoes_node *estacao_escolhida, viagens_node * lista_viagens, int botoes[])
{
  estacoes_node *aux;
  aux = lista_estacoes;
  int x1,x2,y1,y2;
  int grossura = 0;
  //converte latitutde e longitude para x e y da estação escolhida
  converter(estacao_escolhida->info.lon_estacao,estacao_escolhida->info.lat_estacao,&x1,&y1);
  //ligas trasnparencia
  SDL_SetRenderDrawBlendMode(_renderer,SDL_BLENDMODE_BLEND);

  // set color of renderer to some color
  SDL_SetRenderDrawColor( _renderer, 0, 205, 0, 200 );

  //percorre a lista de estações
  while (aux!=NULL)
  {
    //calcula o número de viagens entre a estação escolhida e as outras uma a uma
    grossura = contar_viagens(estacao_escolhida,aux,lista_viagens, botoes);
    //converte latitutde e longitude para x e y da estação
    converter(aux->info.lon_estacao,aux->info.lat_estacao,&x2,&y2);
    //divide o número de viagens por uma constante para dar a grossura à linha
    if(grossura > 15)
      grossura /= 15;
    else if(grossura != 0)
      grossura = 1; //menos de 20 viagens
    else
      grossura = 0; //nenhuma viagem

    //cria várias linha paralelas criando uma linha mais grossa
    for(int i=-grossura; i<grossura;i++)
      SDL_RenderDrawLine(_renderer,x1,y1+i,x2,y2+i);
    aux = aux->next;
  }

  // set color of renderer to some color
  SDL_SetRenderDrawColor( _renderer, 255, 255, 255, 255 );
  //desligas o alpha(trnasparencia)
  SDL_SetRenderDrawBlendMode(_renderer,SDL_BLENDMODE_NONE);
}

/*
 * contar_viagens:conta numero de viagens entre duas estações
 * \param estacao_escolhida estação escolhida
 * \param estacao_aux outra estação
 * \param lista_viagens ponteiro apra cabeça da lista de viagens
 * \param botoes[] vetor do valor dos botões
 * \return retorna numero de viagens
 */
int contar_viagens(estacoes_node *estacao_escolhida, estacoes_node *estacao_aux, viagens_node *lista_viagens, int botoes[])
{
  int i = 0;
  while (lista_viagens!=NULL) {
    //verifa se botão OUT está ligado e a viagem é de ida
    if(lista_viagens->info.id_inicial->info.id_estacao == estacao_escolhida->info.id_estacao && lista_viagens->info.id_final->info.id_estacao == estacao_aux->info.id_estacao && botoes[8] == 1)
      i++;
    //verifa se o botão IN está ligado e a viagem de de vinda
    if(lista_viagens->info.id_final->info.id_estacao == estacao_escolhida->info.id_estacao && lista_viagens->info.id_inicial->info.id_estacao == estacao_aux->info.id_estacao && botoes[7] == 1)
      i++;
    lista_viagens = lista_viagens->next;
  }
  return i;
}

/*
 * refazer_lista:dá free da lista, volta a ler e filtra consoante os botões da semana
 * \param string nome do ficheiro
 * \param head ponteiro apra cabeça da lista de viagens
 * \param botoes[] vetor do valor dos botões
 */
void refazer_lista(viagens_node **head, int botoes[],char *string, estacoes_node *head_e)
{
  //free da lista
  free_viagens(*head);
  //leitura
  *head = leitura_v(string, head_e);

  viagens_node *aux=NULL;
  int dia_da_semana = 0;
  aux = *head;
  while (aux !=NULL)
  {
    //converte dia da viagem para  dia da semana
    dia_da_semana = converter_para_dia(aux->info.data_inicial);
    //verifica se o botão desse dia da semna está ligado
    if(botoes[dia_da_semana] == 0)
      apagar_no(&aux,head); //apaga se o botão estiver desligado e pmete o aux no próximo
    else
      aux = aux->next;
  }
}

/*
 * check_click: verifica se ele clicou nalgum botão
 * \param _mouseX e _mouseY x e y de onde clicaram
 * \return retorna o valor do botão clicado ou -1 se  foi fora dos botões
 */
int check_click(int _mouseX, int _mouseY )
{
  int xi,xf,yi,yf;
  for (int i = 0; i < 2 ; i++ )
  {
    //ve se o x e y do mouse estão dentro dos dois botõe s em cima(IN, OUT)
    xi =(BUTTON_X + i*SEP_BUTTON_X);
    xf = (BUTTON_X + i*SEP_BUTTON_X) + BUTTON_W;
    yi =BUTTON_X;
    yf= BUTTON_Y+BUTTON_H;
    //se estiver dentro dos parametros retorna o valor do botão
    if(_mouseX<=xf && _mouseX>=xi && _mouseY<=yf && _mouseY>= yi)
    {
      return i+7;
    }
  }

  for (int i = 0; i < 7 ; i++ )
  {
    //ve se o x e y do mouse estão nso 7 blocos dos dia da semna
    xi =(BUTTON_X + i*SEP_BUTTON_X);
    xf = (BUTTON_X + i*SEP_BUTTON_X) + BUTTON_W;
    yi =BUTTON_Y+SEP_BUTTON_Y;
    yf= BUTTON_Y+BUTTON_H+SEP_BUTTON_Y;
    //se estiver dentro dos aprametros retorna o valor do botão
    if(_mouseX<=xf && _mouseX>=xi && _mouseY<=yf && _mouseY>= yi)
    {
      return i;
    }
  }
  return -1;
}

/*
 * check_rect: mete uma layer verde sobre os botões clicados
 * \param botoes vetor dos botões
 * \param _renderer render
 */
void check_rect(int botoes[], SDL_Renderer *_renderer)
{
  int x = 0;
  int y = 0;
  //enable da transparencia
  SDL_SetRenderDrawBlendMode(_renderer,SDL_BLENDMODE_BLEND);
  //percorre o vetor dos botões
  for (int p = 0; p<9; p++)
  {
    //e o botão estiver enable imprimir uma layer verde sobre o botão
    if(botoes[p] == 1)
    {
      x = (BUTTON_X + (p)*SEP_BUTTON_X);
      if(p<=6)
      {
        x = (BUTTON_X + (p)*SEP_BUTTON_X);
        y=BUTTON_Y+SEP_BUTTON_Y;
      }
      else
      {
        x = (BUTTON_X + (p-7)*SEP_BUTTON_X);
        y = BUTTON_Y;
      }
      SDL_SetRenderDrawColor(_renderer, 0, 238, 0, 150 );//verde

      SDL_Rect retangulo2; //retangulo verde
      retangulo2.x=x;
      retangulo2.y=y;
      retangulo2.w=BUTTON_W;
      retangulo2.h=BUTTON_H;
      //cria retangulo verde
      SDL_RenderFillRect(_renderer, &retangulo2);

    }
  }
  //tira o enbale da transparencia
  SDL_SetRenderDrawBlendMode(_renderer,SDL_BLENDMODE_NONE);
}

/*
 * flag_botao: mexe com o valor do botão pressionado
 * \param botoes vetor dos botões
 * \param p numero do botão pressionado
 */
void flag_botao(int botoes[], int p)
{
  //se não clicar num botão sia da função
  if(p == -1)
    return;
  //se o botão não estiver ativado ativa
  if(botoes[p] == 0)
    botoes[p] = 1;
  else
    botoes[p] = 0;  //desativa se tiver ativado
}
