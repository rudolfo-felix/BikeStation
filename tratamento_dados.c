#include"tratamento_dados.h"

/**
 * MainMenu: menu inicial do modo textual
 * \param lista_viagens ponteiro para a cabeça da lista de viagens
 * \param lista_estacoes ponteiro para a cabeça da lista de estações
 * \param argv vetor de strings com os argumentos
 */
void MainMenu(viagens_node **lista_viagens, estacoes_node **lista_estacoes, char *argv[])
{
  int quit = 0, escolha_menu = 0;
  tempo_i_f intervalo_tempo;
  int **rotas;

  //inicializar parametros a -1
  intervalo_tempo.hora_i = -1;
  intervalo_tempo.hora_f = -1;
  int dia_da_semana[TOTAL_DIAS], duracao = -1;
  for (int i=0;i<TOTAL_DIAS;i++) {
    dia_da_semana[i] = -1;
  }

  while(quit == 0)
  {
    //escolha de que sub-menu aceder
    escolha_menu = menu();
    switch (escolha_menu) {
      case 1:
        //free à lista que estava lida
        free_viagens(*lista_viagens);
        //ler lista novamente
        *lista_viagens = leitura_v(argv[2], *lista_estacoes);
        //selecionar parametros
        selecao_de_dados(&intervalo_tempo, dia_da_semana, &duracao);
        //remover viagens fora dos parametros
        *lista_viagens = selecionar(*lista_viagens, intervalo_tempo, duracao, dia_da_semana);
      break;
      case 2:
        //imprimir as viagens
        imprimir_viagens(*lista_viagens);
      break;
      case 3:
        //imprimir as estações com maximos, minimso e médias
        imprimir_estacoes(*lista_viagens, intervalo_tempo,*lista_estacoes);
      break;
      case 4:
        //imprimir rotas
        rotas = imprimir_rotas(*lista_viagens,*lista_estacoes);
        free_rotas(rotas, *lista_estacoes);
      break;
      case 5:
        //aceder sub-menu das estastísticas
        estatisticas(*lista_estacoes,*lista_viagens);
      break;
      case 6:
        //sair
        quit = 1;
      break;
    }
  }
}

/**
 * menu: imprime menu e espera qual aceder
 * \return  retorna o valor do menu
 */
int menu()
{
  int escolha_menu;
  char menu[MAX_STRING];
  int p;
  //limpa janela
  system("clear");
  do {
    printf("1. Seleção de dados\n");
    printf("2. Listagem de viagens\n");
    printf("3. Listagem de estações\n");
    printf("4. Listagem de rota\n");
    printf("5. Listagem de estatísticas\n");
    printf("6. Sair\n");
    fgets(menu, MAX_STRING, stdin);
    p = sscanf(menu,"%d",&escolha_menu);
    //enquanto o que for digitado for diferente de um inteiro e fora da gama de valores pede novamente
  } while(p!=1 || escolha_menu > 7 ||escolha_menu < 1);
  //limpa a janela
  system("clear");
  return escolha_menu;
}

/**
 * selecao_de_dados: ler parametros
 * \param intervalo_tempo estrutura com hora inicial e final
 * \param dias_da_semana vetor com 7 posições usado para dias da semna
 * \param duracao duração máxima da viagem
 */
void selecao_de_dados(tempo_i_f *intervalo_tempo,int (dias_da_semana)[],int *duracao)
{
  int p;
  char menu[MAX_STRING];
  int escolha_menu;
  int num_dias;
  int quit = 0;
  while(quit == 0)
  {
    system("clear");
    do {
      printf("1. Periodo do tempo:\n");
      printf("2. Dia da Semana\n");
      printf("3. Duração\n");
      printf("4. Limpar critérios\n");
      printf("5. Volta atrás\n");
      fgets(menu, MAX_STRING, stdin);
      p = sscanf(menu,"%d",&escolha_menu);
    } while(p!=1 || escolha_menu > 5 ||escolha_menu < 1);
    system("clear");
    switch (escolha_menu)
    {
      case 1:
        //selecionar intervalo de tempo
        intervalo_tempo = escolher_horario(intervalo_tempo);
      break;
      case 2:
        //selecionar numero de dias da semana
        num_dias = escolhernumor_dias();
        //imprimir a lista da semana
        semana();
        //ler dias da semana
        for (int i = 0; i < num_dias; i++) {
          (dias_da_semana)[i] = escolher_dia(i);
        }
      break;
      case 3:
        //escolher duração máxima da viagem
        escolher_duracao(&duracao);
      break;
      case 4:
        //meter todos os parametros a -1 que significa limpar
        (*intervalo_tempo).hora_i = -1;
        intervalo_tempo->hora_f = -1;
        for (int i=0;i<TOTAL_DIAS;i++) {
          (dias_da_semana)[i] = -1;
        }
        *duracao = -1;
      break;
      case 5:
        //sair deste sub-menu
        quit = 1;
      break;
    }
  }
}

