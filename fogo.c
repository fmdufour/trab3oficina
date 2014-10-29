#include <stdio.h>
#include <stdlib.h>

#define escCima 'U'
#define escBaixo 'D'
#define escCiBa 'E'
#define fogo 'F'
#define torneira 'T'
#define chao '.'
#define parede '#'
#define saida 'S'
#define comeco 'J'


int a,w,h,b;
int dy[4]={1,0,-1,0};
int dx[4]={0,1,0,-1};
int temp = 0;


typedef struct Coord{
int x;
int y;
int z;
}Coord;	

typedef struct CoordQ{
int z;
int y;
int x;
int b;
}CoordQ;

	

typedef struct ElemPred{
	char obj;
	int dist;
}ElemPred;	

typedef CoordQ ElementType;

typedef struct qNode{
ElementType element;
struct qNode *next;
}qNode;

typedef struct Queue{
qNode *front;
qNode *back;
}Queue;

Coord inicio;
Coord final;




void qInic(Queue *q){
	q->front = (qNode*) malloc(sizeof(qNode));
	q->back = q->front;
	q->front->next = NULL;
}

void qEnqueue(Queue *q, ElementType *element){	
		q->back->next = (qNode*)malloc(sizeof(qNode));
		q->back = q->back->next;
		q->back->element = *element;
		q->back->next = NULL;	
}

int qIsEmpty(Queue *q){
	return (q->front == q->back);
}

void qDequeue(Queue *q, ElementType *element){
	qNode *aux;
    aux = q->front->next;
    free(q->front);
    q->front = aux;    
    *element = aux->element;
}


ElemPred**** AlocaMatriz4d(){
int i, j, k;
ElemPred ****matriz4d;
// Faz o alocamento de uma matriz array, ultilizando mallocs.

	matriz4d=(ElemPred ****) malloc(sizeof(ElemPred ***)*a);
	if (matriz4d == NULL)
		exit(1);

	for (i= 0; i<a; i++)
		matriz4d[i] = (ElemPred ***) malloc(sizeof(ElemPred**)*h);
		
	for (i= 0; i<a; i++)
		for (j=0; j<h;j++)
			matriz4d[i][j] = (ElemPred**) malloc(sizeof(ElemPred*)*w);
			
	for (i= 0; i<a; i++)
		for (j=0; j<h; j++)
			for( k=0; k<w; k++)
				matriz4d[i][j][k] = (ElemPred*) malloc(sizeof(ElemPred)*b+1);
return matriz4d; 			       		
}

void DefineInicio(int i,int j,int k){
	inicio.z = i;
	inicio.y = j;						
	inicio.x = k;
}
void DefineFinal(int i,int j,int k){
	final.z = i;
	final.y = j;						
	final.x = k;   
}

char GetEsp(ElemPred ****predio, ElementType cd){
	return predio[cd.z][cd.y][cd.x][cd.b].obj;
}



void LePredio(ElemPred**** matriz4d){
int i,j,k,m;	
		for (k=0; k<a; k++) 		
			for (i= 0; i<h; i++)
				for (j=0; j<w; j++)	{	
					char valor;
					scanf(" %c", &valor);
					if (valor == comeco){
						DefineInicio(k,i,j);
					}			
					if (valor == saida){
						DefineFinal(k,i,j);
					}
					for (m = 0; m < b+1; m++)
					{
						matriz4d[k][i][j][m].obj = valor;
						matriz4d[k][i][j][m].dist = -1;
					}							
				}
}

int EspacoValido(ElemPred ****predio, int z, int y, int x, int b){
	if (y >= h || y < 0 || x >= w || x < 0)
		return 0;	
    if (predio[z][y][x][b].obj == parede)
			return 0;	
	return 1;	
}		
	
void AddCoordNaFila(Queue *queue, int z, int y, int x, int baldes){
	ElementType coord;
	coord.z = z;
	coord.y = y;
	coord.x = x;
	coord.b = baldes;
	qEnqueue(queue, &coord);
}	

