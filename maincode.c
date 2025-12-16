/*
    Projeto Mr Do!

    Definicoes: Ao continuar um jogo assumir que seja fase 1 e carregar funções fase 1

*/

#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <string.h>
#include <ctype.h>
#include <conio2.h>
#include <time.h>
#define MAXSCORES 5
#define MAXNOME 40
#define NLIN 23
#define NCOL 60
#define NGHOST 10

#define COD_MRDO 64 //227 -69 64
#define COD_FANTASMA -103 //153 -103 48
#define COD_NINHO -50 //206 -50 88
#define COD_PAREDE 35 //176 -80 35
#define COD_FRUTA 102 //229 -27 102
#define COD_TIRO -6 //250 -6 42
#define COD_CAMINHO 32
#define COD_BARRA 219 //-37 219

#define COD_CIMA 72
#define COD_ESQUERDA 75
#define COD_DIREITA 77
#define COD_BAIXO 80
#define COD_ESC 27
#define COD_VEL_TIRO 17

#define COR_MENU 14
#define COR_PAREDE 10
#define COR_FANTASMA 15
#define COR_FRUTA 12
#define COR_MRDO 11
#define COR_TIRO 15
#define COR_NINHO 8

char arquivo_highscores[] = "highscores.bin";
char arquivo_fase1[] = "fase1.txt";
char arquivo_fase2[] = "fase2.txt";
char arquivo_continuar[] = "continuar.txt";
char arquivo_estado[] = "estado.bin";

//int a=0;

typedef struct
{
    int pontos;
    char nome[MAXNOME];
} TIPO_SCORE;

typedef struct
{
    int pontos, fantasmas;
} TIPO_ESTADO;

typedef struct
{
    int x, y, direcao;
    int status;
} TIPO_MRDO;

int menu_inicial()
{
    int resposta=0, i;
    char simbol;

    do
    {
        system("cls");
        textcolor(COR_MENU); //4-red, 14-amarelo, 7-cinza, 9-azul

        gotoxy(10,6); //posicao da barra
        simbol = 219;
        for(i=0; i<60; i++) //impressao da barra
            printf("%c", simbol);

        gotoxy(37,7);
        printf("Mr Do!\n");
        //pula uma linha

        gotoxy(10,9);
        simbol = 205; //simbolo da linha
        for(i=0; i<60; i++) //impresao da linha
            printf("%c", simbol);

        gotoxy(20,10);
        printf("1 - Novo Jogo\n");
        gotoxy(20,11);
        printf("2 - Continuar\n");
        gotoxy(20,12);
        printf("3 - High Scores\n");
        gotoxy(20,13);
        printf("4 - Sair\n");
        //pula uma linha

        gotoxy(10,15);
        simbol = 196; //simbolo da linha
        for(i=0; i<60; i++) //impressao da linha
            printf("%c", simbol);

        gotoxy(20,16);
        printf("Entre com a opcao desejada: \n");

        gotoxy(10,17);
        simbol = 219;
        for(i=0; i<60; i++) //impressao da barra
            printf("%c", simbol);

        gotoxy(47,16); //posicao certa para scan opcao menu
        scanf("%d", &resposta);
    }
    while(resposta!=1 && resposta!=2 && resposta!=3 && resposta!=4 && resposta!=5);

    return resposta;
}

///Novo Jogo
void abre_le_arquivo(char m[NLIN][NCOL], char *nome_arq)
{
    FILE *arq;
    int linha, coluna;
    char c;

    arq = fopen(nome_arq,"r"); //abre arquivo fase 1 pressupondo que ele exista
    if (arq == NULL) // retorno NULL indica erro na abertura
        printf("Erro ao abrir arquivo fase ou arquivo continuar.");

    for(linha=0; linha<NLIN; linha++)
    {
        for(coluna=0; coluna<NCOL; coluna++)
        {
            c = fgetc(arq); //pega char por char
            if(c=='\n') //final da linha, ele pega char de novo pra excluir \n
                c = fgetc(arq); //pegou novo char e ignorou \n

            m[linha][coluna] = c; //preencheu a matriz com o char
        }
    }
     if(ferror(arq))
        printf("Erro ao ler dados do arquivo fase ou arquivo continuar.");
    fclose(arq);
}

void imprime_matriz(char m[NLIN][NCOL])
{
    int linha, coluna, i;
    textcolor(COR_PAREDE);
    for(i=1;i<=NCOL+2;i++)
    {
        gotoxy(i,1);
        printf("%c",COD_BARRA);
        gotoxy(i,25);
        printf("%c",COD_BARRA);
    }
    printf("\n");
    for(linha=0; linha<NLIN; linha++)
    {
        for(coluna=0; coluna<NCOL; coluna++)
        {
            switch(m[linha][coluna])
            {
                case 'p' : m[linha][coluna] = COD_PAREDE; //parede
                textcolor(COR_PAREDE);
                break;
                case 'i' : m[linha][coluna] = COD_FANTASMA; //fantasma
                textcolor(COR_FANTASMA);
                break;
                case 'f' : m[linha][coluna] = COD_FRUTA; //fruta
                textcolor(COR_FRUTA);
                break;
                case 'd' : m[linha][coluna] = COD_MRDO; //mrdo
                textcolor(COR_MRDO);
                break;
                case 't' : m[linha][coluna] = COD_TIRO; //tiro
                textcolor(COR_TIRO);
                break;
                case 'n' : m[linha][coluna] = COD_NINHO; //ninho fantasma
                textcolor(COR_NINHO);
                break;
                case 'v': m[linha][coluna] = COD_CAMINHO; //caminho vazio
                break;
            }
            gotoxy(coluna+2,linha+2);
            printf("%c",m[linha][coluna]);
        }
        printf("\n");
    }
    for(i=2;i<=NLIN+2;i++)
    {
        gotoxy(1,i);
        printf("%c",COD_BARRA);
        gotoxy(62,i);
        printf("%c",COD_BARRA);
    }
}