/**
 * escolher_horario: ler intervalo de horas
 * \param intervalo_tempo estrutura com hora inicial e final
 * \return estrutura com hora inicial e final
 */
tempo_i_f *escolher_horario(tempo_i_f *intervalo_tempo)
{
  int p,a,c;
  char str[MAX_STRING]="",str1[MAX_STRING]="";
  //escolha da hora inicial
  do {
    printf("Digite hora inicial: ");
    fgets(str,MAX_STRING,stdin);
    p = sscanf(str, "%d",&a);
  } while(p!=1 || a >24 || a<0);
  //escolha da hora final
  do {
    printf("Digite hora final: ");
    fgets(str1,MAX_STRING,stdin);
    p = sscanf(str1, "%d",&c);
  } while(p!=1 || c >24 || c<0 || c < a);

  intervalo_tempo->hora_i = a;
  intervalo_tempo->hora_f = c;
  //retornar estrutura com hora inicial e final
  return intervalo_tempo;
}

/**
 * escolher_dia: ler dia da semana
 * \param i o numero do dia a ser escolhido
 * \return retorna o valor do dia da semana escolhido
 */
int escolher_dia(int i)
{
  int p;
  int n;
  char ch[MAX_STRING];
  //escolher um dia da semna 0 - 6
  do {
    printf("Escolha o %dº dia\n", i+1);
    fgets(ch,MAX_STRING,stdin);
    p = sscanf(ch,"%d", &n);
  } while( p!=1 || n<0 ||n > 6 );
  return n;
}

/**
 * escolhernumor_dias: ler numeros de dias da semna a ser escolhidos
 * \return retorna o valor de dias a escolher
 */
int escolhernumor_dias()
{
  int p;
  int n;
  char ch[MAX_STRING];
  //ler quantos dias da semana seecionar
  do {
    printf("Quantos dias da semana pretende selecionar: ");
    fgets(ch,MAX_STRING,stdin);
    p = sscanf(ch,"%d", &n);
  } while( p!=1 || n<1 ||n > 7 );
  return n;
}

/**
 * semana: imprimir a semna
 */
void semana()
{
  printf("0. Domingo\n");
  printf("1. Segunda-feira\n");
  printf("2. Terça-feira\n");
  printf("3. Quarta-feira\n");
  printf("4. Quinta-feira\n");
  printf("5. Sexta-feira\n");
  printf("6. Sábado\n");
}

/**
 * escolher_duracao: ler duração máxima da viagem em segundos
  * \param duracao duração das viagens máxima
 */
void escolher_duracao(int **duracao)
{
  int p;
  int n;
  char ch[MAX_STRING];
  //ler a duração
  do {
    printf("Duracao da viagem em segundos: ");
    fgets(ch,MAX_STRING,stdin);
    p = sscanf(ch,"%d", &n);
  } while( p!=1 || n<1 );
  **duracao = n;
}

/**
 * imprimir_viagens: imprimir a viagens por página
  * \param head ponteiro para a cabeça da lista
 */
void imprimir_viagens(viagens_node *head)
{
  int p= 0, n= 0,j = 0, flag_sair = 0;
  char ch[MAX_STRING];
  //ler numero de viagens por página
  do {
    printf("Numero de viagens por página: ");
    fgets(ch,MAX_STRING,stdin);
    p = sscanf(ch,"%d", &n);
  } while( p!=1 || n<1 );
  //verificar se a lista está vazia
  if(head == NULL)
  {
      printf("Nenhuma viagem está dentro dos parametros\n");
      //esperar clicar n para prosseguir
      wait();
      return;
  }
  //percorrer a lista toda
  while (head!=NULL)
  {
    //imprimir n(viagens por páginas) viagens
    for(int i = 0; i<n && head!=NULL;i++)
    {
      j++;
      //imprimir este nó no ecra
      print_trips(head, j);
      //avançar para o próximo nó
      head = head->next;
    }
    //esperar decisão do utilizador
    flag_sair = wait_or_leave();
    if(flag_sair == 1)
      return;
    //limpar ecra
    system("clear");
  }
}

