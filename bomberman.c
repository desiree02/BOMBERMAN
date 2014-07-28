/*			Bomber Man
			==========

Computação I (2014-1) - CC / UFRRJ. 
Grupo: Desiree Araújo e Fernanda Oliveira  
Professor: Filipe Braida do Carmo


*/

#include <stdio.h>
#include <stdlib.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>
#include <SDL/SDL_mixer.h>
#include <time.h>
#include <string.h>

#define GRAMA  1
#define PAREDE 2
#define MURO   3
#define SAIDA  4
#define VOCE   5
#define BOMBA  6
#define GRASS  7
#define CASA   8

#define LARGURA 760 
#define ALTURA  720
#define BPP       0 




//================================================================================================== 
//--------------------------------------------VARIAVEIS---------------------------------------------
//==================================================================================================
int quit = 0;

SDL_Surface * tela, * piso, * parede, * player,* final, * grama, *bomba,*muro, *temp, *grass,*fogo,*invu,*veloc,*pont,*nada,*pont_inimigo,*vetorBomba[4],*top,*ganhou, *perdeu, *vidinha;

SDL_Surface *telaInicial,*telaRecord, *botao_novo,*botao_instru,*botao_record,*botao_sair;

typedef struct inimigo{

SDL_Rect rect_inimigo;

int Ativo;
int linhaInimigo;
int colunaInimigo;
}Inimigo;
int vida = 3;
int tempoInimigo = 0;

SDL_Rect rectBomba = {0,0};
SDL_Rect rect_box;
SDL_Rect destino;

SDL_Rect rect_tela;
SDL_Rect rect_bNovo;
SDL_Rect rect_bInstru;
SDL_Rect rect_bRecord;
SDL_Rect rect_bSair;

Inimigo inimigo[5];

SDL_Rect destinoExplosao;

int posi = 0;
int pontuacao = 0;
int pontuacao_auxiliar = 0;
int tempoBomba = 0;
int linhaBomba = 0, colunaBomba = 0;
int bombaAtiva = 0;
int coluna_atual = 1, linha_atual = 1, fim = 0;
int jogoAtivo = 0;

//--------------------------------------------MAPA---------------------------------------------------
int mapa[15][19] = {
3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,
3,1,1,1,1,1,1,2,2,1,1,1,1,1,2,1,2,2,3,
3,1,3,2,3,1,3,2,3,1,3,2,3,2,3,1,3,2,3,
3,2,2,1,1,1,2,2,2,1,1,1,2,2,1,1,2,2,3,
3,1,3,2,3,1,3,2,3,1,3,2,3,2,3,1,3,2,3,
3,2,2,2,1,1,1,2,2,1,1,1,2,2,1,1,1,2,3,
3,1,3,2,3,1,3,2,3,1,3,2,3,2,3,1,3,2,3,
3,1,1,1,1,1,2,2,1,2,2,2,1,1,2,2,1,1,3,
3,1,3,2,3,1,3,2,3,1,3,2,3,2,3,1,3,2,3,
3,2,2,1,1,1,2,2,2,1,1,1,2,2,1,1,2,2,3,
3,1,3,2,3,1,3,2,3,1,3,2,3,2,3,1,3,2,3,
3,2,2,1,1,1,2,2,2,1,1,1,2,2,1,1,2,2,3,
3,1,3,2,3,1,3,2,3,1,3,2,3,2,3,1,3,2,3,
3,1,2,2,2,2,1,1,1,1,1,2,2,1,1,1,4,1,3,
3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,
};

int mapaBonus[15][19];

//================================================================================================== 
//---------------------------------DECLARANDO FUNCOES-------------------------------------------------
//==================================================================================================


void funcaoBonus();
void topo();
void controla_fps (int tempo_inicial);
int carrega_imagens ();
void moveInimigos ();
void desenha_mapa (int tempoBomba);
void move_jogador ( SDL_Event event );
void contatoInimigo();
void verificaBonus ();
void escolha_menu( SDL_Event event, SDL_Rect destino);
int menu();
int instrucoes();
int introducao();
int recordes();
int jogo();
int init();
int intro_jogo(int *p_quit);
void reinicia();
void desenha_texto(char* texto, SDL_Surface* dst, int x, int y);
//================================================================================================== 
//-------------------------------------EM NOME DE JESUS---------------------------------------------
//==================================================================================================

#define MAX_ARRAY 10
#define MAX_NOME 50


typedef struct
{
    char nome[MAX_NOME];
    int pontuacao;
} RECORDE;

typedef struct
{
    RECORDE vetor[MAX_ARRAY];
    int n;

} ARRAY;

RECORDE elemento(ARRAY *, int);
int tamanho(ARRAY *);
void insere(ARRAY *, RECORDE);
void remove_posicao(ARRAY *, int);
void imprime(ARRAY *);
void sort(ARRAY *);
void bubble_sort(RECORDE *, int);



  RECORDE rec1;


RECORDE elemento(ARRAY *array, int posicao)
{
	return array->vetor[posicao];
}

int tamanho(ARRAY *array)
{
	return array->n;
}

void insere(ARRAY *array, RECORDE recorde)
{
    if(tamanho(array) < MAX_ARRAY)
    {
        array->vetor[tamanho(array)] = recorde;
        array->n += 1;
    }
    else
    {
        RECORDE menor_recorde;
        menor_recorde = elemento(array, MAX_ARRAY - 1);
        
        if(menor_recorde.pontuacao >= recorde.pontuacao)
        {
            return;
        }
        else
        {
            remove_posicao(array, MAX_ARRAY - 1);
            insere(array, recorde);
            return;
        }
    }
    
    sort(array);
}

void remove_posicao(ARRAY *array, int posicao)
{
	if(tamanho(array) == 0)
	{
		return;
	}
	else if(tamanho(array) == 1)
	{
		array->n -= 1;
		return;
	}
	else
	{
		array->n -= 1;
		array->vetor[posicao] = array->vetor[tamanho(array)];
	}
    
    sort(array);
}

void imprime(ARRAY *array)
{
	int i;
    RECORDE recorde;

    printf("------------------------------\n");
	printf("Recordes:\n");

	for(i = 0; i < tamanho(array); i++)
	{
        recorde = elemento(array, i);
        printf("%d - %d :: %s \n", i+1, recorde.pontuacao, recorde.nome);
	}

	printf("------------------------------\n");
}

void sort(ARRAY *array)
{
	bubble_sort(array->vetor, array->n);
}