///Continuar Jogo
void desconverter_matriz(char m[NLIN][NCOL], int *direcao) //atualiza matriz com letras para salvar o arquivo
{
    int linha, coluna, pos;
    for(linha=0; linha<NLIN; linha++)
    {
        for(coluna=0; coluna<NCOL; coluna++)
        {
            pos = m[linha][coluna];
            switch(pos)
            {
                case COD_PAREDE : m[linha][coluna] = 'p'; //parede
                break;
                case COD_FANTASMA : m[linha][coluna] = 'i'; //fantasma
                break;
                case COD_FRUTA : m[linha][coluna] = 'f'; //fruta
                break;
                case COD_MRDO : m[linha][coluna] = 'd'; //mrdo
                break;
                case COD_TIRO : m[linha][coluna] = 'v'; //tiro = recebe vazio e status de tiro volta desligado
                break;
                case COD_NINHO : m[linha][coluna] = 'n'; //ninho fantasma
                break;
                case COD_CAMINHO: m[linha][coluna] = 'v'; //caminho vazio
                break;
            }
        }
    }
}

void abre_grava_continuar_mapa(char m[NLIN][NCOL])
{
    FILE *arq;
    int linha,coluna;

    arq = fopen(arquivo_continuar,"w");

    if (arq == NULL) // retorno NULL indica erro na abertura
        printf("Erro ao abrir arquivo continuar.");
    /*for(linha=0; linha<NLIN; linha++)
    {
        fputs(m[linha], arq); //grava linha por linha
    }*/
    for(linha=0;linha<NLIN;linha++)
    {
        for(coluna=0;coluna<NCOL;coluna++)
        {
            putc(m[linha][coluna],arq);
        }
        putc('\n',arq);
    }
    fclose(arq);
    if(ferror(arq))
        printf("Erro ao gravar dados no arquivo continuar.");
}

void abre_grava_continuar_estado(TIPO_ESTADO estado)
{
    FILE *arq;
    arq = fopen(arquivo_estado,"wb"); //wb cria um novo por cima com os novos dados do estado
    if (arq == NULL) // verifica se funcionou: retorno NULL indica erro
        printf("Erro ao abrir arquivo estado.");
    else
    {
        fwrite(&estado, sizeof(TIPO_ESTADO), 1, arq);
    }
    fclose(arq);
    if(ferror(arq))
        printf("Erro ao gravar dados no arquivo estado.");
}

TIPO_ESTADO le_arquivo_estado(TIPO_ESTADO estado)
{
    FILE *arq;
    arq = fopen(arquivo_estado,"rb"); //rb somente leitura pressupondo que o arquivo exista.
    if (arq == NULL) // verifica se funcionou: retorno NULL indica erro
        printf("Erro ao ler arquivo estado.");
    else
    {
        fread(&estado, sizeof(TIPO_ESTADO), 1, arq);
    }
    if(ferror(arq))
        printf("Erro ao ler dados do arquivo estado.");
    fclose(arq);
    return estado;
}

void tratamento_continuar(char m[NLIN][NCOL]) //limpar tiro da matriz quando for continuar um jogo gravado --SALVANDO JOGO SEM TIRO
{
    int linha, coluna;
    for(linha=0;linha<NLIN;linha++)
    {
        for(coluna=0;coluna<NCOL;coluna++)
        {
            if(m[linha][coluna] == COD_TIRO)
                m[linha][coluna] = COD_CAMINHO;
        }
    }
}

///High Scores
int le_arquivo_hs(TIPO_SCORE entradas[])
{
    FILE *arq;
    int cont;
    arq = fopen(arquivo_highscores,"rb"); //rb somente leitura pressupondo que o arquivo exista.
    if (arq == NULL) // verifica se funcionou: retorno NULL indica erro
        printf("Erro na abertura do arquivo highscores.");
    else
    {
        if((cont = fread(entradas,sizeof(TIPO_SCORE), MAXSCORES, arq)))
        {
            if(ferror(arq))
            {
                cont = -1;
            }
        }
    }
    fclose(arq);
    return cont;
}

void imprime_highscores(TIPO_SCORE entradas[], int numentradas)
{
    int i;
    printf("HIGHSCORES\nNOME                SCORE");
    for(i=0; i<numentradas; i++)
    {
        printf("\n%-20s %-20d",entradas[i].nome,entradas[i].pontos);
    }
}

void grava_arquivo_hs(TIPO_SCORE entradas[], int numscores)
{
    FILE *arq;
    arq = fopen(arquivo_highscores,"wb"); //wb cria um novo por cima com os novos dados do score
    if (arq == NULL) // verifica se funcionou: retorno NULL indica erro
        printf("Erro na abertura do arquivo highscores.");
    else
    {
        fwrite(entradas, sizeof(TIPO_SCORE), numscores, arq);
    }
    if(ferror(arq))
    {
        printf("\n\nErro na escrita dos dados no arquivo highscores!");
    }
    fclose(arq);
}

int verifica_score(TIPO_SCORE score[], int atual, int players) //teste com verifica highscores pra saber se vai pro ranking - retorna 1 pra sim, 0 pra não
{
    int ind, flag=0;
    if(players<5)
        flag = 1;
    else
    {
        for(ind=0; ind<players; ind++)
        {
            if(atual > score[ind].pontos) //se pontos atual for maior que alguma pontuacao, ativa flag
                flag = 1; //flag ativada significa que tem que atualizar highscore
        }
    }
    return flag;
}

int atualiza_score(TIPO_SCORE score[], TIPO_SCORE atual, int players) //coloca em ordem o highscore
{
    int ind, sinal, aux;
    char nome[MAXNOME];

    if(players>=1)
    {
        score[players-1].pontos = atual.pontos;
        strcpy(score[players-1].nome, atual.nome);

        do
        {
            sinal = 0;
            for(ind = 0; ind < players-1; ind++)
            {
                if(score[ind].pontos < score[ind + 1].pontos)
                {
                    aux = score[ind].pontos;
                    strcpy(nome, score[ind].nome);

                    score[ind].pontos = score[ind + 1].pontos;
                    strcpy(score[ind].nome, score[ind + 1].nome);

                    score[ind + 1].pontos = aux;
                    strcpy(score[ind + 1].nome, nome);
                    sinal = 1; //indica que houve troca
                }
            }

        }
        while(sinal == 1);
    }
    else
    {
        score[players].pontos = atual.pontos;
        strcpy(score[players].nome, atual.nome);
        players = 1;
    }
    return players;
}