/**
 * print_trips: imprimir uma viagem no ecra
  * \param no nó da lista a ser impresso
  * \param i numero da viagem
 */
void print_trips(viagens_node *no, int i)
{
  printf("Viagem %d:\n", i);
  printf("ID da viagem: %d\n", no->info.id_viagem);
  printf("Duranção da viagem em segundos: %d\n", no->info.duracao);
  printf("Inicio: %d/%d/%d %d:%d:00\n", no->info.data_inicial.dia, no->info.data_inicial.mes, no->info.data_inicial.ano, no->info.tempo_inicial.hora,no->info.tempo_inicial.minuto);
  printf("Fim: %d/%d/%d %d:%d:00\n", no->info.data_final.dia, no->info.data_final.mes, no->info.data_final.ano, no->info.tempo_final.hora,no->info.tempo_final.minuto);
  printf("Estação inicial: %d Estação final: %d\n", no->info.id_inicial->info.id_estacao,no->info.id_final->info.id_estacao);
  printf("ID bike: %c%.5d\n", no->info.id_bike.letra_st, no->info.id_bike.numero_st);

  //verifica se é registado(usuario == 1)
  if(no->info.usuario == 1)
  {
    printf("Usuário: Registered\n");
    printf("Ano nascimento: %d\n", no->info.ano_nascimento);
    //verifica se é do sexo masculino(genero == 0)
    if(no->info.genero == 0)
    {
      printf("Genero: Masculino\n\n\n");
    }else
    {
      printf("Genero: Feminino\n\n\n");
    }
  }else
  {
    printf("Usuário: Casual\n");
    printf("Ano nascimento: -\n");
    printf("Genero: -\n\n\n");
  }
}

/**
 * converter_para_dia: converte uam data para o dia da semana
  * \param data_c estruta com a data(dia,mes,ano)
  * \return retorna o valor do dia da semana (domigo 0, sábado 6)
 */
int converter_para_dia(data data_c)
{
  //converte uma data pelo método Zeller's Rule
  int k;
  k= data_c.dia;
  int m;
  int c;
  int d;
  int f;
  if(data_c.mes<=2)
  {
    m = data_c.mes + 10;
    d = data_c.ano%100 - 1;
  }else
  {
    m = data_c.mes - 2;
    d = data_c.ano%100;
  }
  c = data_c.ano/100;
  f = k + ((13*m - 1)/5) + d + d/4 + c/4 - 2*c;
  f = f%TOTAL_DIAS;
  if(f<0)
    return 7+f;
  return f;
}

/**
 * print_stations: imprime no ecra informações de uma estação
 * \param no nó da lista de estações para ser printado
 */
void print_stations(estacoes_node *no)
{
  printf("ID da estação: %d\n", no->info.id_estacao);
  printf("Código da estação: %c%d\n", no->info.cod_estacao.letra_st, no->info.cod_estacao.numero_st);
  printf("Nome: %s, %s\n",no->info.nome_estacao, no->info.municipio_estacao);
  printf("Latitude: %f  Longitude: %f\n", no->info.lat_estacao, no->info.lon_estacao);
}

/**
 * imprimir_estacoes:imprmir max,min e média de bikes as sair e entrar por cada estação
 * \param head_v ponteiro para a cabeça da lista de viagens
 * \param head_e ponteiro para a cabeça da lista de estações
 * \param intervalo_tempo intervalo de tempo
 */
void imprimir_estacoes( viagens_node *head_v, tempo_i_f intervalo_tempo, estacoes_node *head_e)
{
  int sair[MAX_HORAS] = {0};
  int entrar[MAX_HORAS] = {0};
  viagens_node *aux_v;
  estacoes_node *aux_e;
  aux_v = head_v;
  aux_e = head_e;

  //percorrer lista de estações
  while (aux_e != NULL)
  {
    //imprimir informações desta estação
    print_stations(aux_e);
    aux_v = head_v;
    //percorrer lista de viagens
    while (aux_v != NULL)
    {
      //bicicletas a sair
      if(aux_v->info.id_inicial->info.id_estacao == aux_e->info.id_estacao)
        sair[aux_v->info.tempo_inicial.hora]++;
      //bicicletas a entrar
      if(aux_v->info.id_final->info.id_estacao == aux_e->info.id_estacao)
        entrar[aux_v->info.tempo_inicial.hora]++;
      aux_v = aux_v->next;
    }
    //imprmir máximos minimos e médias
    imprimir_max_min(sair,entrar,intervalo_tempo);
    //meter tudo a 0's
    for(int i = 0; i <MAX_HORAS;i++) {
      sair[i] =0;
      entrar[i] = 0;
    }
    aux_e = aux_e->next;
  }
  wait();
}

