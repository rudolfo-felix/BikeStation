#include"listas.h"

/**
 * procurar: função que recede um id de estação e rretorna o ponteiro da estação com esse id
 * \param head cabeça da lista de estações
 * \param i id da estação a ser retornada
 * \return ponteiro para a estação com o id i ou null se não existir
 */
estacoes_node *procurar(estacoes_node *head, int i)
{
  //ponteiro auxiliar para percorrer a lista de estações
  estacoes_node *aux = head;
  while (aux!=NULL) {
    //verifica se o id da estação aux é igual a i
    if(i == aux->info.id_estacao)
      return aux;
    aux = aux->next;
  }
  return NULL;
}


/**
 * selecionar: função que filtra a lista de viagens consoante parametros eliminando os nós que estão fora dos parametros
 * \param head cabeça da lista de viagens
 * \param intervalo_tempo hora inicial e final da viagem
 * \param duracao druacao maxima
 * \param dias_da_semana array com 7 posições correspondendo aos dias da semna
 * \return ponteiro para a cabeça da lista de viagens
 */
viagens_node *selecionar( viagens_node *head, tempo_i_f intervalo_tempo, int duracao, int dias_da_semana[])
{
  int flag= 0, dia= 0, flag_dia = 0, p = 0;
  viagens_node *aux1 = NULL;
  //vetor auxiliar para percorrer a lista de viagens
  aux1 = head;
  while (aux1!=NULL)
  {
    //reinicia flags
    flag = 0;
    flag_dia = 0;
    p =0;
    if(duracao != -1)   //se o parametro estiver a -1 quer dizer que não tem filtro
    {
      if(aux1->info.duracao > duracao)  //verifica se a duração da viagem excedo o parametro
      {
        //apagar o nó e deixa o aux1 já no proximo parametro
        apagar_no(&aux1, &head);
        //flag usada para ver se o nó foi eliminado
        flag = 1;
      }
    }else if(intervalo_tempo.hora_i != -1) //se o parametro estiver a -1 quer dizer que não tem filtro
    {
      if((aux1->info.tempo_final.hora >= intervalo_tempo.hora_f) || (aux1->info.tempo_inicial.hora < intervalo_tempo.hora_i) )
      {
        //se as horas da viagem estiveram no intervalo do tempo, elimina o nó
        apagar_no(&aux1, &head);
        flag = 1;
      }
    }else if(dias_da_semana[0]!= -1)  //se o parametro estiver a -1 quer dizer que não tem filtro
    {
      //converte o dia da viagem para dia da semana
      dia = converter_para_dia(aux1->info.data_inicial);
      //percorre o vetor até encontrar o -1 que simboliza que não há mais aprametros
      while(dias_da_semana[p] != -1)
      {
        //se o dia da semana selecionado nos parametros corresponder com o dia da viagem mete flag_dia a 1
        if(dias_da_semana[p] == dia)
          flag_dia = 1;
        p++;
      }
      //se a flag_dia estiver a 0 quer dizer que não está dentro dos aprametros
      if(flag_dia == 0)
      {
        apagar_no(&aux1, &head);
        flag = 1;
      }
    }
    //se a flag estiver a 1 o aux já está no próximo
    if(flag != 1)
      aux1 = aux1->next;

  }
  //return da cabeça da lista
  return head;
}

/**
 * leitura_v: função que lê ficheiro de viagens e mete na lista
 * \param ficheiro string com o nome do ficheiro
 * \return ponteiro para a cabeça da lista de viagens
 */
viagens_node *leitura_v(char ficheiro[], estacoes_node *head_e)
{
  FILE *ftrip = NULL;//ficheiro da viagens
  char ch[MAX_STRING] = {0};
  //estrutura usada para passar os argumentos para inserir na lista
  viagens auxiliar;
  //cabeça da lista
  viagens_node *head = NULL;
  //fim da lista
  viagens_node *tail;


  //Abrir ficheiro e verificar se foi bem aberto
  ftrip = fopen(ficheiro , "r");
  if (ftrip == NULL)
  {
	printf("Impossível ler ficheiros\n");
	exit(EXIT_FAILURE);
  }

  //Ler o ficheiro linha a linha até ao final
	while( fgets(ch,MAX_STRING, ftrip) != NULL)
  	{
      //verifica se a linha tem dados corretos
		  if(separar_parametros(&auxiliar,ch, head_e) != -1)
		    head = insert_ordered_trips(head, auxiliar, &tail); //insere esse nó na tail
	}
	fclose(ftrip);
	return head;
}