void highscore_completo(int pontos) //ver se entra highscore, se entrar pedir nome, gravar
{
    TIPO_SCORE score[MAXSCORES], atual;
    int quant_pl, vai_score;
    atual.pontos = pontos;
    quant_pl = le_arquivo_hs(score);
    if(quant_pl != -1)
    {
        vai_score = verifica_score(score,atual.pontos,quant_pl);
        if(vai_score)
        {
            clrscr();
            printf("Parabens, voce vai pro HighScores com a pontuacao %d!!!\nDigite seu nome: ", atual.pontos);
            gets(atual.nome);
            quant_pl = atualiza_score(score,atual,quant_pl);
            grava_arquivo_hs(score,quant_pl);
        }
        else
        {
            printf("Que pena! Nao foi dessa vez que voce entrou no seleto grupo HighScores!!!");
        }
    }
    else
    {
        printf("Erro quantidade de players no highscore completo!");
    }

}

///FANTASMA
int isCross(char m[NLIN][NCOL],int *x, int *y, int *direcao)
{
    int ret=0;
    switch(*direcao)
    {
    case 1: //cruzamento pra cima
        if((m[*x-1][*y]==COD_CAMINHO || m[*x-1][*y]==COD_BARRA) && m[*x][*y-1]==COD_CAMINHO && m[*x][*y+1]==COD_CAMINHO)
            ret = 1;
        break;
    case 2: //cruzamento pra direita
        if((m[*x][*y+1]==COD_CAMINHO || m[*x][*y+1]==COD_BARRA) && m[*x-1][*y]==COD_CAMINHO && m[*x+1][*y]==COD_CAMINHO)
            ret = 1;
        break;
    case 3: //cruzamento pra baixo
        if((m[*x+1][*y]==COD_CAMINHO || m[*x+1][*y]==COD_BARRA) && m[*x][*y-1]==COD_CAMINHO && m[*x][*y+1]==COD_CAMINHO)
            ret = 1;
        break;
    case 4: //cruzamento pra esquerda
        if((m[*x][*y-1]==COD_CAMINHO || m[*x][*y-1]==COD_BARRA) && m[*x-1][*y]==COD_CAMINHO && m[*x+1][*y]==COD_CAMINHO)
            ret = 1;
        break;
    }
    return ret;
}