/**
 * imprimir_max_min:imprmir max,min e média de bikes as sair e entrar
 * \param sair vetor com numero de bicicletas que sairam
 * \param entrar vetor com numero de bicicletas que entraram
 * \param intervalo_tempo intervalo de tempo
 * \return retorna a soma
 */
void imprimir_max_min(int sair[], int entrar[], tempo_i_f intervalo_tempo)
{
  int hora_inicial = intervalo_tempo.hora_i;
  int hora_final = intervalo_tempo.hora_f;
  //verifica se o utilizador escolheu intervalo de tempo como parametro
  if(hora_inicial == -1)
  {
    //mete das 0 às 24
    hora_inicial = 0;
    hora_final = 24;
  }
  //soma de viagens
  int soma_sair = somatotal(sair);
  int soma_entrar = somatotal(entrar);
  int max_sair=INT_MIN, max_entrar=INT_MIN, min_sair=INT_MAX, min_entrar=INT_MAX;
  //encontrar mámimos e minimos
  for (int i = hora_inicial; i< hora_final;i++)
  {
    if(sair[i]>max_sair)
      max_sair = sair[i];
    if(sair[i]<min_sair)
      min_sair = sair[i];
    if(entrar[i]>max_entrar)
      max_entrar = entrar[i];
    if(entrar[i]<min_entrar)
      min_entrar = entrar[i];
  }
  //imprimir informação
  printf("No horário das %d até %d:\n",hora_inicial, hora_final);
  printf("A sair : MAX -> %d MIN -> %d MEDIA -> %.2f\n", max_sair,min_sair,(float)soma_sair/(hora_final- hora_inicial));
  printf("A entrar : MAX -> %d MIN -> %d MEDIA -> %.2f\n\n\n", max_entrar,min_entrar,(float)soma_entrar/(hora_final- hora_inicial));
}

/**
 * somatotal:soma 24 posições consecutivas do vetor
 * \param vetor vetor
 * \return retorna a soma
 */
int somatotal(int vetor[])
{
  int some = 0;
  for(int i = 0; i<MAX_HORAS;i++)
    some  = some + vetor[i];
  return some;
}

/**
 * wait: função que espera usuário cicar s
 */
void wait()
{
  int p;
  char str[MAX_STRING]="", ch;
  do {
    printf("Digite 's' para proseguir ");
    fgets(str,MAX_STRING,stdin);
    p = sscanf(str, "%c",&ch);
  } while(p!=1 || ch != 's');
}

/**
 * print_stations: imprime no ecra informações de uma estação
 * \param no nó da lista de estações para ser printado
 * \return retorna o vetor bidimensional para ser dado free depois
 */
