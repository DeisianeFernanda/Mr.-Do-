# Mr.-Do-

1.	O jogo:
“Mr. Do!” é um jogo  criado pela Universal em 1982, tendo sua mecânica semelhante ao game “Dig-Dug”.
O objeto de Mr. Do! É marcar pontos o quanto possível por meio de coleta de cerejas e atingindo fantasmas (inimigos de Mr. Do!), para isso ele dispõe de sua defesa a possibilidade de atirar nos inimigos e cavar túneis para alcançar as cerejas.
2.	Regras/instruções:
	Para movimentar o mr. Do! Use as teclas direcionais 
	Para atirar nos inimigos pressione a tecla ‘espaço’
	O jogo termina quando: não há mais cerejas para coletar, não ter mais fantasmas para eliminar, ser atingido por algum fantasma (game over)
	Cada cereja equivale a 50 pontos
	Cada fantasma equivale a 10 pontos

3.	Código e suas funções:

3.1	Geral: O código é composto por 26 funções e uma função main (principal) totalizando 1316 linhas.

3.2	 Das funções:
	int menu_inicial():  função que gera um menu principal para interagir com o jogador, por ser int retornará um inteiro sendo este a escolha do usuário, a cada escolha uma interação diferente sendo este executado com um switch case.
	void abre_le_arquivo(char m[NLIN][NCOL], char *nome_arq):Abre o arquivo texto que irá “desenhar “ o cenário do game, a leitura dos elementos é feito char por char, ele ignora o ” \n” e pega o próximo char do arquivo.
	void imprime_matriz(char m[NLIN][NCOL]): Nesta função, ocorrerá  a impressão da matriz do arquivo texto(anterior já citada),em que ocorrerá uma substituição os caracteres por outros para dar característica ao cenário, por exemplo, onde houver uma letra ‘p’ no arquivo texto ele vai substituir(só na impressão) por ‘-80’  código que irá desenhar  a suposta parede.(para mais detalhes gráficos vide o jogo).
	void desconverter_matriz(char m[NLIN][NCOL], int *direcao): Atualiza a matriz com letras para salvar o jogo.
	void abre_grava_continuar_mapa(char m[NLIN][NCOL]): grava o mapa já atualizado do jogo em um arquivo txt para que haja a possibilidade de continuar o jogo.
	void abre_grava_continuar_estado(TIPO_ESTADO estado):Cria um novo arquivo binário em cima dos antigos dados, atualizando o estado jogo.
	void tratamento_continuar(char m[NLIN][NCOL]) :”Limpa”  o tiro da matriz quando for continuar um jogo gravado.
	int le_arquivo_hs(TIPO_SCORE entradas[]) e  void imprime_highscores(TIPO_SCORE entradas[], int numentradas ):supostamente Lê o arquivo binário com os scores de jogos anteriores e imprime os scores para o usuário.
	Para os fantasmas tem as funções:

-	int isCross(char m[NLIN][NCOL],int *x, int *y, int *direcao)
-	void newDirection(char m[NLIN][NCOL],int *x, int *y, int ignoreLast)
-	int isNextWall(char m[NLIN][NCOL],int *x, int *y, int *direcao)
-	void nextStep(char m[NLIN][NCOL],int *x, int *y, int direcao)
-	void func_fantasma(char m[NLIN][NCOL], int *x, int *y, int *direcao)
Em que a primeira dá a condição de movimento para o fantasma, a segunda seleciona randomicamente a posição para o fantasma percorrer, para criação dos fantasmas toda vez que for par, de 2 em 2 segundos, ele executa passo fantasma, usando o tempo corrente com a função time.h(biblioteca) .
	int procura_fruta(char m[NLIN][NCOL]): Função que condiz quantas cerejas há no cenário,ao encontrar uma cereja ele incrementa 1,caso não haja mais frutas a função retornará 0.
	void tiro(char m[NLIN][NCOL],int x,int y,int direcao,int *pontos, int *ativo):Ao pressionar a tecla espaço, essa função é ativada em que testa as posições e possibilidade de ter atingido um fantasma que ao destruí-lo, soma 10 pontos no score.
	Main(): função principal que une as demais funções, sendo o laço de execução dado por um “do-while”.