void newDirection(char m[NLIN][NCOL],int *x, int *y, int *direcao, int ignoreLast, int sorte, int *voltar, int *flag)
{
    switch(ignoreLast)
    {
    case 1: //veio de baixo
        if(sorte>=0 && sorte<=3333)
        {
            //case1 - m[x-1][y] cima
            if(m[*x-1][*y]==COD_CAMINHO || m[*x-1][*y]==COD_MRDO)
            {
                if(m[*x-1][*y]==COD_MRDO)
                {
                    *voltar = 1;
                    *flag = 0;
                }
                m[*x][*y] = COD_CAMINHO;
                gotoxy(*y+2,*x+2);
                printf("%c",COD_CAMINHO);

                *x = *x - 1;
                m[*x][*y] = COD_FANTASMA;
                textcolor(COR_FANTASMA);
                gotoxy(*y+2,*x+2);
                printf("%c",COD_FANTASMA);
                *direcao = 1;
            }
        }
        else if(sorte>3333 && sorte<=6666)
        {
            //case 2 - m[x][y-1] esquerda
            if(m[*x][*y-1]==COD_CAMINHO || m[*x][*y-1]==COD_MRDO)
            {
                if(m[*x][*y-1]==COD_MRDO)
                {
                    *voltar = 1;
                    *flag = 0;
                }
                m[*x][*y] = COD_CAMINHO;
                gotoxy(*y+2,*x+2);
                printf("%c",COD_CAMINHO);

                *y = *y - 1;
                m[*x][*y] = COD_FANTASMA;
                textcolor(COR_FANTASMA);
                gotoxy(*y+2,*x+2);
                printf("%c",COD_FANTASMA);
                *direcao = 4;
            }
        }
        else
        {
            //case 3 - m[x][y+1] direita
            if(m[*x][*y+1]==COD_CAMINHO || m[*x][*y+1]==COD_MRDO)
            {
                if(m[*x][*y+1]==COD_MRDO)
                {
                    *voltar = 1;
                    *flag = 0;
                }
                m[*x][*y] = COD_CAMINHO;
                gotoxy(*y+2,*x+2);
                printf("%c",COD_CAMINHO);

                *y = *y + 1;
                m[*x][*y] = COD_FANTASMA;
                textcolor(COR_FANTASMA);
                gotoxy(*y+2,*x+2);
                printf("%c",COD_FANTASMA);
                *direcao = 2;
            }
        }
        break;
    case 2: //veio da esquerda
        if(sorte>=0 && sorte<=3333)
        {
            //case1 - m[x][y+1] direita
            if(m[*x][*y+1]==COD_CAMINHO || m[*x][*y+1]==COD_MRDO)
            {
                if(m[*x][*y+1]==COD_MRDO)
                {
                    *voltar = 1;
                    *flag = 0;
                }
                m[*x][*y] = COD_CAMINHO;
                gotoxy(*y+2,*x+2);
                printf("%c",COD_CAMINHO);

                *y = *y + 1;
                m[*x][*y] = COD_FANTASMA;
                textcolor(COR_FANTASMA);
                gotoxy(*y+2,*x+2);
                printf("%c",COD_FANTASMA);
                *direcao = 2;
            }
        }
        else if(sorte>3333 && sorte<=6666)
        {
            //case 2 - m[x-1][y] cima
            if(m[*x-1][*y]==COD_CAMINHO || m[*x-1][*y]==COD_MRDO)
            {
                if(m[*x-1][*y]==COD_MRDO)
                {
                    *voltar = 1;
                    *flag = 0;
                }
                m[*x][*y] = COD_CAMINHO;
                gotoxy(*y+2,*x+2);
                printf("%c",COD_CAMINHO);

                *x = *x - 1;
                m[*x][*y] = COD_FANTASMA;
                textcolor(COR_FANTASMA);
                gotoxy(*y+2,*x+2);
                printf("%c",COD_FANTASMA);
                *direcao = 1;
            }
        }
        else
        {
            //case 3 - m[x+1][y] baixo
            if(m[*x+1][*y]==COD_CAMINHO || m[*x+1][*y]==COD_MRDO)
            {
                if(m[*x+1][*y]==COD_MRDO)
                {
                    *voltar = 1;
                    *flag = 0;
                }
                m[*x][*y] = COD_CAMINHO;
                gotoxy(*y+2,*x+2);
                printf("%c",COD_CAMINHO);

                *x = *x + 1;
                m[*x][*y] = COD_FANTASMA;
                textcolor(COR_FANTASMA);
                gotoxy(*y+2,*x+2);
                printf("%c",COD_FANTASMA);
                *direcao = 3;
            }
        }
        break;
    case 3: //veio de cima
        if(sorte>=0 && sorte<=3333)
        {
            //case1 - m[x+1][y] baixo
            if(m[*x+1][*y]==COD_CAMINHO || m[*x+1][*y]==COD_MRDO)
            {
                if(m[*x+1][*y]==COD_MRDO)
                {
                    *voltar = 1;
                    *flag = 0;
                }
                m[*x][*y] = COD_CAMINHO;
                gotoxy(*y+2,*x+2);
                printf("%c",COD_CAMINHO);

                *x = *x + 1;
                m[*x][*y] = COD_FANTASMA;
                textcolor(COR_FANTASMA);
                gotoxy(*y+2,*x+2);
                printf("%c",COD_FANTASMA);
                *direcao = 3;
            }
        }
        else if(sorte>3333 && sorte<=6666)
        {
            //case 2 - m[x][y-1] esquerda
            if(m[*x][*y-1]==COD_CAMINHO || m[*x][*y-1]==COD_MRDO)
            {
                if(m[*x][*y-1]==COD_MRDO)
                {
                    *voltar = 1;
                    *flag = 0;
                }
                m[*x][*y] = COD_CAMINHO;
                gotoxy(*y+2,*x+2);
                printf("%c",COD_CAMINHO);

                *y = *y - 1;
                m[*x][*y] = COD_FANTASMA;
                textcolor(COR_FANTASMA);
                gotoxy(*y+2,*x+2);
                printf("%c",COD_FANTASMA);
                *direcao = 4;
            }
        }
        else
        {
            //case 3 - m[x][y+1] direita
            if(m[*x][*y+1]==COD_CAMINHO || m[*x][*y+1]==COD_MRDO)
            {
                if(m[*x][*y+1]==COD_MRDO)
                {
                    *voltar = 1;
                    *flag = 0;
                }
                m[*x][*y] = COD_CAMINHO;
                gotoxy(*y+2,*x+2);
                printf("%c",COD_CAMINHO);

                *y = *y + 1;
                m[*x][*y] = COD_FANTASMA;
                textcolor(COR_FANTASMA);
                gotoxy(*y+2,*x+2);
                printf("%c",COD_FANTASMA);
                *direcao = 2;
            }
        }
        break;
    case 4: //veio da direita
        if(sorte>=0 && sorte<=3333)
        {
            //case1 - m[x][y-1] esquerda
            if(m[*x][*y-1]==COD_CAMINHO || m[*x][*y-1]==COD_MRDO)
            {
                if(m[*x][*y-1]==COD_MRDO)
                {
                    *voltar = 1;
                    *flag = 0;
                }
                m[*x][*y] = COD_CAMINHO;
                gotoxy(*y+2,*x+2);
                printf("%c",COD_CAMINHO);

                *y = *y - 1;
                m[*x][*y] = COD_FANTASMA;
                textcolor(COR_FANTASMA);
                gotoxy(*y+2,*x+2);
                printf("%c",COD_FANTASMA);
                *direcao = 4;
            }
        }
        else if(sorte>3333 && sorte<=6666)
        {
            //case 2 - m[x-1][y] cima
            if(m[*x-1][*y]==COD_CAMINHO || m[*x-1][*y]==COD_MRDO)
            {
                if(m[*x-1][*y]==COD_MRDO)
                {
                    *voltar = 1;
                    *flag = 0;
                }
                m[*x][*y] = COD_CAMINHO;
                gotoxy(*y+2,*x+2);
                printf("%c",COD_CAMINHO);

                *x = *x - 1;
                m[*x][*y] = COD_FANTASMA;
                textcolor(COR_FANTASMA);
                gotoxy(*y+2,*x+2);
                printf("%c",COD_FANTASMA);
                *direcao = 1;
            }
        }
        else
        {
            //case 3 - m[x+1][y] baixo
            if(m[*x+1][*y]==COD_CAMINHO || m[*x+1][*y]==COD_MRDO)
            {
                if(m[*x+1][*y]==COD_MRDO)
                {
                    *voltar = 1;
                    *flag = 0;
                }
                m[*x][*y] = COD_CAMINHO;
                gotoxy(*y+2,*x+2);
                printf("%c",COD_CAMINHO);

                *x = *x + 1;
                m[*x][*y] = COD_FANTASMA;
                textcolor(COR_FANTASMA);
                gotoxy(*y+2,*x+2);
                printf("%c",COD_FANTASMA);
                *direcao = 3;
            }
        }
        break;
    case 0:
        if(sorte>=0 && sorte<=2500)
        {
            //case1 - m[x-1][y] cima
            if(m[*x-1][*y]==COD_CAMINHO || m[*x-1][*y]==COD_MRDO)
            {
                if(m[*x-1][*y]==COD_MRDO)
                {
                    *voltar = 1;
                    *flag = 0;
                }
                m[*x][*y] = COD_CAMINHO;
                gotoxy(*y+2,*x+2);
                printf("%c",COD_CAMINHO);

                *x = *x - 1;
                m[*x][*y] = COD_FANTASMA;
                textcolor(COR_FANTASMA);
                gotoxy(*y+2,*x+2);
                printf("%c",COD_FANTASMA);
                *direcao = 1;
            }
        }
        else if(sorte>2500 && sorte<=5000)
        {
            //case 2 - m[x+1][y] baixo
            if(m[*x+1][*y]==COD_CAMINHO || m[*x+1][*y]==COD_MRDO)
            {
                if(m[*x+1][*y]==COD_MRDO)
                {
                    *voltar = 1;
                    *flag = 0;
                }
                m[*x][*y] = COD_CAMINHO;
                gotoxy(*y+2,*x+2);
                printf("%c",COD_CAMINHO);

                *x = *x + 1;
                m[*x][*y] = COD_FANTASMA;
                textcolor(COR_FANTASMA);
                gotoxy(*y+2,*x+2);
                printf("%c",COD_FANTASMA);
                *direcao = 3;
            }
        }
        else if(sorte>5000 && sorte<=7500)
        {
            //case3 - m[x][y-1] esquerda
            if(m[*x][*y-1]==COD_CAMINHO || m[*x][*y-1]==COD_MRDO)
            {
                if(m[*x][*y-1]==COD_MRDO)
                {
                    *voltar = 1;
                    *flag = 0;
                }
                m[*x][*y] = COD_CAMINHO;
                gotoxy(*y+2,*x+2);
                printf("%c",COD_CAMINHO);

                *y = *y - 1;
                m[*x][*y] = COD_FANTASMA;
                textcolor(COR_FANTASMA);
                gotoxy(*y+2,*x+2);
                printf("%c",COD_FANTASMA);
                *direcao = 4;
            }
        }
        else
        {
            //case 4 - m[x][y+1] direita
            if(m[*x][*y+1]==COD_CAMINHO || m[*x][*y+1]==COD_MRDO)
            {
                if(m[*x][*y+1]==COD_MRDO)
                {
                    *voltar = 1;
                    *flag = 0;
                }
                m[*x][*y] = COD_CAMINHO;
                gotoxy(*y+2,*x+2);
                printf("%c",COD_CAMINHO);

                *y = *y + 1;
                m[*x][*y] = COD_FANTASMA;
                textcolor(COR_FANTASMA);
                gotoxy(*y+2,*x+2);
                printf("%c",COD_FANTASMA);
                *direcao = 2;
            }
        }
        break;
    }
}