int  **imprimir_rotas(viagens_node *head_v, estacoes_node *head_e)
{
  int estacao_escolhida, n= 0,j = 0,flag_sair = 0,**rotas, flag_n_rotas_2 = 0, flag_n_rotas = 0,numero_estacoes;
  viagens_node *aux = NULL;
  aux = head_v;
  estacoes_node *aux1 = NULL, *nome = NULL;
  //escolher estação
  estacao_escolhida = escolher_estacao(head_e);
  //escolher páginas
  n = escolher_pag();
  //contar numero de estações
  numero_estacoes = contar_estacoes(head_e);
  //criar um veto bidimensional com o tamnhano de estações-1
  rotas = (int **)malloc(sizeof(int *) * (numero_estacoes));
  for (int  i = 0; i < numero_estacoes ; i++)
  {
    //cada linha tem 3 colunas
    //correpondendo ao id estação, bikes a sair, bikes a entrar
    rotas[i] = (int *)malloc(sizeof(int) * 3);
  }
  //aux1 a apontar para lista de estações
  aux1 = head_e;
  //inicializar o vetor bidimensional
  while (aux1 != NULL) {
      //guardar id da estação
      rotas[j][0] = aux1->info.id_estacao;
      //rotas a sair da estação
      rotas[j][1] = 0;
      //rotas a entrar na estação
      rotas[j][2] = 0;
      j++;
    aux1 = aux1->next;
  }
  //percorrer a lista de viagens
  while(aux!=NULL)
  {
    //calcular numero de rotas entre estação escolhida e outras e gurdar em rotas[][]
    calcular_n_rotas(aux,rotas,numero_estacoes , estacao_escolhida);
    aux = aux->next;
  }
  //reorganizar para bicletas a sair
  bubble_sort(numero_estacoes,rotas, 1);
  //printa rotas ordenadas decrescentemente bicicletas a sair
  printf("Viagens que têm inicio na estação %d:\n", estacao_escolhida);
  for (int i = 0; i < numero_estacoes; i++)
  {
    flag_n_rotas++;
    nome = procurar(head_e, rotas[i][0]);
    printf("ID FINAL: %d\nNome:%s \nViagens: %d\n\n\n",rotas[i][0],nome->info.nome_estacao,rotas[i][1]);
    if(flag_n_rotas == n)
    {
        flag_sair = wait_or_leave();
        if(flag_sair == 1)
          return rotas;
      flag_n_rotas = 0;
      system("clear");
      printf("Viagens que têm inicio na estação %d:\n", estacao_escolhida);
    }
  }
  //reorganizar para bicletas a entrar
  bubble_sort(numero_estacoes,rotas, 2);
  //print das rotas ordenadas decrescentemente bicletas a entrar
  printf("Viagens que têm fim na estação %d:\n", estacao_escolhida);
  for (int i = 0; i < numero_estacoes; i++)
  {
    flag_n_rotas_2++;
    nome = procurar(head_e, rotas[i][0]);
    printf("ID INICIAL: %d\nNome:%s \nViagens: %d\n\n\n",rotas[i][0],nome->info.nome_estacao, rotas[i][2]);
    if(flag_n_rotas_2 == n)
    {
      flag_sair = wait_or_leave();
      if(flag_sair == 1)
        return rotas;
      flag_n_rotas_2 = 0;
      system("clear");
      printf("Viagens que têm fim na estação %d:\n", estacao_escolhida);
    }
  }
  wait();
  return rotas;
}

/**
 * bubble_sort: reordena um array bidimensional
 * \param numero_estacoes numero de estações para iterar no array
 * \param rotas array
 * \param a decide se é reorganizado pelo 2ª coluna ou pela 3ª
 */
void bubble_sort(int numero_estacoes, int **rotas, int a)
{
  int ajudante1,ajudante2,ajudante3;
  //percorre o vetor
  for (int i = 0; i < numero_estacoes-1; i++)
  {
    //percorre o vetor
    for (int c = 0; c < numero_estacoes-1; c++)
    {
      //verifica se tem de fazer uma troca
      if(rotas[c][a] < rotas[c+1][a])
      {
        ajudante1 = rotas[c][1];
        ajudante2 = rotas[c][2];
        ajudante3 = rotas[c][0];
        rotas[c][1] = rotas[c+1][1];
        rotas[c][2] = rotas[c+1][2];
        rotas[c][0] = rotas[c+1][0];
        rotas[c+1][1] = ajudante1;
        rotas[c+1][2] = ajudante2;
        rotas[c+1][0] = ajudante3;
      }
    }
  }
}

/**
 * escolher_pag: escolhe numero de rotas por página
 * \return retorna o numero de rotas
 */
int escolher_pag()
{
  char ch[MAX_STRING];
  int p,n;
  do {
    printf("Numero de rotas por página: ");
    fgets(ch,MAX_STRING,stdin);
    p = sscanf(ch,"%d", &n);
  } while( p!=1 || n<1 );
  return n;
}

/**
 * calcular_n_rotas: conta o numero de bicicletas a entrar e a sair(incrementando no vetor)
 * \param aux nó da lista de viagens
 * \param rotas array que guarda a informação
 * \param numero_estacoes numero total de estações para iterar
 * \param estacao_escolhida id da estação escolhida incialmente
 */
void calcular_n_rotas(viagens_node *aux, int **rotas, int numero_estacoes , int estacao_escolhida)
{
  int j=0;
    // viagens a sair da estação escolhida
  if(aux->info.id_inicial->info.id_estacao == estacao_escolhida)
  {
    j = 0;
    //encontrar o j
    while (aux->info.id_final->info.id_estacao != rotas[j][0] && j < numero_estacoes -1) {
      j++;
    }
    //j corresponde ao index onde rotas[j][0]== id da estação final
    rotas[j][1]++;
  }
  //viagens a acabar na estação escolhida
  if(aux->info.id_final->info.id_estacao == estacao_escolhida)
  {
    j = 0;
    //encontrar o j
    while (aux->info.id_inicial->info.id_estacao != rotas[j][0] && j < numero_estacoes -1) {
      j++;
    }
    //j corresponde ao index onde rotas[j][0]== id da estação inicial
    rotas[j][2]++;
  }
}