void bubble_sort(RECORDE list[], int n)
{
	int i, j;
    RECORDE swap;

	for(i = 0 ; i < ( n - 1 ); i++)
	{
		for(j = 0 ; j < n - i - 1; j++)
		{
			if(list[j].pontuacao < list[j+1].pontuacao)
			{ 
				swap = list[j];
				list[j] = list[j+1];
				list[j+1] = swap;
			}
		}
	}
}








void reinicia()
{
	int mapa[15][19] = {	3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,
				3,1,1,1,1,1,1,2,2,1,1,1,1,1,2,1,2,2,3,
				3,1,3,2,3,1,3,2,3,1,3,2,3,2,3,1,3,2,3,
				3,2,2,1,1,1,2,2,2,1,1,1,2,2,1,1,2,2,3,
				3,1,3,2,3,1,3,2,3,1,3,2,3,2,3,1,3,2,3,
				3,2,2,2,1,1,1,2,2,1,1,1,2,2,1,1,1,2,3,
				3,1,3,2,3,1,3,2,3,1,3,2,3,2,3,1,3,2,3,
				3,1,1,1,1,1,2,2,1,2,2,2,1,1,2,2,1,1,3,
				3,1,3,2,3,1,3,2,3,1,3,2,3,2,3,1,3,2,3,
				3,2,2,1,1,1,2,2,2,1,1,1,2,2,1,1,2,2,3,
				3,1,3,2,3,1,3,2,3,1,3,2,3,2,3,1,3,2,3,
				3,2,2,1,1,1,2,2,2,1,1,1,2,2,1,1,2,2,3,
				3,1,3,2,3,1,3,2,3,1,3,2,3,2,3,1,3,2,3,
				3,1,2,2,2,2,1,1,1,1,1,2,2,1,1,1,4,1,3,
				3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,
				};
	quit = 1;
	posi = 0;
	pontuacao = 0;
	pontuacao_auxiliar = 0;
	tempoBomba = 0;
	linhaBomba = 0, colunaBomba = 0;
	bombaAtiva = 0;
	coluna_atual = 1, linha_atual = 1, fim = 0;
	jogoAtivo = 0;
	vida = 3;
	tempoInimigo = 0;
	SDL_Rect rectBomba = {0,0};
	SDL_FreeSurface(muro);
	SDL_FreeSurface(parede);
	SDL_FreeSurface(grass);
	SDL_FreeSurface(tela);
	SDL_FreeSurface(piso);
	SDL_FreeSurface(grama);
	

	carrega_imagens ();
	moveInimigos ();
	desenha_mapa (tempoBomba);
}

    












//Inicialização e tratamento de erros
int TTF_Init();
void TTF_Quit();
char *TTF_GetError();
TTF_Font *TTF_OpenFont(const char *file, int ptsize);
void TTF_CloseFont(TTF_Font *font);
//int TTF_GetFontStyle(TTF_Font *font);
void TTF_SetFontStyle(TTF_Font *font, int style);
TTF_Font* font;

#define FONTSIZE 18

SDL_Surface *TTF_RenderText_Solid(TTF_Font *font, const char *text, SDL_Color fg);
SDL_Surface *TTF_RenderText_Shaded(TTF_Font *font, const char *text, SDL_Color fg, SDL_Color bg);
SDL_Surface *TTF_RenderText_Blended(TTF_Font *font, const char *text, SDL_Color fg);

int pontos = 0;
char pontos_str[11] = "0";
char pontsrec[100];






void reverse (char string[])
{
	int i, j;
	char aux;
	for (i = 0, j = strlen(string)-1; i<j; i++, j--)
	{
 		aux = string[i]; 
 		string[i] = string[j];
 		string[j] = aux;
	}
}


void itoa(int numero, char string[])
{
	int i, sign; 

	if ((sign = numero) < 0) 
		numero = -numero;
	
	i = 0; 
	
	do
	{ 
		string[i++] = numero % 10 + '0';
    	}while ((numero /= 10) > 0);

	if (sign < 0)
		string[i] = '-';
	
	string[i] = '\0';
	reverse(string);
}


void atualizaPontos(pontuacao)
{

	itoa(pontuacao,pontos_str);
}











void desenha_texto(char* texto, SDL_Surface* dst, int x, int y)
{
  SDL_Color cor = {255, 255, 255}; //vermelho
  SDL_Surface* src = TTF_RenderText_Blended(font, texto, cor);
  SDL_Rect dst_rect = {x, y, 0, 0};
  
  SDL_BlitSurface(src, NULL, dst, &dst_rect);
  
  SDL_FreeSurface(src);
}



void clean_up()
{
    //Free the surfaces
    //SDL_FreeSurface( message );

    //Close the font that was used
   // TTF_CloseFont( font );

    //Quit SDL_ttf
    //TTF_Quit();
}

//--------------------------------------------TOPO---------------------------------------------------


void topo()
{
	SDL_Rect destino;
	destino.y = 0;
	destino.x = 0;
	SDL_BlitSurface(top, NULL, tela, &destino);

	font = TTF_OpenFont("cool.ttf", FONTSIZE);
	//SDL_Flip(tela);

	SDL_Rect alvoVida[3];

	alvoVida[0].y = 40;
	alvoVida[0].x = 40;


	alvoVida[1].y = 40;
	alvoVida[1].x = 100;


	alvoVida[2].y = 40;
	alvoVida[2].x = 160;

	if(vida == 3)
	{
		SDL_BlitSurface(vidinha, NULL, tela, &alvoVida[0]);
		SDL_BlitSurface(vidinha, NULL, tela, &alvoVida[1]);
		SDL_BlitSurface(vidinha, NULL, tela, &alvoVida[2]);
	}

	if(vida == 2)
	{
		SDL_BlitSurface(vidinha, NULL, tela, &alvoVida[0]);
		SDL_BlitSurface(vidinha, NULL, tela, &alvoVida[1]);
		SDL_BlitSurface(NULL, NULL, tela, &alvoVida[2]);
	}

	if(vida == 1)
	{
		SDL_BlitSurface(vidinha, NULL, tela, &alvoVida[0]);
		SDL_BlitSurface(NULL, NULL, tela, &alvoVida[1]);
		SDL_BlitSurface(NULL, NULL, tela, &alvoVida[2]);
	}

		atualizaPontos(pontuacao);
		char ponto_txt[10] = "Pontos:";
		desenha_texto((strcat(ponto_txt,pontos_str)),tela,400,50);
}














    char *nome_arquivo = "recordes.dat";
    FILE *p_arquivo;

    ARRAY array;