/**
 * separar_parametros: função que seprar os parametros de uma linha do ficheiro das viagens
 * \param auxiliar estrutura com informação da viagem
 * \param ch string coma  informação para ser separada
 * \return -1 se algum erro com a leitura e 0 se correu tudo bem
 */
int separar_parametros(viagens *auxiliar, char *ch, estacoes_node *head_e)
{
	//string usada para dividir a linha
  	char *token;
	//dividir a linha pelos caracteres ",:/ " e atribuir a cada campo o seu valor
	token = strtok(ch,",:/ ");
	auxiliar->id_viagem = atoi(token);
	token = strtok(NULL,",:/ ");
	auxiliar->duracao = atoi(token);
  //verifica se a viagem tem tempo negativo
  if(auxiliar->duracao <0)
    return -1;
	token = strtok(NULL,",:/ ");
	auxiliar->data_inicial.mes = atoi(token);
	token = strtok(NULL,",:/ ");
	auxiliar->data_inicial.dia = atoi(token);
	token = strtok(NULL,",:/ ");
	auxiliar->data_inicial.ano = atoi(token);
	token = strtok(NULL,",:/ ");
	auxiliar->tempo_inicial.hora = atoi(token);
	token = strtok(NULL,",:/ ");
	auxiliar->tempo_inicial.minuto = atoi(token);
	token = strtok(NULL,",:/ ");
	token = strtok(NULL,",:/ ");
	auxiliar->id_inicial = procurar(head_e, atoi(token));
	token = strtok(NULL,",:/ ");
	auxiliar->data_final.mes = atoi(token);
	token = strtok(NULL,",:/ ");
	auxiliar->data_final.dia = atoi(token);
	token = strtok(NULL,",:/ ");
	auxiliar->data_final.ano = atoi(token);
	token = strtok(NULL,",:/ ");
	auxiliar->tempo_final.hora = atoi(token);
	token = strtok(NULL,",:/ ");
	auxiliar->tempo_final.minuto = atoi(token);
	token = strtok(NULL,",:/ ");
	token = strtok(NULL,",:/ ");
	auxiliar->id_final = procurar(head_e, atoi(token));
	token = strtok(NULL,",:/ ");
  //verifica se a linha tem id da bike
  if(strcmp(token,"Casual") == 0 || strcmp(token,"Registered") == 0)
    return -1;
	auxiliar->id_bike.letra_st = token[0];
	auxiliar->id_bike.numero_st = atoi(token+1);
	token = strtok(NULL,",:/ ");
  if (token == NULL)
    return -1;
	if(strcmp(token, "Registered") == 0)
	{
		//1 para registado
		auxiliar->usuario = 1;
		token = strtok(NULL,",:/ ");
		auxiliar->ano_nascimento = atoi(token);
		token = strtok(NULL,",:/ ");
    if (token == NULL)
      return -1;
		if(strcmp(token, "Male\r\n") == 0)
		{
			//0 para masculino
			auxiliar->genero = 0;
		}else
		{
		  	//1 para femenino
		  	auxiliar->genero = 1;
		}
	}else
	{
		//0 para casual
		auxiliar->usuario = 0;
		//não é registado
		auxiliar->ano_nascimento = 0;
		//não é registado
		auxiliar->genero = -1;
	}
  //a linha não tem erros nos parametros
  return 0;
}

/**
 * GetNewNode_trips: função que alloca memoria para um nó tipo viagens_node
 * \param auxiliar estrutura com informação da viagem
 * \return ponteiro do nó criado
 */