int isNextWall(char m[NLIN][NCOL],int *x, int *y, int *direcao)
{
    int ret=1;
    switch(*direcao)
    {
    case 1: //cima
        if(m[*x-1][*y]==COD_CAMINHO || m[*x-1][*y]==COD_MRDO)
            ret = 0;
        break;
    case 2: //direita
        if(m[*x][*y+1]==COD_CAMINHO || m[*x][*y+1]==COD_MRDO)
            ret = 0;
        break;
    case 3: //baixo
        if(m[*x+1][*y]==COD_CAMINHO || m[*x+1][*y]==COD_MRDO)
            ret = 0;
        break;
    case 4: //esquerda
        if(m[*x][*y-1]==COD_CAMINHO || m[*x][*y-1]==COD_MRDO)
            ret = 0;
        break;
    }
    return ret;
}

void nextStep(char m[NLIN][NCOL],int *x, int *y, int direcao, int *voltar, int *flag)
{
    switch(direcao)
    {
    case 1: //cima
        //printf("\n\ncima");
        //system("pause");
        m[*x][*y] = COD_CAMINHO; //posição atual recebe caminho
        gotoxy(*y+2,*x+2);
        printf("%c",COD_CAMINHO);

        *x = *x - 1; //atualiza posição
        if(m[*x][*y] == COD_MRDO)
        {
            *voltar = 1;
            *flag = 0;
        }
        m[*x][*y] = COD_FANTASMA; //nova posição recebe fantasma
        textcolor(COR_FANTASMA);
        gotoxy(*y+2,*x+2);
        printf("%c",COD_FANTASMA);
        //printf("\n\ncima2");
        //system("pause");
        break;
    case 2: //direita
        //printf("\n\ndireita");
        //system("pause");
        m[*x][*y] = COD_CAMINHO; //posição atual recebe caminho
        gotoxy(*y+2,*x+2);
        printf("%c",COD_CAMINHO);

        *y = *y + 1; //atualiza posição
        if(m[*x][*y] == COD_MRDO)
        {
            *voltar = 1;
            *flag = 0;
        }
        m[*x][*y] = COD_FANTASMA; //nova posição recebe fantasma
        textcolor(COR_FANTASMA);
        gotoxy(*y+2,*x+2);
        printf("%c",COD_FANTASMA);
        break;
    case 3: //baixo
        //printf("\n\nbaixo");
        //system("pause");
        m[*x][*y] = COD_CAMINHO; //posição atual recebe caminho
        textcolor(COR_FANTASMA);
        gotoxy(*y+2,*x+2);
        printf("%c",COD_CAMINHO);

        *x = *x + 1; //atualiza posição
        if(m[*x][*y] == COD_MRDO)
        {
            *voltar = 1;
            *flag = 0;
        }
        m[*x][*y] = COD_FANTASMA; //nova posição recebe fantasma
        textcolor(COR_FANTASMA);
        gotoxy(*y+2,*x+2);
        printf("%c",COD_FANTASMA);
        break;
    case 4: //esquerda
        //printf("\n\nesquerda");
        //system("pause");
        m[*x][*y] = COD_CAMINHO; //posição atual recebe caminho
        gotoxy(*y+2,*x+2);
        printf("%c",COD_CAMINHO);

        *y = *y - 1; //atualiza posição
        if(m[*x][*y] == COD_MRDO)
        {
            *voltar = 1;
            *flag = 0;
        }
        m[*x][*y] = COD_FANTASMA; //nova posição recebe fantasma
        textcolor(COR_FANTASMA);
        gotoxy(*y+2,*x+2);
        printf("%c",COD_FANTASMA);
        break;
    }
}

void func_fantasma(char m[NLIN][NCOL], int *x, int *y, int *direcao, int *status, int sorte, int *voltar, int *flag)
{
    //time_t inicio, fim; /* variaveis do "tipo" tempo */
    //int tempo_total=0;
    if(*status==1)
    {
        //if((tempo_total%2)==0) //toda vez que for par, de 2 em 2 segundos, ele executa passo fantasma
        //{
            //inicio = time(NULL);
            m[*x][*y] = COD_FANTASMA;
            if(isCross(m,x,y,direcao)==1)
            {
                //printf("\n\nisCross%d",a);
                //a++;
                //system("pause");
                newDirection(m,x,y,direcao,*direcao,sorte,voltar,flag);
            }
            else if((isNextWall(m,x,y,direcao))==0)
            {
                //printf("\nisNext%d",a);
                //a++;
                //system("pause");
                nextStep(m,x,y,*direcao,voltar,flag);
            }
            else
            {
                //printf("\n\nNew%d",a);
                //a++;
                //system("pause");
                newDirection(m,x,y,direcao,0,sorte,voltar,flag);
            }
            //fim = time(NULL);
            //tempo_total = tempo_total + difftime(fim,inicio);
        //}
    }
}

///JOGO
void posicao_mrdo(char m[NLIN][NCOL], int *x, int *y)
{
    int linha, coluna;
    for(linha=0; linha<NLIN; linha++)
    {
        for(coluna=0; coluna<NCOL; coluna++)
        {
            if(m[linha][coluna]==COD_MRDO)
            {
                *x = linha;
                *y = coluna;
            }
        }
    }
}

void imprime_score(int score)
{
    gotoxy(68,1);
    textcolor(14);
    printf("Mr. Do!");
    gotoxy(64,5);
    printf("SCORE: %05d",score);
}