void AumentaDist(ElemPred ****predio, int z, int y, int x, int baldes, int distAnt){	
	predio[z][y][x][baldes].dist = distAnt +1;
}

int MenorDist(ElemPred ****predio, int z, int y, int x, int baldes, int distAnt){	
	if (predio[z][y][x][baldes].dist > distAnt+1 || predio[z][y][x][baldes].dist == -1) //se tiver o valor padrão
		return 1;																//de -1 também retorna true
	else
		return 0;															
}

void AnalisaPosicao(ElemPred ****predio, Queue *fila, int z, int y, int x, int b, int distAnt){
	
	if (MenorDist(predio, z, y, x, b, distAnt)){
		AddCoordNaFila(fila, z, y, x, b);
		AumentaDist(predio, z, y, x, b, distAnt);
	}
}

int main(){	
char espAtual;	
int i;	
ElemPred ****predio;
ElementType c;
Queue queue;
ElementType cd;
int distAtual = 0;
int subtBalde = 0;

scanf("%d %d %d %d", &a, &w, &h, &b);
	
predio = AlocaMatriz4d();
LePredio(predio);


c.y = inicio.y; //coordenadas do inicio
c.x = inicio.x;
c.z = inicio.z;
c.b = 0;
predio[c.z][c.y][c.x][c.b].dist = 0;


qInic(&queue);
qEnqueue(&queue, &c);

	while (!qIsEmpty(&queue)){	
		qDequeue(&queue,&cd); //passou o valor da coordenada para o struct cd
		espAtual = GetEsp(predio, cd); //espaco atual
		distAtual = predio[cd.z][cd.y][cd.x][cd.b].dist;
				
			for (i = 0; i < 4; i++)
			{	//adiciona coordenadas vizinhas do mesmo andar
				if (EspacoValido(predio, cd.z, cd.y+dy[i], cd.x+dx[i], cd.b)){	
					
					if (predio[cd.z][cd.y+dy[i]][cd.x+dx[i]][cd.b].obj == fogo) //se for fogo subtrai um balde para poder 'pisar'
						subtBalde = 1;
					
					if((cd.b-subtBalde) >= 0)
						AnalisaPosicao(predio, &queue, cd.z, cd.y+dy[i], cd.x+dx[i], cd.b-subtBalde, distAtual);
					subtBalde = 0;
				}	
			}
			
			//ANALISA SE POSIÇAO ATUAL É TORNEIRA
			if (espAtual == torneira && (cd.b+1) <= b){ //se for torneira e todos os baldes nao estiverem cheios				
				AnalisaPosicao(predio, &queue, cd.z, cd.y, cd.x, cd.b+1, distAtual);
			}					
			
			//ANALISA SE POSIÇAO ATUAL É ESCADA PARA CIMA
			if (espAtual == escCima && (cd.z+1) < a){ //se for escada pra cima e nao ultrapassar o limite de andares				
				AnalisaPosicao(predio, &queue, cd.z+1, cd.y, cd.x, cd.b, distAtual);
			}
			
			//ANALISA SE POSIÇAO ATUAL É ESCADA PARA BAIXO
			if (espAtual == escBaixo && (cd.z-1) >= 0){ //se for escada pra baixo e nao ultrapassar o limite de andares
				AnalisaPosicao(predio, &queue, cd.z-1, cd.y, cd.x, cd.b, distAtual);
			}
			
			//ANALISA SE POSIÇAO ATUAL É ESCADA BIDIRECIONAL
			if (espAtual == escCiBa){ 
				if ((cd.z+1) < a){
					AnalisaPosicao(predio, &queue, cd.z+1, cd.y, cd.x, cd.b, distAtual);
				}	
				if ((cd.z-1) >= 0){
					AnalisaPosicao(predio, &queue, cd.z-1, cd.y, cd.x, cd.b, distAtual);
				}			
			}										
	}	
	printf("%d\n", predio[final.z][final.y][final.x][0].dist);	
return 0;
}