viagens_node * GetNewNode_trips (viagens auxiliar)
{
	viagens_node *newNode = NULL;

	//allocar memória para o nó e verifica se foi bem alocada
	newNode = (viagens_node *)malloc(sizeof(viagens_node));
	if (newNode == NULL)
	{
		printf("Memory was NOT allocated. \n");
		exit(EXIT_FAILURE);
	}

	//preencher o novo nó coma  informação
	newNode->info = auxiliar;
	newNode->next = NULL;
  newNode->prev = NULL;

	//retorna o nó criado
	return newNode;
}

/**
 * insert_tail_trips: função que insere um nó ordenadamente(id da viagem) na lista
 * \param head cabeça da lista
 * \param auxiliar estrutura com informação da viagem
 * \param tail ponteiro para o último elemento da lista
 * \return da cabeça da lista
 */
viagens_node* insert_ordered_trips( viagens_node *head, viagens auxiliar, viagens_node **tail)
{
	viagens_node *current;
  //novo nó
	current = GetNewNode_trips(auxiliar);
  viagens_node *aux;
  aux = head;

	//verifica se a lista está vazia
	if (head == NULL)
	{
    //meter a tail e a head(retornando o nó) a apontar para o nó criado
    (*tail) = current;
		return current;
	}
  //inserir na cabeça
  if(head->info.id_viagem > current->info.id_viagem)
  {
    //current passa  a ser a nova cabeça
    head->prev = current;
    current->next = head;
    return current;
  }else if ((*tail)->info.id_viagem < current->info.id_viagem) //inserir na cauda
  {
    //meter na cauda o novo nó
  	(*tail)->next = current;
    //ligar o novo nó ao anterio
    current->prev = *tail;
    //atualizar o ponteiro apra o último elemento
    *tail = current;
    //return head por não ter sido alterada
  	return head;
  }
  while(aux!=NULL)
  {
    if(aux->info.id_viagem < current->info.id_viagem && aux->next->info.id_viagem> current->info.id_viagem)
    {
      //inserir no meio da lista
      aux->next->prev = current;
      current->next = aux->next;
      aux->next = current;
      current->prev = aux;
      return head;
    }
    aux = aux->next;
  }
  return head;
}

/**
 * leitura_e: função que lê ficheiro de estações e mete na lista
 * \param ficheiro string com o nome do ficheiro
 * \return ponteiro para a cabeça da lista de estações
 */
estacoes_node *leitura_e(char ficheiro[])
{
  FILE *fstation = NULL;//ficheiro da viagens
  char ch[MAX_STRING] = {0};
  //estrutura usada para passar os argumentos para inserir na lista
  estacoes aux;
  //cabeça da lista
  estacoes_node *head = NULL;
  //tail da lista
  estacoes_node *tail;
  //string usada para dividir a linha
  char *token;

  //Abrir ficheiro e verificar se foi bem aberto
  fstation = fopen(ficheiro , "r");
  if (fstation == NULL)
  {
	printf("Impossível ler ficheiros\n");
	exit(EXIT_FAILURE);
  }
  //Ler o ficheiro linha a linha até ao final
  while( fgets(ch,MAX_STRING, fstation) != NULL)
  {
	  //dividir a linha pelos caracteres "," e atribuir a cada campo o seu valor
	  token = strtok(ch,",");
	  aux.id_estacao = atoi(token);
	  token = strtok(NULL,",");
	  aux.cod_estacao.letra_st = token[0];
	  aux.cod_estacao.numero_st = atoi(token+1);
	  token = strtok(NULL,",");
	  strcpy(aux.nome_estacao, token);
	  token = strtok(NULL,",");
	  strcpy(aux.municipio_estacao, token);
	  token = strtok(NULL,",");
    aux.lat_estacao = atof(token);
	  token = strtok(NULL,",");
	  aux.lon_estacao = atof(token);
	  token = strtok(NULL,",");
	  strcpy(aux.status, token);
	  head = insert_tail_stations(head, aux, &tail);
  }
	fclose(fstation);
	return head;

}

/**
 * GetNewNode_stations: função que alloca memoria para um nó tipo estacoes_node
 * \param auxiliar estrutura com informação da estação
 * \return ponteiro do nó criado
 */