int procura_fruta(char m[NLIN][NCOL])
{
    int fruta=0, linha, coluna, ret=1;
    for(linha=0; linha<NLIN; linha++)
    {
        for(coluna=0; coluna<NCOL; coluna++)
        {
            if(m[linha][coluna]==COD_FRUTA) //achou fruta vai incrementando
            {
                fruta++;
            }
        }
    }
    if(fruta==0) //acabaram as frutas, retorna 0
      ret = 0;
    return ret;
}

void tiro(char m[NLIN][NCOL],int x,int y,int direcao,int *pontos, int *ativo, TIPO_MRDO ghost[NGHOST], int *matou)
{
    int char_fut_posicao=0, i=1, max=0,c;
    *ativo = 1;
    switch(direcao)
    {
    case 1: //cima -x
        char_fut_posicao = m[x-i][y];
        while(char_fut_posicao==COD_CAMINHO)
        {
            gotoxy(y+2,x-i+2);
            printf("%c",COD_TIRO);
            Sleep(COD_VEL_TIRO);
            gotoxy(y+2,x-i+2);
            printf("%c",COD_CAMINHO);
            i++;
            char_fut_posicao = m[x-i][y];
        }
        *ativo = 0;
        if(char_fut_posicao==COD_FANTASMA)
        {
            ///destrói fantasma, aumenta *pontos em 10, *ativo = 0 pra dar outro tiro
            m[x-i][y] = COD_CAMINHO;
            gotoxy(y+2,x-i+2);
            printf("%c",COD_CAMINHO);

            *pontos = *pontos + 10;
            *ativo = 0;
            *matou = *matou + 1;
            for(c=0;c<NGHOST;c++)
            {
                if((ghost[c].x==(x-i)) && (ghost[c].y==y))
                {
                    ghost[c].status = 0;
                }
            }
        }
        break;
    case 2: //direita +y
        char_fut_posicao = m[x][y+i];
        while(char_fut_posicao==COD_CAMINHO)
        {
            gotoxy(y+i+2,x+2);
            printf("%c",COD_TIRO);
            Sleep(COD_VEL_TIRO);
            gotoxy(y+i+2,x+2);
            printf("%c",COD_CAMINHO);
            i++;
            char_fut_posicao = m[x][y+i];
        }
        *ativo = 0;
        if(char_fut_posicao==COD_FANTASMA)
        {
            ///destrói fantasma, aumenta *pontos em 10, *ativo = 0 pra dar outro tiro
            m[x][y+i] = COD_CAMINHO;
            gotoxy(y+i+2,x+2);
            printf("%c",COD_CAMINHO);

            *pontos = *pontos + 10;
            *ativo = 0;
            *matou = *matou + 1;
            for(c=0;c<NGHOST;c++)
            {
                if((ghost[c].x==x) && (ghost[c].y==(y+i)))
                    ghost[c].status = 0;
            }
        }
        break;
    case 3: //baixo +x
        char_fut_posicao = m[x+i][y];
        while(char_fut_posicao==COD_CAMINHO)
        {
            gotoxy(y+2,x+i+2);
            printf("%c",COD_TIRO);
            Sleep(COD_VEL_TIRO);
            gotoxy(y+2,x+i+2);
            printf("%c",COD_CAMINHO);
            i++;
            char_fut_posicao = m[x+i][y];
        }
        *ativo = 0;
        if(char_fut_posicao==COD_FANTASMA)
        {
            ///destrói fantasma, aumenta *pontos em 10, *ativo = 0 pra dar outro tiro
            m[x+i][y] = COD_CAMINHO;
            gotoxy(y+2,x+i+2);
            printf("%c",COD_CAMINHO);

            *pontos = *pontos + 10;
            *ativo = 0;
            *matou = *matou + 1;
            for(c=0;c<NGHOST;c++)
            {
                if((ghost[c].x==(x+i)) && (ghost[c].y==y))
                    ghost[c].status = 0;
            }
        }
        break;
    case 4: //esquerda -y
        char_fut_posicao = m[x][y-i];
        while(char_fut_posicao==COD_CAMINHO)
        {
            gotoxy(y-i+2,x+2);
            printf("%c",COD_TIRO);
            Sleep(COD_VEL_TIRO);
            gotoxy(y-i+2,x+2);
            printf("%c",COD_CAMINHO);
            i++;
            char_fut_posicao = m[x][y-i];
        }
        *ativo = 0;
        if(char_fut_posicao==COD_FANTASMA)
        {
            ///destrói fantasma, aumenta *pontos em 10, *ativo = 0 pra dar outro tiro
            m[x][y-i] = COD_CAMINHO;
            gotoxy(y-i+2,x+2);
            printf("%c",COD_CAMINHO);

            *pontos = *pontos + 10;
            *ativo = 0;
            *matou = *matou + 1;
            for(c=0;c<NGHOST;c++)
            {
                if((ghost[c].x==x) && (ghost[c].y==(y-i)))
                    ghost[c].status = 0;
            }
        }
        break;
    }
}

