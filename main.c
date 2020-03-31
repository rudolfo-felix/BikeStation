#include"main.h"
/*****************************************************************

BIKESTATION
Jorge Simões 87047
Rudolfo Felix 87114
25/05/2017

*****************************************************************/

int main(int argc, char *argv[])
{
  if (argc != 4)
  {
	printf("Possui argumentos a menos ou a mais\n");
	exit(EXIT_FAILURE);
  }

  viagens_node *lista_viagens = NULL;
  estacoes_node  *lista_estacoes = NULL;


  if(strcmp("-g",argv[1]) == 0)
  {
    //modo gráfico
    //ler listas
    lista_estacoes = leitura_e(argv[3]);
    lista_viagens = leitura_v(argv[2], lista_estacoes);
    main_graphic(&lista_viagens,&lista_estacoes, argv);
  }
  else if(strcmp("-t",argv[1]) == 0)
  {
    //modo textual
    //ler listas
    lista_estacoes = leitura_e(argv[3]);
    lista_viagens = leitura_v(argv[2], lista_estacoes);
    MainMenu(&lista_viagens,&lista_estacoes, argv);

  }else
  {
    printf("Digite -g para modo gráfico e -t para modo textual como segundo argumento\n");
    exit(EXIT_FAILURE);
  }
  //free das listas
  free_viagens(lista_viagens);
  free_estacoes(lista_estacoes);
}