estacoes_node * GetNewNode_stations (estacoes auxiliar)
{
	estacoes_node *newNode = NULL;

	//alocar memória para o novo nó e verificar se foi bem alocada
	newNode = (estacoes_node *)malloc(sizeof(estacoes_node));
	if (newNode == NULL)
	{
		printf("Memory was NOT allocated. \n");
		exit(EXIT_FAILURE);
	}

	//preencher o nó com a informação
	newNode->info = auxiliar;
	newNode->next = NULL;
  newNode->prev = NULL;

	//retornar o nó criado
	return newNode;
}

/**
 * insert_tail_stations: função que insere um nó na cauda da lista
 * \param head cabeça da lista
 * \param auxiliar estrutura com informação da estação
 * \param tail ponteiro para o último elemento da lista
 * \return da cabeça da lista
 */
estacoes_node* insert_tail_stations( estacoes_node *head, estacoes auxiliar, estacoes_node **tail)
{
	estacoes_node *current;
  //criar novo nó
	current = GetNewNode_stations(auxiliar);

	//verifica se a lista está vazia
	if (head == NULL)
	{
    //mete a taila a pontar para o nó criado e retorna o nó criado(head)
    *tail = current;
		return current;
	}
  //insere o nó criado na cauda
	(*tail)->next = current;
  //liga o nó criado ao nó anterior
  current->prev = *tail;
  //atualizar tail para o último elemento da lista
  *tail = current;
	return head;
}

/**
 * apagar_no: apaga um nó da lista
  * \param head ponteiro para a cabeça da lista de viagens
  * \param aux ponteiro para o nó a eliminar
 */
void apagar_no(viagens_node **aux, viagens_node **head)
{
  viagens_node *aux2 = NULL;
  //verifica se não está vazia
  if(*aux != NULL)
  {
    //verifica se é a head a ser eliminada
    if((*aux)->prev == NULL && (*aux)->next != NULL)
    {
      //ponteiro auxiliar para dar free
      aux2 = *aux;
      //atualizar o aux
      *aux = (*aux)->next;
      //atualizar o prev do aux
      (*aux)->prev = NULL;
      //atualizar a head
      *head = *aux;
      free(aux2);
      return;
    }
    //verifica se é a tail a ser eliminada
    if((*aux)->next == NULL && (*aux)->prev != NULL)
    {
      //ponteiro auxiliar para dar free
      aux2 = *aux;
      //andar para tras com o aux
      *aux = (*aux)->prev; //push a head a apontar para o proximo elemento
      (*aux)->next = NULL;
      free(aux2); //Faz free da memoria
      return;
    }
    //lista com um só elemento
    if((*aux)->prev == NULL && (*aux)->next == NULL)
    {
      aux2 = *aux;
      *aux = NULL;
      *head = NULL;
      free(aux2);
      return;
    }
    //para o meio
    aux2 = *aux;
    *aux = (*aux)->prev;
    (*aux)->next =(*aux)->next->next;
    (*aux)->next->prev = *aux;
    *aux = (*aux)->next;
    free(aux2);
  }
}

/**
 * free_estacoes: dar free à lista
 * \param head cabeça da lista
 */
void free_estacoes(estacoes_node *lista_estacoes)
{
  estacoes_node *aux1 = NULL; //usamos um auxiliar

  while (lista_estacoes != NULL)
	{
    //auxiliar para dar free
	  aux1 = lista_estacoes;
    //avança para o pŕoximo
	  lista_estacoes = lista_estacoes->next;
	  free(aux1); //Faz free da memoria
	}
}

/**
 * free_viagens: dar free à lista
 * \param head cabeça da lista
 */
void free_viagens(viagens_node *head)
{
  viagens_node * aux2 = NULL;
  //percorrer a lista toda
  while (head != NULL)
  {
    //usar um ponteiro auxiliar para dar free
    aux2 = head;
    //meter head no proximo nó
    head = head->next;
    //free ao elemento
    free(aux2);
  }
}