void move_switch(char m[NLIN][NCOL], int *x, int *y, int *direcao, int *pontos, int *fantasmas, int *voltar, int *flag)
{
    int char_fut_posicao=0, futx=0, futy=0;
    if(*direcao==1) //cima
    {
        //printf("\nentrou if *direcao=%d direcao=%d",*direcao, direcao);
        futx = *x - 1;
        //printf("\nfutx=%d *x=%d", futx, *x);
        futy = *y;
        //printf("\nfuty=%d *y=%d", futy, *y);
        char_fut_posicao = m[futx][futy];
        //printf("\nfutura posicao %d", char_fut_posicao);
    }
    else
        if(*direcao==2) //direita
        {
            //printf("\nentrou if *direcao=%d direcao=%d",*direcao, direcao);
            futx = *x;
            //printf("\nfutx=%d *x=%d", futx, *x);
            futy = *y + 1;
            //printf("\nfuty=%d *y=%d", futy, *y);
            char_fut_posicao = m[futx][futy];
            //printf("\nfutura posicao %d", char_fut_posicao);
        }
        else
            if(*direcao==3) //baixo
            {
                //printf("\nentrou if *direcao=%d direcao=%d",*direcao, direcao);
                futx = *x + 1;
                //printf("\nfutx=%d *x=%d", futx, *x);
                futy = *y;
                //printf("\nfuty=%d *y=%d", futy, *y);
                char_fut_posicao = m[futx][futy];
                //printf("\nfutura posicao %d", char_fut_posicao);
            }
            else
                if(*direcao==4) //esquerda
                {
                    //printf("\nentrou if *direcao=%d direcao=%d",*direcao, direcao);
                    futx = *x;
                    //printf("\nfutx=%d *x=%d", futx, *x);
                    futy = *y - 1;
                    //printf("\nfuty=%d *y=%d", futy, *y);
                    char_fut_posicao = m[futx][futy];
                    //printf("\nfutura posicao %d", char_fut_posicao);
                }
    switch(char_fut_posicao)
    {
    case COD_CAMINHO: //caminho vazio
        //printf("\nfutura posicao %d", char_fut_posicao);
        m[futx][futy] = COD_MRDO; //futura posicao recebendo o mrdo
        gotoxy(futy+2,futx+2); //cursor na posição nova
        textcolor(COR_MRDO); //muda cor para cor do símbolo
        printf("%c",m[futx][futy]); //printando mrdo na tela

        m[*x][*y] = COD_CAMINHO; //posição anterior recebendo caminho(vazio)
        gotoxy(*y+2,*x+2); //cursor na posição antiga
        printf("%c",m[*x][*y]); //printando caminho na tela
        *x = futx; //atualizando x mrdo
        *y = futy; //atualizando y mrdo

        //printf("");

        //clrscr();
        //imprime_matriz(m);
        break;
    case COD_FRUTA: //fruta
        m[futx][futy] = COD_MRDO; //futura posicao recebendo o mrdo
        gotoxy(futy+2,futx+2); //cursor na posição nova
        textcolor(COR_MRDO); //muda cor para cor do símbolo
        printf("%c",m[futx][futy]); //printando mrdo na tela

        m[*x][*y] = COD_CAMINHO; //posição anterior recebendo caminho(vazio)
        gotoxy(*y+2,*x+2); //cursor na posição antiga
        printf("%c",m[*x][*y]); //printando caminho na tela
        *x = futx; //atualizando x mrdo
        *y = futy; //atualizando y mrdo
        *pontos = *pontos + 50; //comer fruta aumenta 50 pontos

        //clrscr();
        //imprime_matriz(m);
        break;
    case COD_NINHO: //ninho fantasma
        //não faz nada, pois não é possível atravessar o ninho fantasma
        break;
    case COD_PAREDE: //parede
        m[futx][futy] = COD_MRDO; //futura posicao recebendo o mrdo
        gotoxy(futy+2,futx+2); //cursor na posição nova
        textcolor(COR_MRDO); //muda cor para cor do símbolo
        printf("%c",m[futx][futy]); //printando mrdo na tela

        m[*x][*y] = COD_CAMINHO; //posição anterior recebendo caminho(vazio)
        gotoxy(*y+2,*x+2); //cursor na posição antiga
        printf("%c",m[*x][*y]); //printando caminho na tela
        *x = futx; //atualizando x mrdo
        *y = futy; //atualizando y mrdo

        //clrscr();
        //imprime_matriz(m);
        break;
    case COD_FANTASMA: //fantasma
        //se próxima posição for fantasma, ele morre e jogo pausa //implementar vidas //cheat PACMAN(imortal e mata fantasma)
        *voltar = 1;
        *flag = 0;
        break;
    }
}

void movimentacao(char m[NLIN][NCOL],char *tecla, int *x, int *y, int *direcao, int *pontos, int *fantasmas, int *voltar, int *flag, int *ativo, TIPO_MRDO ghost[NGHOST], int *matou)
{
    int itecla;
    itecla = *tecla;
    TIPO_ESTADO estado;
    switch(itecla)
    {
    case COD_CIMA: //SETA CIMA
        if(*x>0)
        {
            //printf("\nentrou cima *tecla=%d itecla=%d",*tecla, itecla);
            //printf("\nx=%d *x=%d", x, *x);
            *direcao = 1;
            move_switch(m,x,y,direcao,pontos,fantasmas,voltar,flag); //FUNÇÃO COM MATRIZ + POSIÇÃO MR DO X Y E DIREÇÃO, PONTOS, FANTASMAS
            //gotoxy(1,25);
            //printf("teste");
        }
        break;
    case COD_ESQUERDA: //SETA ESQUERDA
        if(*y>0)
        {
            *direcao = 4;
            move_switch(m,x,y,direcao,pontos,fantasmas,voltar,flag);
            //gotoxy(1,25);
            //printf("teste");
        }
        break;
    case COD_DIREITA: //SETA DIREITA
        if(*y<59)
        {
            *direcao = 2;
            move_switch(m,x,y,direcao,pontos,fantasmas,voltar,flag);
            //gotoxy(1,25);
            //printf("teste");
        }
        break;
    case COD_BAIXO: //SETA BAIXO
        if(*x<22)
        {
            *direcao = 3;
            move_switch(m,x,y,direcao,pontos,fantasmas,voltar,flag);
            //gotoxy(1,25);
            //printf("teste");
        }
        break;
    case COD_CAMINHO: //ESPAÇO - TIRO
        if(*ativo==0)
        {
            tiro(m,*x,*y,*direcao,pontos,ativo,ghost,matou);
        }
        break;
    case COD_ESC: //ESC
        desconverter_matriz(m,direcao);
        abre_grava_continuar_mapa(m);
        estado.pontos = *pontos; //atualiza pontos
        estado.fantasmas = *fantasmas; //atualiza fantasmas
        abre_grava_continuar_estado(estado);
        *flag = 0; //sair do loop do jogo
        *voltar = 1; //voltar pro menu
        break;
    }
}