/**
 * free_rotas: dá free do vetor alocado dinamicaente
 * \param rotas array para ser dado free
 * \param head ponteiro para a cabeça da linha de estações
 */
void free_rotas(int **rotas, estacoes_node* head)
{
  estacoes_node *aux;
  aux = head;
  int j = 0;
  free(rotas[0]);
  //percorrer a lista de estações até chegar à última dando free a todas as linhas do vetor
  while (aux!=NULL && aux->info.id_estacao == rotas[j][0]) {
    j++;
    free(rotas[j]);
  }
  free(rotas);
}

/**
 * wait_or_leave: função que espera decisão do utilizador
 * \return 0 se quer continuar, 1 se quer sair
 */
int wait_or_leave()
{
  int p;
  char str[MAX_STRING]="", ch;
  do {
    printf("Digite 'n' para proseguir ou 's' para sair ");
    fgets(str,MAX_STRING,stdin);
    p = sscanf(str, "%c",&ch);
  } while(p!=1 || (ch != 's' && ch != 'n'));
  if (ch == 's') {
    return 1;
  }
  return 0;
}

/**
 * escolher_estacao: pede ao utilizador um id de uma estação válida
 * \param head ponteiro para a cabeça da linha de estações
 * \return retorna um id de estação válido
 */
int escolher_estacao(estacoes_node *head)
{
  int p,a;
  int flag = 0;
  estacoes_node *aux = NULL;
  char str[MAX_STRING]="";
  do {
    aux = head;
    printf("Digite ID da estação a selecionar: ");
    fgets(str,MAX_STRING,stdin);
    p = sscanf(str, "%d",&a);
    //verifica se o id digitado é válido
    while (aux != NULL) {
      if(a == aux->info.id_estacao)
        flag = 1;
      aux = aux->next;
    }
  } while(p!=1 || flag != 1);
  return a;
}

/**
 * contar_estacoes: conta o numero de estações no ficheiro de lista_estacoes
 * \param head ponteiro apra a cabela da lista de estações
 * \return retorna numero de estações
 */
int contar_estacoes(estacoes_node *head)
{
  estacoes_node *aux =NULL;
  aux = head;
  int i = 0;
  //conta as estações
  while (aux != NULL) {
    i++;
    aux = aux->next;
  }
  return i;
}

/**
 * estatisticas_males_females: estatisticas por género
 * \param head_v ponteiro para a cabeça de lista de viagens
 * \param fp ficheiro onde vai ser escrito as estastísticas
 */
void estatisticas_males_females(viagens_node *head_v, FILE * fp)
{
  viagens_node * aux;
  aux = head_v;
  int numero_registados = 0;
  int numero_males = 0;
  int numero_females = 0;

  while(aux!=NULL)
  {
    //verifica se é registado
    if(aux->info.usuario == 1)
    {
      //conta total
      numero_registados++;
      if(aux->info.genero == 0)
      {
        //conta masculinos
        numero_males++;
      }
      else
      {
        //conta femininos
        numero_females++;
      }
    }
    aux = aux->next;
  }

  //imprime no ficheiro os valores das percentagens
  fprintf(fp, "Percentagem de usuários registados masculinos: %.1f %%\n",(float)(numero_males*100)/numero_registados);
  fprintf(fp, "Percentagem de usuários registados femininos: %.1f %%\n\n\n",(float)(numero_females*100)/numero_registados);
  printf("Estatísticas de género impressa no ficheiro estatisticas.txt\n\n");
}

/**
 * estatisticas_idades: estatisticas por idades
 * \param head_v ponteiro para a cabeça de lista de viagens
 * \param fp ficheiro onde vai ser escrito as estastísticas
 */