ARRAY arraynew;


void MANAGER_FILE()
{
    array.n = 0;
	p_arquivo = fopen("recordes.dat", "r");
    
	if(fopen("recordes.dat", "r") != NULL)
	{
		fread(&array, sizeof(ARRAY), 1, p_arquivo);
		insere(&array, rec1);
		arraynew = array;
	}

	else
	{
		array.n = 0;
	}
}



void listaRecorde()
{
	int i, x= 150, y=200;
    
	if(fopen("recordes.dat", "r") != NULL)
	{
		fread(&arraynew, sizeof(ARRAY), 1, p_arquivo);
		for(i=0; i<10; i++)
		{
			if(arraynew.vetor[i].pontuacao != 0)
			{
				desenha_texto(arraynew.vetor[i].nome, tela, 50, y);
				itoa(arraynew.vetor[i].pontuacao, pontsrec );
				desenha_texto(pontsrec, tela, x+100, y);
				y+= 50;
			}
		}
	}
 

	else
	{
		desenha_texto("No scores!",tela, x, 70);
	}
	
	y = 0;
	
	imprime(&arraynew);
	fclose(p_arquivo);
}










void DIGITAR_NOME()
{

       fclose(p_arquivo);
       p_arquivo = fopen("recordes.dat", "w");
       
       //draw_text("GAME OVER!", fonte, screen, 350, 0, 0, 0, 255);
       desenha_texto("GAME OVER!", tela, 350, 100);
desenha_texto("Digite seu nome : ", tela, 10, 300);
desenha_texto(rec1.nome, tela, 400, 300);

}






//================================================================================================== 
//-------------------------------------FUNCAO PRINCIPAL---------------------------------------------
//==================================================================================================

int main (int argc, char *argv[])
{
 //-------------------------------------INICIALIZANDO SDL---------------------------------------------

MANAGER_FILE();
init();
int quit = 0;
    // Chama Introdução
    intro_jogo(&quit);

    if (quit == 1)
    {
        return 0;
    }

    SDL_WM_SetCaption( "BOMBER", NULL );
    //Initialize all SDL subsystems
    if( SDL_Init( SDL_INIT_EVERYTHING ) == -1 )
    {
        return 1;
    }
    
    //Initialize SDL_ttf
    if( TTF_Init() == -1 )
    {
        return 1;
    }

    //Initialize SDL_mixer
    if( Mix_OpenAudio( 22050, MIX_DEFAULT_FORMAT, 2, 4096 ) == -1 )
    {
        return 1;
    }

    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);


	if(SDL_Init(SDL_INIT_VIDEO) != 0)
	{
		printf("Erro: %s\n", SDL_GetError());
		exit(-1);
	}
	menu();


//---------------------------------------FIM SDL-----------------------------------------------------
printf("VIDA %d\n",vida);
       SDL_Flip(tela);
    clean_up();
SDL_Quit();
return 0;

}


//================================================================================================== 
//------------------------------------------FUNCOES-------------------------------------------------
//==================================================================================================


//-----------------------------------CRIAR MAPA BONUS-----------------------------------------------
void funcaoBonus()
{
	int i,j;
	int bonus;
	for(i = 0; i<15; i++)
	{
		for(j = 0; j<19; j++)
		{
			bonus = rand() % 5;
			mapaBonus[i][j] = bonus;
		}
	}
}


//--------------------------------------------TEMPO--------------------------------------------------
void controla_fps (int tempo_inicial)
{
	int fps = 1000/6;
	int tempo_agora = SDL_GetTicks() - tempo_inicial;
	if(tempo_agora < fps)
	SDL_Delay(fps - tempo_agora);
}

//------------------------------------CARREGA IMAGENS-----------------------------------------------
int carrega_imagens ()
{
	bomba = IMG_Load("bomba2.png");
	if (bomba == NULL)
	{
		printf("Não carregou imgBomba.bmp\n");
		return 0;
	}

	vidinha = IMG_Load("bonuss.png");
	if (vidinha == NULL)
	{
		printf("Não carregou vida.png\n");
		return 0;
	}


	parede = SDL_LoadBMP("imgParede.bmp");
	if (parede == NULL)
	{
		printf("Não carregou imgParede.bmp\n");
		return 0;
	}

	muro = SDL_LoadBMP("imgMuro.bmp");
	if (parede == NULL)
	{
		printf("Não carregou imgMuro.bmp\n");
		return 0;
	}

	player = SDL_LoadBMP("sprite.bmp");
	if (player == NULL)
	{
		printf("Não carregou imgJogador.bmp\n");
		return 0;
	}

	final = IMG_Load("home.png");
	if (final == NULL)
	{
		printf("Não carregou fim.bmp\n");
		return 0;
	}

	ganhou = IMG_Load("bonus1.png");
	if (final == NULL)
	{
		printf("Não carregou futuro.bmp\n");
		return 0;
	}

	perdeu = IMG_Load("bonus1.png");
	if (final == NULL)
	{
		printf("Não carregou futuro.bmp\n");
		return 0;
	}

	grama = SDL_LoadBMP("imgGrama.bmp");
	if (grama == NULL)
	{
		printf("Não carregou imgGrama.bmp\n");
		return 0;
	}
	
	pont = SDL_LoadBMP("invu.bmp");
	if (pont == NULL)
	{
		//bonus = 2
		printf("Não carregou bonu.bmp\n");
		return 0;
	}

	nada = SDL_LoadBMP("imgGrama.bmp");
	if (nada == NULL)
	{
		//bonus = 4
		printf("Não carregou imgGrama.bmp\n");
		return 0;
	}

	pont_inimigo = IMG_Load("inimi9.png");
	if (inimigo == NULL)
	{
		//bonus = 5
		printf("Não carregou sprite.bmp\n");
		return 0;
	}

	vetorBomba[0] = IMG_Load("nova4.png");
	if (inimigo == NULL)
	{
		printf("Não carregou kba1.bmp\n");
		return 0;
	}

	vetorBomba[1] = IMG_Load("nova3.png");
	if (inimigo == NULL)
	{
		printf("Não carregou kba2.bmp\n");
		return 0;
	}

	vetorBomba[2] = IMG_Load("nova2.png");
	if (inimigo == NULL)
	{
		printf("Não carregou kba3.bmp\n");
		return 0;
	}

	vetorBomba[3] = IMG_Load("nova1.png");
	if (inimigo == NULL)
	{
		printf("Não carregou kba4.bmp\n");
		return 0;
	}

	top = SDL_LoadBMP("top1.bmp");
	if (inimigo == NULL)
	{
		printf("Não carregou top1.bmp\n");
		return 0;
	}

	return 1;
}
//------------------------------------MOVIMENTO INIMIGO----------------------------------------------
void moveInimigos ()
{
int p;
	for(p = 0 ; p < 5; p++)
	{
		posi = rand() % 4; 
		if(inimigo[p].Ativo == 1)
		{
			//__________________________________ESQUERDA__________________________________
			if(posi == 1)
			{
				if(mapa[inimigo[p].colunaInimigo][inimigo[p].linhaInimigo-1]!=MURO && mapa[inimigo[p].colunaInimigo][inimigo[p].linhaInimigo-1]!=PAREDE)
				inimigo[p].linhaInimigo = inimigo[p].linhaInimigo - 1;
			}		
			//_____________________________________DIREITA_______________________________
			if(posi == 2)
			{
				if(mapa[inimigo[p].colunaInimigo][inimigo[p].linhaInimigo+1]!=MURO && mapa[inimigo[p].colunaInimigo][inimigo[p].linhaInimigo+1]!=PAREDE)
				inimigo[p].linhaInimigo = inimigo[p].linhaInimigo + 1;
			}
			//________________________________________CIMA______________________________
			if(posi == 3)
			{
				if(mapa[inimigo[p].colunaInimigo-1][inimigo[p].linhaInimigo]!=MURO && mapa[inimigo[p].colunaInimigo-1][inimigo[p].linhaInimigo]!=PAREDE)
				inimigo[p].colunaInimigo = inimigo[p].colunaInimigo - 1;
			}
			//______________________________________BAIXO_______________________________
			if(posi == 4)
			{
				if(mapa[inimigo[p].colunaInimigo+1][inimigo[p].linhaInimigo]!=MURO && mapa[inimigo[p].colunaInimigo+1][inimigo[p].linhaInimigo]!=PAREDE)
				inimigo[p].colunaInimigo = inimigo[p].colunaInimigo + 1;
			}		
		}
	}
}