int jogo_principal(char m[NLIN][NCOL], int *pontos, int *fantasmas, int *voltar, int fase)
{
    int flag=1,fruta=1,retorno=0,ativo=0,ii,achou_fant=0;
    int linha, coluna;
    TIPO_MRDO mrdo;
    TIPO_MRDO ghost[NGHOST];
    int cont_ghost=0;
    char tecla;
    int posicao=0, quant_ghost=0, matou=0;
    mrdo.direcao = 1;
    int velocidade=1;

    time_t inicio, fim, fim2; /* variaveis do "tipo" tempo */
    int tempo_total=0, i;
    int tempo_anda=0;
    int sorte;
    srand(time(NULL));

    /*if(fase==1) //se fase=1, velocidade fase 1, se fase=2, velocidade fase 2
        velocidade = 2;
    else
        velocidade = 1;*/

    for(i=0;i<10;i++)
    {
        ghost[i].x = 11;
        ghost[i].y = 23;
        ghost[i].direcao=1;
        ghost[i].status=0;
    }
    if(*fantasmas>0)
    {
        for(linha=0; linha<NLIN; linha++)
        {
            for(coluna=0; coluna<NCOL; coluna++)
            {
                if(m[linha][coluna]==COD_FANTASMA)
                {
                    ghost[achou_fant].x = linha;
                    ghost[achou_fant].y = coluna;
                    ghost[achou_fant].status = 1;
                    ghost[achou_fant].direcao = 0;
                    achou_fant++;
                }
            }
        }
        cont_ghost = *fantasmas - 1;
    }
    do
    {
        sorte = rand() % 10000;
        inicio = time(NULL);
        if(kbhit())
        {
            fflush(stdin);
            tecla = getch();
        }
        else
        {
            tecla = 0;
        }
        if(tecla==-32)
        {
            tecla = getch();
        }
        posicao_mrdo(m,&mrdo.x,&mrdo.y);
        movimentacao(m,&tecla,&mrdo.x,&mrdo.y,&mrdo.direcao,pontos,fantasmas,voltar,&flag,&ativo,ghost,&matou);
        imprime_score(*pontos);
        fruta = procura_fruta(m);
        if(tecla!=COD_ESC)
        {
            if(fruta==0 || matou==10)
            {
                flag = 0;
                retorno = 1;
            }
        }
        fim = time(NULL);
        tempo_total = tempo_total + difftime(fim,inicio);
        if(tempo_total==3)
        {
            //cria fantasma
            if(cont_ghost<10)
            {
                ghost[cont_ghost].status = 1;
                func_fantasma(m,&ghost[cont_ghost].x,&ghost[cont_ghost].y,&ghost[cont_ghost].direcao,&ghost[cont_ghost].status,sorte,voltar,&flag);//-1 no cont status
                if(cont_ghost<10)
                    cont_ghost++;
                tempo_total = 0;
                *fantasmas = *fantasmas + 1;
            }
        }
        gotoxy(25,13);
        textcolor(COR_NINHO);
        printf("%c",COD_NINHO);
        fim2 = time(NULL);
        tempo_anda =  tempo_anda + difftime(fim2,inicio);
        if((tempo_anda==velocidade))
        {
            for(ii=0;ii<NGHOST;ii++)
            {
                func_fantasma(m,&ghost[ii].x,&ghost[ii].y,&ghost[ii].direcao,&ghost[ii].status,sorte,voltar,&flag);
            }
            tempo_anda = 0;
        }
    }while(flag==1);
    return retorno;
}

main()
{
    TIPO_SCORE score[MAXSCORES], atual;
    TIPO_ESTADO estado;
    int opcao_menu, quant_pl, fase=1, vai_score=0, voltar=0, proximo=0;
    char matriz[NLIN][NCOL], nome[MAXNOME], resp;

    time_t inicio, fim; /* variaveis do "tipo" tempo */
    int tempo,i;

    do
    {
        opcao_menu = menu_inicial();
        fflush(stdin);

        switch(opcao_menu)
        {
        case 1: //novo jogo
            clrscr();
            abre_le_arquivo(matriz,arquivo_fase1); //le o arquivo fase e preenche a matriz
            imprime_matriz(matriz);
            estado.pontos = 0;
            estado.fantasmas = 0;
            proximo = jogo_principal(matriz,&estado.pontos,&estado.fantasmas,&voltar,fase);
            if(proximo == 1) //vitoria, fase 2
            {
                highscore_completo(estado.pontos); //final do jogo le score, ver se entra highscore, se entrar pedir nome, gravar
                fase = 2;
                clrscr();
                abre_le_arquivo(matriz,arquivo_fase2); //le o arquivo fase e preenche a matriz
                imprime_matriz(matriz);
                estado.pontos = 0;
                estado.fantasmas = 0;
                proximo = jogo_principal(matriz,&estado.pontos,&estado.fantasmas,&voltar,fase);
                if(proximo == 1)
                {
                    highscore_completo(estado.pontos);
                }
            }
            break;

        case 2: //continuar jogo anterior
            clrscr();
            estado = le_arquivo_estado(estado); //le estado do continuar e salva em estado
            abre_le_arquivo(matriz,arquivo_continuar); //matriz atualizada com arquivo continuar
            imprime_matriz(matriz);
            proximo = jogo_principal(matriz,&estado.pontos,&estado.fantasmas,&voltar,fase);
            if(proximo == 1) //vitoria por fruta, fase 2
            {
                highscore_completo(estado.pontos); //final do jogo le score, ver se entra highscore, se entrar pedir nome, gravar
                fase = 2;
                clrscr();
                abre_le_arquivo(matriz,arquivo_fase2); //le o arquivo fase e preenche a matriz
                imprime_matriz(matriz);
                estado.pontos = 0;
                estado.fantasmas = 0;
                proximo = jogo_principal(matriz,&estado.pontos,&estado.fantasmas,&voltar,fase);
                if(proximo == 1)
                {
                    highscore_completo(estado.pontos);
                }
            }
            break;

        case 3: //highscores
            clrscr();
            quant_pl = le_arquivo_hs(score);//le arquivo highscores e retorna quantidade de players
            imprime_highscores(score,quant_pl);
            do
            {
                printf("\n\nDeseja voltar ao menu? (S/N) ");
                scanf(" %c",&resp);
                resp = toupper(resp);
            }
            while(resp!='S' && resp!='N');
            if(resp=='S')
                voltar = 1;
            else
                voltar = 0;
            break;

        case 4://sair
            clrscr();
            textcolor(12);
            printf("Obrigado por jogar o Mr Do, jogo desenvolvido por alguem que nao conheco mas \nversao atual feita por Deisiane e Fernando.\n\n");
            voltar = 0;
            break;
        case 5: //teste

            inicio = time(0);
            fim = time(0);
            tempo = difftime(fim,inicio);
            clrscr();
            for(i=0;i<10;i++)
            {
                //Sleep(1000);
                //fim = time(0);
                //tempo = difftime(fim,inicio);
                printf("Tempo em segundo(s): %d\n",tempo);
            }
            break;

        default:
            clrscr();
            printf("Erro desconhecido(switch case)!\n\n");
            voltar = 0;
        }
    }
    while(voltar==1);
}