void estatisticas_idades(viagens_node *head_v, FILE * fp)
{
  viagens_node * aux;
  aux = head_v;
  int numero_registados = 0;
  int idades[MAX_IDADES] = {0};

  //Toda esta estatística é feita só com pessoas com até 80 porque como alguns
  //dados estão mal(mau utilizador) é só considerado até 80 anos

  while(aux!=NULL)
  {
    if(aux->info.usuario == 1)
    {
      //contar numero total
      numero_registados++;
      //verifica se tem menos de 80 anos e incrementa no vetor em que o indice é a idade da pessoa
      if(aux->info.data_inicial.ano - aux->info.ano_nascimento < MAX_IDADES)
        idades[aux->info.data_inicial.ano - aux->info.ano_nascimento]++;
    }
    //anda para a frente na lista
    aux = aux->next;
  }
  fprintf(fp, "Percentagem de usuários por idades:\nIDADES:         PERCENTAGEM\n");

  for (int i = 12; i< MAX_IDADES; i = i+2)
  {
    //ao imprimir no ficheiro calculasse a percentagem por cada 2 intervalos de idades
    //somando posições consecutivas do vetor porque corresponde ao total de viagens
    //feitas por pessoas dessas idades
    fprintf(fp, "%.3d - %.3d         : %.2f %%\n",i,i+1,((float)(idades[i]+idades[i+1])*100)/numero_registados );
  }
  fprintf(fp, "\n\n");
  printf("Estatisticas de idades impressas no ficheiro.\n\n");
}

/**
 * estatisticas_duracao: estatisticas de duracao
 * \param head_v ponteiro para a cabeça de lista de viagens
 * \param fp ficheiro onde vai ser escrito as estastísticas
 */
void estatisticas_duracao(viagens_node *head_v, FILE * fp)
{
  viagens_node * aux;
  aux = head_v;
  int numero_registados = 0;
  //usado um vetor com 6*60*60 de posições, que correponde a 6 horas em segundos
  int duracao_viagens[6*60*60] = {0};

  while(aux!=NULL)
  {
    //verifica se é registado
    if(aux->info.usuario == 1)
    {
      //conta total
      numero_registados++;
      //se a duração for inferior a 6 horas
      //usar como indice a duração da viagem e incrementar
      if(aux->info.duracao< 6*60*60)
        duracao_viagens[aux->info.duracao]++;
    }
    aux = aux->next;
  }

  fprintf(fp, "Percentagem de viagens por tempo:\nTEMPO(minutos):      PERCENTAGEM:\n");

  for (int i = 0; i< 6*60*60; i = i+15*60)
  {
    //foi usado o mesmo método usado nas Estatisticas de idades
    //só que aqui temos de somar 15min de indexes
    //correspondendo ao sumatorio de duracao_viagens[0]até duracao_viagens[15*60]
    //para cada iteração até chegar às 6 horas
    fprintf(fp, "%.3d - %.3d         : %.3f %%\n",i/60,i/60 + 15,(float)(sumatorio(duracao_viagens, i)*100)/numero_registados);
  }

  fprintf(fp, "\n");
  printf("Estatisticas de duração impressas no ficheiro\n\n");
}

/**
 * sumatorio: somar posições de vetor
 * \param idades vetor
 * \param a posição inical da soma
 * \return retorna o valor da soma
 */
int sumatorio(int idades[],int a)
{
  //somar 15*60 posições de um vetor e retornar
  int soma = 0;
  for(int i = a;i < a + 15*60;i++)
    soma = soma + idades[i];
  return soma;
}

/**
 * estatisticas: sub menu das estatisticas
 * \param head_v ponteiro para a cabeça de lista de viagens
 * \param head_e ponteiro para a cabeça de lista de estações
 */
void estatisticas(estacoes_node *head_e, viagens_node *head_v)
{
  int p;
  char menu[MAX_STRING];
  int escolha_menu;
  int quit = 0;
  FILE *fp=NULL;
  //abrir ficheiro
  fp = fopen("estatisticas.txt","w");
  if (fp == NULL)
    exit(EXIT_FAILURE);

  while(quit == 0)
  {
    do {
      printf("1. Estatisticas por género\n");
      printf("2. Estatisticas de idade\n");
      printf("3. Estatísticas da duração\n");
      printf("4. Estatisticas da velocidade\n");
      printf("5. Voltar ao menu\n" );
      fgets(menu, MAX_STRING, stdin);
      p = sscanf(menu,"%d",&escolha_menu);
    } while(p!=1 || escolha_menu > 5 ||escolha_menu < 1);
    system("clear");
    switch (escolha_menu)
    {
      case 1:
        //chama estatisticas por genero
        estatisticas_males_females(head_v,fp);
      break;
      case 2:
        //chama estatisticas por idades
        estatisticas_idades(head_v,fp);
      break;
      case 3:
        //chama estatisticas de duracao
        estatisticas_duracao(head_v,fp);
      break;
      case 4:
        //chama estatisticas de velocidade
        estatisticas_velocidade(head_e,head_v,fp);
      break;
      case 5:
        quit = 1;
      break;
    }
  }
  fclose(fp);
}