//-------------------------------------DESENHAR MAPA------------------------------------------------
void desenha_mapa (int tempoBomba)
{
	SDL_Rect destino;
	int linha, coluna;
	for (linha = 0; linha < 15; linha++)
	{
		destino.y = linha * 40 + 120;
		for (coluna = 0; coluna < 19; coluna++)
		{
			destino.x = coluna * 40;
			if (mapa[linha][coluna] == PAREDE)
			{
				SDL_BlitSurface(parede, NULL, tela, &destino);
			}
			
			else if (mapa[linha][coluna] == MURO)
			{
				SDL_BlitSurface(muro, NULL, tela, &destino);
			}
			
			else if (mapa[linha][coluna] == GRAMA)
			{
				SDL_BlitSurface(grama, NULL, tela, &destino);
			}
			
			else if (mapa[linha][coluna] == GRASS)
			{
				if(mapaBonus[linha][coluna] == 0)
				{
					SDL_BlitSurface(pont, NULL, tela, &destino);
				}
				else if(mapaBonus[linha][coluna] == 1)
				{
					SDL_BlitSurface(nada, NULL, tela, &destino);
				}			

				else if(mapaBonus[linha][coluna] == 2)
				{
					SDL_BlitSurface(nada, NULL, tela, &destino);
				}

				else if(mapaBonus[linha][coluna] == 3)
				{
					SDL_BlitSurface(nada, NULL, tela, &destino);
				}

				else if(mapaBonus[linha][coluna] == 4)
				{
					SDL_BlitSurface(nada, NULL, tela, &destino);
				}

				SDL_BlitSurface(grass, NULL, tela, &destino);
			}
			
			else if (mapa[linha][coluna] == SAIDA)
			{
				SDL_BlitSurface(parede, NULL, tela, &destino);
			}

			else if (mapa[linha][coluna] == CASA)
			{
				SDL_BlitSurface(final, NULL, tela, &destino);
			}

				if(SDL_GetTicks() - tempoBomba < 2000 && tempoBomba != 0)
				{
					SDL_BlitSurface(bomba, NULL, tela, &rectBomba);
				}

				else if(SDL_GetTicks() - tempoBomba < 2250 && tempoBomba != 0)
				{
					SDL_BlitSurface(vetorBomba[0], NULL, tela, &rectBomba);
				}

				else if(SDL_GetTicks() - tempoBomba < 2500 && tempoBomba != 0) 
				{
					SDL_BlitSurface(vetorBomba[1], NULL, tela, &rectBomba);
				}

				else if(SDL_GetTicks() - tempoBomba < 2750 && tempoBomba != 0)
				{
					SDL_BlitSurface(vetorBomba[2], NULL, tela, &rectBomba);
				}

				else if(SDL_GetTicks() - tempoBomba < 3000 && tempoBomba != 0)
				{
					SDL_BlitSurface(vetorBomba[3], NULL, tela, &rectBomba);
				}

				else if(SDL_GetTicks() - tempoBomba < 3250 && tempoBomba != 0) 
				{
					tempoBomba = 0;
					bombaAtiva = 0;

					SDL_BlitSurface(grama, NULL, tela, &rectBomba);
				}
				else
				{					
					SDL_BlitSurface(grama, NULL, tela, &rectBomba);

				}
		}
	}
}


//------------------------------------MOVIMENTO JOGADOR----------------------------------------------

void move_jogador ( SDL_Event event )
{
	switch (event.type)
	{
		case SDL_QUIT:
		break;
	
		case SDL_KEYDOWN:
		switch (event.key.keysym.sym)
		{
//________________________________________ESQUERDA___________________________________________________
			case SDLK_LEFT:
				if(mapa[coluna_atual][linha_atual-1]!=MURO && mapa[coluna_atual][linha_atual-1]!=PAREDE && mapa[coluna_atual][linha_atual-1]!=SAIDA)
				linha_atual = linha_atual - 1;
				if ( rect_box.x == 192 )
					rect_box.x = 224;
				else
					rect_box.x = 192;
					destino.x -= 5;
			break;
//___________________________________________DIREITA_________________________________________________
			case SDLK_RIGHT:
				if(mapa[coluna_atual][linha_atual+1]!=MURO && mapa[coluna_atual][linha_atual+1]!=PAREDE && mapa[coluna_atual][linha_atual+1]!=SAIDA)
				linha_atual = linha_atual + 1;
				if ( rect_box.x == 64 )
					rect_box.x = 96;
				else
					rect_box.x = 64;
					destino.x += 5;
			break;

//____________________________________________CIMA___________________________________________________
			case SDLK_UP:
				if(mapa[coluna_atual-1][linha_atual]!=MURO && mapa[coluna_atual-1][linha_atual]!=PAREDE && mapa[coluna_atual-1][linha_atual]!=SAIDA)
				coluna_atual = coluna_atual - 1;
				if ( rect_box.x == 0 )
					rect_box.x = 32;
				else
					rect_box.x = 0;
					destino.y -= 5;
			break;
//___________________________________________BAIXO___________________________________________________
			case SDLK_DOWN:
				if(mapa[coluna_atual+1][linha_atual]!=MURO && mapa[coluna_atual+1][linha_atual]!=PAREDE && mapa[coluna_atual+1][linha_atual]!=SAIDA)
					coluna_atual = coluna_atual + 1;
				if ( rect_box.x == 128 )
					rect_box.x = 160;
				else
					rect_box.x = 128;
					destino.y += 5;
			break;
//___________________________________________BOMBA___________________________________________________
			case SDLK_SPACE:
				if(bombaAtiva == 0)
				{
					bombaAtiva = 1;
					rectBomba.x = linha_atual*40;
					rectBomba.y = coluna_atual*40 + 120;
					linhaBomba = linha_atual;
					colunaBomba = coluna_atual;
					tempoBomba = SDL_GetTicks();
				}
				SDL_Rect alvo;
			break;	

		}
	}	
}	

//------------------------------------VERIFICA BONUS----------------------------------------------

void verificaBonus ()
{
	if(mapa[coluna_atual][linha_atual] == GRASS && mapaBonus[coluna_atual][linha_atual] == 0)
	{
		mapa[coluna_atual][linha_atual] = GRAMA;
		mapaBonus[coluna_atual][linha_atual] = 4;
		pontuacao = pontuacao + 20;
		pontuacao_auxiliar = pontuacao_auxiliar + 20;
	}
}




















//----------------------------------CONTATO COM INIMIGO----------------------------------------------

void contatoInimigo()
{
	int p;
	for(p = 0; p<5; p++)
	{
		if(inimigo[p].colunaInimigo == coluna_atual && inimigo[p].linhaInimigo == linha_atual && inimigo[p].Ativo == 1)
		{
			linha_atual = 1;
			coluna_atual = 1;
			vida--;
		}
			
		else if(inimigo[p].colunaInimigo == coluna_atual && inimigo[p].linhaInimigo == linha_atual && inimigo[p].Ativo == 1)
		{
			linha_atual = 1;
			coluna_atual = 1;
			vida--;
		}
	
		else if(inimigo[p].colunaInimigo == coluna_atual && inimigo[p].linhaInimigo == linha_atual && inimigo[p].Ativo == 1)
		{
			linha_atual = 1;
			coluna_atual = 1;
			vida--;
		}
	
		else if(inimigo[p].colunaInimigo == coluna_atual && inimigo[p].linhaInimigo == linha_atual && inimigo[p].Ativo == 1)
		{
			linha_atual = 1;
			coluna_atual = 1;
			vida--;
		}
	
		else if(inimigo[p].colunaInimigo == coluna_atual && inimigo[p].linhaInimigo == linha_atual && inimigo[p].Ativo == 1)
		{
			linha_atual = 1;
			coluna_atual = 1;
			vida--;
		}
	}
}











//----------------------------------------ESCOLHA MENU----------------------------------------------

void escolha_menu( SDL_Event event, SDL_Rect destino)
{
	int mouse_x;
	int mouse_y;
	
	if( event.type == SDL_MOUSEBUTTONUP )
	{                

		if( event.button.button == SDL_BUTTON_LEFT)
		{
			//Get the mouse offsets
			mouse_x = event.button.x;
			mouse_y = event.button.y;
	
			if (mouse_x >= 290 && mouse_x <= 290 + 180 && mouse_y >= 180 && mouse_y <= 180 + 40)
			{
				reinicia();
				jogoAtivo = 1;
				introducao();
			}

			else if (mouse_x >= 290 && mouse_x <= 290 + 180 && mouse_y >= 250 && mouse_y <= 250 + 40)
			{
				instrucoes();
			}

			else if (mouse_x >= 290 && mouse_x <= 290 + 320 && mouse_y >= 320 && mouse_y <= 320 + 40)
			{
				recordes();
			}

			else if (mouse_x >= 290 && mouse_x <= 290 + 390 && mouse_y >= 390 && mouse_y <= 390 + 40)
			{
				fim = 1;
			}
		}
	}
}


//-------------------------------------------MENU--------------------------------------------------
int menu()
{
	SDL_Event evento; //para os eventos
	
	telaInicial = IMG_Load("telaInicial.jpg");
	botao_novo = IMG_Load("novojogo.png");
	botao_instru = IMG_Load("instrucoes.png");
	botao_record = IMG_Load("record.png");
	botao_sair = IMG_Load("sair.png");
	
	if (botao_novo == NULL)
	{
		printf("Não carregou imgMuro.bmp\n");
		return 0;
	}
//_____________________________________CONFIGURACAO VIDEO____________________________________________

	tela = SDL_SetVideoMode(800, 600, BPP, SDL_SWSURFACE | SDL_ANYFORMAT);
	if(tela == NULL)
	{
		printf("Erro: %s\n", SDL_GetError());
		return 1; //encerre o programa 
	}

//________________________________________LOOP EVENTOS______________________________________________
	int tempo_inicial;
	while (fim == 0) //Enquanto NÃO for verdadeiro o fim
	{	int var;
		while(SDL_PollEvent(&evento))
		{
			if(evento.type == SDL_QUIT)
			{
				fim = 1;
			}
			else
			{						
				escolha_menu(evento,destino);
			}
		}

		rect_bNovo.x = 290;
		rect_bNovo.y = 180;

		rect_bInstru.x = 290;
		rect_bInstru.y = 250;

		rect_bRecord.x = 290;
		rect_bRecord.y = 320;

		rect_bSair.x = 290;
		rect_bSair.y = 390;
				
		//Pinta a tela inteira de branco antes de desenhar o mapa, esse branco eh o caminho 
		SDL_FillRect(tela, NULL, SDL_MapRGB(tela->format, 255, 255, 255));
		SDL_BlitSurface(telaInicial,NULL,tela,&rect_tela);
		SDL_BlitSurface(botao_novo,NULL,tela,&rect_bNovo);
		SDL_BlitSurface(botao_instru,NULL,tela,&rect_bInstru);
		SDL_BlitSurface(botao_record,NULL,tela,&rect_bRecord);
		SDL_BlitSurface(botao_sair,NULL,tela,&rect_bSair);
		SDL_UpdateRect(tela,0,0,0,0); //Atualiza a tela inteira
		controla_fps(tempo_inicial); // controla o FPS
	}
return 0;
}