/**
 * estatisticas: sub menu das estatisticas
 * \param head_v ponteiro para a cabeça de lista de viagens
 * \param head_e ponteiro para a cabeça de lista de estações
 * \param fp ficheiro onde vai ser escrito as estastísticas
 */
void estatisticas_velocidade(estacoes_node *head_e, viagens_node *head_v, FILE *fp)
{
  viagens_node *aux = head_v;

  long double idades_m[MAX_IDADES][2] = {{0}};
  long double idades_f[MAX_IDADES][2] = {{0}};
  long double velocidades_m, velocidades_f;
  while (aux!=NULL) {
    if(aux->info.usuario == 1)
    {
      //verifica se é registado
      if(aux->info.genero == 0)
      {
        //masculinos
        //é usado a mesma estratégia nas estatisticas de idades
        //só que neste caso temos de armazenar a duração e as velocidades
        if(aux->info.data_inicial.ano - aux->info.ano_nascimento < MAX_IDADES)
        {
          //armazenar duração e velocidades no vetor
          idades_m[aux->info.data_inicial.ano - aux->info.ano_nascimento][0] += velocidade(aux->info.id_inicial->info.lat_estacao, aux->info.id_final->info.lat_estacao, aux->info.id_inicial->info.lon_estacao, aux->info.id_final->info.lon_estacao);
          idades_m[aux->info.data_inicial.ano - aux->info.ano_nascimento][1] += aux->info.duracao;
        }
      }
      else
      {
        //femininos
        if(aux->info.data_inicial.ano - aux->info.ano_nascimento < MAX_IDADES)
        {
          idades_f[aux->info.data_inicial.ano - aux->info.ano_nascimento][0] += velocidade(aux->info.id_inicial->info.lat_estacao, aux->info.id_final->info.lat_estacao, aux->info.id_inicial->info.lon_estacao, aux->info.id_final->info.lon_estacao);
          idades_f[aux->info.data_inicial.ano - aux->info.ano_nascimento][1] += aux->info.duracao;
        }
      }
    }
    aux = aux->next;
  }
  fprintf(fp, "Estatisticas por velocidades\nIDADES:          VELOCIDADES(km/h):\n");

  for (int i = 12; i< MAX_IDADES; i = i+2)
  {
    //verifica se a duração é 0
    if(idades_f[i][1]+ idades_f[i+1][1] != 0)
      velocidades_f = (idades_f[i][0]+idades_f[i+1][0])/(idades_f[i][1]+ idades_f[i+1][1]) * 3.6;
    else
      velocidades_f = 0;
    //verifa se a duração é 0
    if(idades_m[i][1]+ idades_m[i+1][1] != 0)
      velocidades_m = (idades_m[i][0]+idades_m[i+1][0])/(idades_m[i][1]+ idades_m[i+1][1]) * 3.6;
    else
      velocidades_m = 0;
    //imprime no ficheiro com a mesma técnica das estatisticas por idades
    fprintf(fp, "%d - %d         M= %.3Lf     F=%.3Lf\n",i,i+1,velocidades_m,velocidades_f);
  }

  fprintf(fp, "\n\n");
  printf("Estatisticas de velocidade impressas no ficheiro\n\n");
}

/**
 * velocidade: converte latitude+longitude para distancia
 * \param lon1 longitude 1
 * \param lon2 longitude 2
 * \param lat1 latitude 1
 * \param lat1 latitude 1
 * \return retorna o valor da distância
 */
long double velocidade(double lon1, double lon2, double lat1, double lat2)
{
  //por se tratar de distancias dentro de uma cidade(pequenas) usamos um algoritmo simplificado
  lon1 = lon1*PI/180;
  lon2 = lon2*PI/180;
  lat1 = lat1*PI/180;
  lat2 = lat2*PI/180;
  long double lon = lon2 - lon1;
  long double lat = lat2 - lat1;
  long double lat_m = (lat1 + lat2)/2;
  long double x = lon * cos(lat_m);
  long double y = lat;
  return RAIO*(sqrt(x*x + y*y));
}