//---------------------------------------INTRODUCAO--------------------------------------------------
int introducao()
{
	SDL_Event evento; //para os eventos
	
	telaInicial = IMG_Load("intro.png");
	
	if (telaInicial == NULL)
	{
		printf("Não carregou imgMuro.bmp\n");
		return 0;
	}
//_____________________________________CONFIGURACAO VIDEO____________________________________________

	tela = SDL_SetVideoMode(800, 600, BPP, SDL_SWSURFACE | SDL_ANYFORMAT);
	if(tela == NULL)
	{
		printf("Erro: %s\n", SDL_GetError());
		return 1; //encerre o programa 
	}

//________________________________________LOOP EVENTOS______________________________________________
	int tempo_inicial;
	while (fim == 0) //Enquanto NÃO for verdadeiro o fim
	{	int var;
		while(SDL_PollEvent(&evento))
		{
			if(evento.type == SDL_QUIT)
			{
				fim = 1;
			}
			else if(evento.type == SDL_KEYDOWN)
			{
				switch (evento.key.keysym.sym)
				{
//________________________________________ESQUERDA___________________________________________________
					case SDLK_ESCAPE:
					jogo();
				
					break;
				}
			}
		}

		//Pinta a tela inteira de branco antes de desenhar o mapa, esse branco eh o caminho 
		SDL_FillRect(tela, NULL, SDL_MapRGB(tela->format, 255, 255, 255));
		SDL_BlitSurface(telaInicial,NULL,tela,&rect_tela);
		SDL_UpdateRect(tela,0,0,0,0); //Atualiza a tela inteira
		controla_fps(tempo_inicial); // controla o FPS
	}
return 0;
}

//---------------------------------------INTRUCOES--------------------------------------------------
int instrucoes()
{
	SDL_Event evento; //para os eventos
	
	telaInicial = IMG_Load("instru.png");
	
	if (telaInicial == NULL)
	{
		printf("Não carregou imgMuro.bmp\n");
		return 0;
	}
//_____________________________________CONFIGURACAO VIDEO____________________________________________

	tela = SDL_SetVideoMode(800, 600, BPP, SDL_SWSURFACE | SDL_ANYFORMAT);
	if(tela == NULL)
	{
		printf("Erro: %s\n", SDL_GetError());
		return 1; //encerre o programa 
	}

//________________________________________LOOP EVENTOS______________________________________________
	int tempo_inicial;
	while (fim == 0) //Enquanto NÃO for verdadeiro o fim
	{	int var;
		while(SDL_PollEvent(&evento))
		{
			if(evento.type == SDL_QUIT)
			{
				fim = 1;
			}
			else if(evento.type == SDL_KEYDOWN)
			{
				switch (evento.key.keysym.sym)
				{
//________________________________________ESQUERDA___________________________________________________
					case SDLK_ESCAPE:
					menu();
				
					break;
				}
			}
		}

		//Pinta a tela inteira de branco antes de desenhar o mapa, esse branco eh o caminho 
		SDL_FillRect(tela, NULL, SDL_MapRGB(tela->format, 255, 255, 255));
		SDL_BlitSurface(telaInicial,NULL,tela,&rect_tela);
		SDL_UpdateRect(tela,0,0,0,0); //Atualiza a tela inteira
		controla_fps(tempo_inicial); // controla o FPS
	}
return 0;
}


//---------------------------------------RECORD----------------------------------------------------
int recordes()
{


	SDL_Event evento; //para os eventos
	
	telaRecord = IMG_Load("telaRecord.jpg");
		
	if (telaRecord == NULL)
	{
		printf("Não carregou imgMuro.bmp\n");
		return 0;
	}
//_____________________________________CONFIGURACAO VIDEO____________________________________________

	tela = SDL_SetVideoMode(800, 600, BPP, SDL_SWSURFACE | SDL_ANYFORMAT);
	if(tela == NULL)
	{
		printf("Erro: %s\n", SDL_GetError());
		return 1; //encerre o programa 
	}

//________________________________________LOOP EVENTOS______________________________________________
	int tempo_inicial;
	while (fim == 0) //Enquanto NÃO for verdadeiro o fim
	{	
		while(SDL_PollEvent(&evento))
		{
			if(evento.type == SDL_QUIT)
			{
				fim = 1;
			}
			if(evento.type == SDL_KEYDOWN)
			{
				switch (evento.key.keysym.sym)
				{
//________________________________________ESQUERDA___________________________________________________
					case SDLK_ESCAPE:
					menu();
				
					break;
				}
			}
		}
				
		//Pinta a tela inteira de branco antes de desenhar o mapa, esse branco eh o caminho 
		SDL_FillRect(tela, NULL, SDL_MapRGB(tela->format, 255, 255, 255));
		SDL_BlitSurface(telaRecord,NULL,tela,&rect_tela);
	


	font = TTF_OpenFont("cool.ttf", FONTSIZE);
  	desenha_texto("*****RECORDS*****", tela, 300, 200);


	listaRecorde();
	//SDL_Flip(tela);


		SDL_UpdateRect(tela,0,0,0,0); //Atualiza a tela inteira
		controla_fps(tempo_inicial); // controla o FPS


	}
    clean_up();
return 0;
}




















//--------------------------------------------JOGO---------------------------------------------------
int jogo()
{

    char *nome_arquivo = "recordes.dat";
    FILE *p_arquivo;

    ARRAY array;
    array.n = 0;

    if((p_arquivo = fopen(nome_arquivo, "r")) != NULL)
    {
        fread(&array, sizeof(ARRAY), 1, p_arquivo);
        fclose(p_arquivo);
    }

	srand(time(NULL));
	int colorkey;
	int colorBonus;
	int posi;
	int colorBomba;
	int colorBombaInicial;

	SDL_Event evento; //para os eventos
	SDL_Rect alvo;
	funcaoBonus();  //função para criar matriz de bônus

	if (carrega_imagens() == 0) //Se não carregou uma ou outra imagem 
	{
		return 1; //encerre o programa
	}
//_____________________________________CONFIGURACAO VIDEO___________________________________________
	tela = SDL_SetVideoMode(LARGURA, ALTURA, BPP, SDL_SWSURFACE | SDL_ANYFORMAT);
	if(tela == NULL)
	{
		printf("Erro: %s\n", SDL_GetError());
		return 1; //encerre o programa 
	}

	int tempo_inicial;

	rect_box.x = 128;
	rect_box.y = 0;
	rect_box.w = 32;
	rect_box.h = 32;

//_______________________________________IMG TRANSPARENTE___________________________________________

	colorkey = SDL_MapRGB(tela->format, 255, 0, 255);
	SDL_SetColorKey(player, SDL_SRCCOLORKEY | SDL_RLEACCEL, colorkey);

	colorBonus = SDL_MapRGB(tela->format, 255, 255, 255);
	SDL_SetColorKey(pont, SDL_SRCCOLORKEY | SDL_RLEACCEL, colorkey);

	colorBomba = SDL_MapRGB(tela->format, 0, 0, 0);
	int k;
	for(k = 0; k<4; k++)
	{
		SDL_SetColorKey(vetorBomba[k], SDL_SRCCOLORKEY | SDL_RLEACCEL, colorBomba);	
	}

	colorBombaInicial = SDL_MapRGB(tela->format, 255, 255, 255);
	SDL_SetColorKey(bomba, SDL_SRCCOLORKEY | SDL_RLEACCEL, colorBomba);


//_______________________________________POSICAO INIMIGO___________________________________________
	int p = 0;
	int posiLinha =0;
	int posiColuna =0;

	while(p<5)
	{
		posiLinha = rand() % 15;
		posiColuna = rand() % 19;

		if(mapa[posiColuna][posiLinha]!=MURO && mapa[posiColuna][posiLinha]!=PAREDE)
		{
			inimigo[p].linhaInimigo = posiLinha;
			inimigo[p].colunaInimigo = posiColuna;
			inimigo[p].Ativo = 1;
			p++;
		}
	}


	
//___________________________________________LOOP PRINCIPAL___________________________________________

	while (fim == 0 && jogoAtivo == 1) //Enquanto NÃO for verdadeiro o fim
	{
			
		if(tempoInimigo == 0)
		{
		tempoInimigo = SDL_GetTicks();
		}
		
		else if(SDL_GetTicks() - tempoInimigo > 500)
		{
		moveInimigos();
		tempoInimigo = 0;
		}

		//Para a funcao controla_fps
		tempo_inicial = SDL_GetTicks();





//****************************************    CONTATO    *****************************************




//___________________________________________EXPLODIR PAREDES_________________________________________
		if(SDL_GetTicks() - tempoBomba > 2000 && tempoBomba > 0 && bombaAtiva == 1)
		{
			if(mapa[colunaBomba + 1][linhaBomba] == PAREDE)
			{
				mapa[colunaBomba + 1][linhaBomba] = GRASS;
			}
	
			if(mapa[colunaBomba - 1][linhaBomba] == PAREDE)
			{
				mapa[colunaBomba - 1][linhaBomba] = GRASS;
			}

			if(mapa[colunaBomba][linhaBomba + 1] == PAREDE)
			{
				mapa[colunaBomba][linhaBomba + 1] = GRASS;
			}

			if(mapa[colunaBomba][linhaBomba - 1] == PAREDE)
			{	
				mapa[colunaBomba][linhaBomba - 1] = GRASS;
			}
//___________________________________________ACHAR CASA___________________________________________

			if(mapa[colunaBomba + 1][linhaBomba] == SAIDA)
			{
				mapa[colunaBomba + 1][linhaBomba] = CASA;
			}
	
			if(mapa[colunaBomba - 1][linhaBomba] == SAIDA)
			{
				mapa[colunaBomba - 1][linhaBomba] = CASA;
			}

			if(mapa[colunaBomba][linhaBomba + 1] == SAIDA)
			{
				mapa[colunaBomba][linhaBomba + 1] = CASA;
			}

			if(mapa[colunaBomba][linhaBomba - 1] == SAIDA)
			{	
				mapa[colunaBomba][linhaBomba - 1] = CASA;
			}







			if(mapa[colunaBomba + 1][linhaBomba] == CASA)
			{
				mapa[colunaBomba + 1][linhaBomba] = CASA;
			}
	
			if(mapa[colunaBomba - 1][linhaBomba] == CASA)
			{
				mapa[colunaBomba - 1][linhaBomba] = CASA;
			}

			if(mapa[colunaBomba][linhaBomba + 1] == CASA)
			{
				mapa[colunaBomba][linhaBomba + 1] = CASA;
			}

			if(mapa[colunaBomba][linhaBomba - 1] == CASA)
			{	
				mapa[colunaBomba][linhaBomba - 1] = CASA;
			}

//___________________________________________CONTATO BOMBA___________________________________________

			if(colunaBomba +1 == coluna_atual && linhaBomba == linha_atual)
			{
				linha_atual = 1;
				coluna_atual = 1;
				vida = vida -1;
			}
			
			else if(colunaBomba -1 == coluna_atual && linhaBomba == linha_atual)
			{
				linha_atual = 1;
				coluna_atual = 1;
				vida = vida -1;
			}
	
			else if(colunaBomba == coluna_atual && linhaBomba + 1 == linha_atual )
			{
				linha_atual = 1;
				coluna_atual = 1;
				vida = vida -1;
			}
		
			else if(colunaBomba == coluna_atual && linhaBomba - 1 == linha_atual)
			{
				linha_atual = 1;
				coluna_atual = 1;
				vida = vida -1;
			}
		
			else if(colunaBomba == coluna_atual && linhaBomba == linha_atual)
			{
				linha_atual = 1;
				coluna_atual = 1;
				vida = vida -1;
			}


//___________________________________________CONTATO INIMIGO_________________________________________

			int p;
			for(p = 0 ; p < 5; p++)
			{
				if(colunaBomba +1 == inimigo[p].colunaInimigo && linhaBomba == inimigo[p].linhaInimigo)
				{
					if(inimigo[p].Ativo ==1)
					pontuacao = pontuacao + 50;
					pontuacao_auxiliar = pontuacao_auxiliar + 50;
					inimigo[p].Ativo = 0;
					mapa[colunaBomba + 1][linhaBomba] = GRAMA;
				}

				else if(colunaBomba -1 == inimigo[p].colunaInimigo && linhaBomba == inimigo[p].linhaInimigo)
				{
				if(inimigo[p].Ativo ==1)
					pontuacao = pontuacao + 50;
					pontuacao_auxiliar = pontuacao_auxiliar + 50;
					inimigo[p].Ativo = 0;
					mapa[colunaBomba - 1][linhaBomba] = GRAMA;
				}
		
				else if(colunaBomba == inimigo[p].colunaInimigo && linhaBomba + 1 == inimigo[p].linhaInimigo )
				{
					if(inimigo[p].Ativo ==1)
					pontuacao = pontuacao + 50;
					pontuacao_auxiliar = pontuacao_auxiliar + 50;
					inimigo[p].Ativo = 0;
					mapa[colunaBomba][linhaBomba + 1] = GRAMA;
				}
			
				else if(colunaBomba == inimigo[p].colunaInimigo && linhaBomba - 1 == inimigo[p].linhaInimigo)
				{
					if(inimigo[p].Ativo ==1)
					pontuacao = pontuacao + 50;
					pontuacao_auxiliar = pontuacao_auxiliar + 50;
					inimigo[p].Ativo = 0;
					mapa[colunaBomba][linhaBomba - 1] = GRAMA;	
				}
		
				else if(colunaBomba == inimigo[p].colunaInimigo && linhaBomba == inimigo[p].linhaInimigo)
				{
					if(inimigo[p].Ativo ==1)
					pontuacao = pontuacao + 50;
					pontuacao_auxiliar = pontuacao_auxiliar + 50;
					inimigo[p].Ativo = 0;
					mapa[colunaBomba][linhaBomba - 1] = GRAMA;	
				}
			}
		}





//___________________________________________LOOP EVENTOS___________________________________________
		while(SDL_PollEvent(&evento))
		{
			if(evento.type == SDL_QUIT)
			{
				jogoAtivo = 0;
				    clean_up();
				fim = 1;
			}
	
			//move o jogador
			if (evento.type == SDL_KEYDOWN)
			{
				move_jogador(evento);
			}	
			
			if(evento.type == SDL_KEYDOWN)
			{
				switch (evento.key.keysym.sym)
				{
//________________________________________SAIR___________________________________________________
					case SDLK_ESCAPE:
					    imprime(&array);

					    
					    printf("Nome do Recordista:");
					    fgets(rec1.nome, MAX_NOME, stdin);

					    rec1.nome[strlen(rec1.nome) - 1] = '\0';

					    rec1.pontuacao = pontuacao;

					    insere(&array, rec1);
					    
					    imprime(&array);

					    if((p_arquivo = fopen(nome_arquivo, "w")) == NULL)
					    {
						perror("fopen:");
						return 1;
					    }

					    fwrite(&array, sizeof(ARRAY), 1, p_arquivo);
					    fclose(p_arquivo);
					menu();
				
					break;
				}
			}
		}
				
//___________________________________________VERIFICAR FIM___________________________________________
		SDL_Rect rect_futuro;		
		rect_futuro.x = 200;
		rect_futuro.y = 200;

		if (mapa[coluna_atual][linha_atual] ==  CASA)
		{
			//SDL_BlitSurface(ganhou,NULL,tela,&rect_futuro);
			SDL_Delay(4000);
    			clean_up();
			reinicia();
			menu();
		}

		if(vida < 1)
		{
			//SDL_BlitSurface(perdeu,NULL,tela,&rect_futuro);
			SDL_Delay(3000);
imprime(&array);

					    
					    printf("Nome do Recordista:");
					    fgets(rec1.nome, MAX_NOME, stdin);

					    rec1.nome[strlen(rec1.nome) - 1] = '\0';

					    rec1.pontuacao = pontuacao;

					    insere(&array, rec1);
					    
					    imprime(&array);

					    if((p_arquivo = fopen(nome_arquivo, "w")) == NULL)
					    {
						perror("fopen:");
						return 1;
					    }

					    fwrite(&array, sizeof(ARRAY), 1, p_arquivo);
					    fclose(p_arquivo);
			
			clean_up();
			reinicia();
			menu();
		}
	
		//Pinta a tela inteira de branco antes de desenhar o mapa, esse branco eh o caminho 
		SDL_FillRect(tela, NULL, SDL_MapRGB(tela->format, 0, 0, 0));
	
		//Desenha o mapa sobre a tela
		desenha_mapa(tempoBomba);
	
		int i;
		destino.x = linha_atual * 40;
		destino.y = coluna_atual * 40 + 120;
		
		//Posição inimigo
		for(i = 0; i<5;i++ )
		{
			inimigo[i].rect_inimigo.x = inimigo[i].linhaInimigo * 40;
			inimigo[i].rect_inimigo.y = inimigo[i].colunaInimigo * 40 + 120;
		}
				
		//verificaExplosao(destino.x,destino.y);
		topo();

		// pegue a imagem player completa(NULL) e jogue na tela em destino 
		//SDL_BlitSurface(player, NULL, tela, &destino);
		SDL_BlitSurface(player,&rect_box,tela,&destino);

		for(i = 0; i<5;i++ )
		{
			if(inimigo[i].Ativo == 1)
			SDL_BlitSurface(pont_inimigo,NULL,tela,&inimigo[i].rect_inimigo);
		}

		SDL_UpdateRect(tela,0,0,0,0); //Atualiza a tela inteira
		controla_fps(tempo_inicial); // controla o FPS
		
		verificaBonus();
		contatoInimigo();
		if(pontuacao_auxiliar > 500)
		{
			if(vida<3)
			{
				vida = vida + 1;
			}
			pontuacao_auxiliar = 0;
		}


	}

  
  TTF_Quit();

return 0;
}
















//--------------------------------------------SOM----------------------------------------------------
int init()
{
    //Initialize all SDL subsystems
    if( SDL_Init( SDL_INIT_EVERYTHING ) == -1 )
    {
        return 1;
    }
    
    //Initialize SDL_ttf
    if( TTF_Init() == -1 )
    {
        return 1;
    }

    //Initialize SDL_mixer
    if( Mix_OpenAudio( 22050, MIX_DEFAULT_FORMAT, 2, 4096 ) == -1 )
    {
        return 1;
    }

    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
    
    //Set the window caption
   // SDL_WM_SetCaption( "Apocalypto", NULL );


    srand( (unsigned)time(NULL) ); // seed do rand
    
    //If everything initialized fine
    return 0;
}


int intro_jogo(int *p_quit)
{
    //MUSICA=====================================================
    Mix_Music *intro_musica;
    intro_musica = Mix_LoadMUS( "bomber2.mp3" );
    Mix_FadeInMusic(intro_musica, -1, 2000);
    //MUSICA==========================================================

    int mouse_x = 0, mouse_y = 0;
    int flags = 0;

    

return 0;
}



