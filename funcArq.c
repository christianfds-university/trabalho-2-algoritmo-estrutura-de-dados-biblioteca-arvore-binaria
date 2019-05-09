#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Structs.h"

//Função responsável por substituir os \n por \0 de uma string
//Entrada: Ponteiro pra cabeça de uma string
//Retorno: void
void stripchar(char *s){
	while(*s){
		if(*s=='\n')
			*s = '\0';
		s++;
	}
}

//Função responsável por ler uma string referente a um livro
//Entrada: Ponteiro para a string, Ponteiro para o livro onde as informações devem ser salvas
//Retorno: void
void lerLivro(char *buffer, livro *l){
	sscanf(buffer,"%d", &l->code);
	stripchar(buffer);

	char *c = buffer;
	
	while(*c){ //Substitui todas as virgulas do buffer por \0 para que a função strcpy funcione corretamente
		if(*c==',')
			*c = '\0';
		c++;
	}

	c = buffer;

	while(*c!='\0')
		c++;
	c+=2; // c passa a apontar para a primeira letra do titulo do livro
	strcpy(l->titulo,c);
	
	while(*c!='\0')
		c++;
	c+=2; // c passa a apontar para a primeira letra do nome do autor
	strcpy(l->autor,c);
}

//Função responsável por imprimir no console as informações de um livro
//Entrada: Ponteiro para o livro
//Retorno: void
void imprimeLivro(livro *l){
	printf("========================================\n");
	printf("%-11s %02d\n","Código:"		,l->code);
	printf("%-11s %d\n","Prateleira:"	,l->prateleira);
	printf("%-11s %d\n","Estante:"		,l->estante);
	printf("%-11s %s\n","Titulo:"		,l->titulo);
	printf("%-11s %s\n","Autor:"		,l->autor);
	printf("========================================\n");
}

//Função responsável por escrever um novo cabeçalho no arquivo
//Entrada: Ponteiro para o arquivo, ponteiro para o cabeçalho
//Retorno: void
void escreveCabecalho(FILE* arq, cabecalho* cab){
	fseek(arq,0,SEEK_SET); //posiciona no início do arquivo
	fwrite(cab,sizeof(cabecalho),1,arq);
}

//Função responsável por escrever um novo nó no arquivo
//Entrada: Ponteiro para o arquivo, ponteiro para o no, inteiro referente a posição
//Retorno: void
void escreveNo(FILE* arq, no* x, int pos){
	fseek(arq,sizeof(cabecalho) + pos*sizeof(no),SEEK_SET);
	fwrite(x,sizeof(no),1,arq);
}

//Função responsável por criar um cabeçalho vazio
//Entrada: Ponteiro para o arquivo
//Retorno: void
void cria_lista_vazia(FILE* arq){
	cabecalho * cab = (cabecalho*) malloc(sizeof(cabecalho));
	cab->pos_cabeca = -1;
	cab->pos_topo = 0;
	cab->pos_livre = -1;
	escreveCabecalho(arq,cab);
	free(cab);
 }

//Função responsável por ler o cabeçalho de um arquivo binário
//Entrada: Ponteiro para o arquivo
//Retorno: Ponteiro para o cabeçalho lido
cabecalho* lerCab(FILE *arq) {
	cabecalho * cab = (cabecalho*) malloc(sizeof(cabecalho));
	fseek(arq,0,SEEK_SET); // posiciona no início do arquivo
	fread(cab,sizeof(cabecalho),1,arq);
	return cab;
}

//Função responsável por gerar um novo nó a partir das informações de um livro
//Entrada: Ponteiro para o livro
//Retorno: Ponteiro para o nó criado
no *novoNo(livro *l){
	no *n = (no *) malloc(sizeof(no));

	n->liv.code = l->code;
	n->liv.estante = l->estante;
	n->liv.prateleira = l->prateleira;
	strcpy(n->liv.autor, l->autor);
	strcpy(n->liv.titulo, l->titulo);

	n->l = -1;
	n->r = -1;

	return n;
}

//Função responsável por ler um nó do arquivo
//Entrada: Ponteiro para o arquivo, inteiro referente a posição do no
//Retorno: Ponteiro para o nó lido
no* leNo(FILE* arq, int pos) {
	no* x = malloc(sizeof(no));
	fseek(arq,sizeof(cabecalho)+ pos*sizeof(no),SEEK_SET);
	fread(x,sizeof(no),1,arq);
	return x;
}

//Função responsável por inserir novos livros no arquivo binário
//Entrada: Ponteiro para o livro
//Retorno: void
void insereLivro(FILE *f, livro *l){
	no *No = novoNo(l); //Novo nó a ser inserido
	cabecalho *cab = lerCab(f); //Cabeçalho do arquivo
	int posInserida;	//Variavel responsável por guardar onde o novo nó foi inserido para 
						//atualizar para onde o pai do novo nó deve apontar

	if(cab->pos_cabeca == -1){ //Verifica se é a primeira informação a ser adicionado ao arquivo binário
		escreveNo(f, No, ++cab->pos_cabeca);
		posInserida = cab->pos_cabeca;
		cab->pos_topo++;
	}
	else if(cab->pos_livre == -1) { //Caso não existe posição livre, insere no final do arquivo
		escreveNo(f, No, cab->pos_topo);
		posInserida = cab->pos_topo;
		cab->pos_topo++;
	}
	else { //Caso exista posição livre, insere o no na primeira posição livre 
		no* aux = leNo(f, cab->pos_livre);
		escreveNo(f, No, cab->pos_livre);
		posInserida = cab->pos_livre;
		cab->pos_livre = aux->r; //A lista de posições livres ficam salvas nos elementos r do nó
		free(aux);
	}

	//Atualiza as informações do cabeçalho
	escreveCabecalho(f, cab); 

	no *pai;
	int posPai = cab->pos_cabeca;
	int posPaiNext = cab->pos_cabeca;
	pai = leNo(f,posPaiNext);
	
	do{ //Loop responsável por percorrer a arvore e encontrar a posição do nó pai do nó inserido recentemente
		free(pai); //Evita memory leak	
		pai = leNo(f,posPaiNext);
		posPai = posPaiNext;
		if(l->code >= pai->liv.code)
			posPaiNext = pai->r;
		else
			posPaiNext = pai->l;

	} while(posPaiNext != -1);

	if(posInserida!= cab->pos_cabeca){ //Verifica se não é o primeiro elemento inserido
		if(l->code >= pai->liv.code)
			pai->r = posInserida;
		else
			pai->l = posInserida;

		escreveNo(f,pai,posPai); //Atualiza o valor de l ou r do nó pai
	}


	free(No);
	free(pai);
	free(cab);
}

//Função responsável por ler o arquivo de inicialização
//Entrada: void
//Retorno: void
void lerArq(){
	char arqEntrada[100];
	FILE *fIn;

	printf("Arquivo de entrada:");


	scanf("%s%*c",arqEntrada); //Le o nome do arquivo de entrada do console
	
	if(!(fIn = fopen(arqEntrada,"r"))){ //Verifica se foi possível abrir o arquivo
		printf("Nome de arquivo inválido\n");
		return;
	}

	char buffer[255];
	int e, p; //Guardam os valores referentes a estante e a prateleira 
	livro *l = (livro *) malloc(sizeof(livro)); //Estrutura basica de um livro
	FILE *f = fopen("data.bin", "w+b"); //Arquivo onde serão salvas as informações em binário
	cria_lista_vazia(f);

	while(fgets(buffer,sizeof(buffer),fIn)){ //Em quanto for possível ler o arquivo
		
		if(buffer[0] == 'E'){
			sscanf((buffer+1), "%d", &e);
		}
		else if(buffer[0] == 'P'){
			sscanf((buffer+1), "%d", &p);
		}
		else{
			l->estante = e;
			l->prateleira = p;
			lerLivro(buffer, l);
			insereLivro(f,l);
		}
	}

	fclose(f);
	fclose(fIn);
	free(l);
	printf("Arquivo \" %s \" lido com sucesso\n", arqEntrada);
}

//**************************************************************************************************
//******************************code by: Narue || from DANIWEB**************************************
//**************************************************************************************************
//Código responsável por imprimir recursivamente a arvore
void padding ( char ch, int n ){
	int i;
	for ( i = 0; i < n; i++ )
		putchar ( ch );
}

void structure (FILE *f, int pos, int level ){

	no *No;
	

	if ( pos == -1 ) {
		padding ( '\t', level );
		puts ( "NULL" );
	}
	else {
		No = leNo(f,pos);
		structure (f, No->r, level + 1 );
		padding ( '\t', level );
		printf ( "%d\n", No->liv.code );
		structure (f, No->l, level + 1 );
		free(No);
	}
}
//**************************************************************************************************
//**************************************************************************************************

//Função auxiliar responsável por imprimir recursivamente uma arvore binária
//Entrada: void
//Retorno: void
void imprimeArvBinAux(FILE *f, int i){
	if(i != -1){
		no *No = leNo(f,i);
		printf("[%d,", No->liv.code);
		
		if(No->l != -1){ //Verifica se o nó existe, ou é NULL
			imprimeArvBinAux(f,No->l);
		}
		else
			printf("[]");

		printf(",");

		if(No->r != -1){ //Verifica se o nó existe, ou é NULL
			imprimeArvBinAux(f,No->r);
		}
		else
			printf("[]");

		printf("]");

		free(No);
	}
}

//Função responsável por imprimir uma arvore binária
//Entrada: void
//Retorno: void
void imprimeArvBin1(){
	FILE *f;
	if(f = fopen("data.bin", "rb")){ //Arquivo com as informações em binário
		cabecalho *cab = lerCab(f); //Cabeçalho do arquivo
		system("clear");
		imprimeArvBinAux(f,cab->pos_cabeca);
		fclose(f);
	}
	else
		printf("Arquivo de dados não encontrado\n");
}

//Função responsável por imprimir uma arvore binária
//Entrada: void
//Retorno: void
void imprimeArvBin2(){
	FILE *f;
	if(f = fopen("data.bin", "rb")){ //Arquivo com as informações em binário
		cabecalho *cab = lerCab(f); //Cabeçalho do arquivo
		system("clear");
		structure(f,cab->pos_cabeca,0);
		fclose(f);
	}
	else
		printf("Arquivo de dados não encontrado\n");
}


//Função que realiza uma busca pelo codigo de um livro no arquivo de dados
//Entrada: código referente ao livro
//Retorno: posição do livro no arquivo, ou -1 caso não exista
int posLivro(int code){
	FILE *f = fopen("data.bin", "rb"); //Arquivo com as informações em binário
	cabecalho *cab = lerCab(f); //Cabeçalho do arquivo

	no *pai;
	int posPai;
	
	posPai = cab->pos_cabeca;
	pai = leNo(f,posPai);

	do{ //Loop responsável por percorrer a arvore e encontrar o livro com o código inserido
		free(pai); //Evita memory leak
		pai = leNo(f,posPai);
	
		if(code > pai->liv.code)
			posPai = pai->r;
		else if(code < pai->liv.code)
			posPai = pai->l;
		else
			break;

	} while((posPai != -1));


	free(pai);

	return posPai;
}

//Função que realiza a busca pelas informações de um livro
//Entrada: código referente ao livro buscado
//Retorno: void
void buscaLivro(){

	int i;

	FILE *f;
	if(f = fopen("data.bin", "rb")){
		printf("Código do livro: ");
		scanf("%d%*c", &i);

		int pos = posLivro(i); //Busca pelo livro dentro do arquivo

		if(pos == -1){ //Verifica a existência do livro
			printf("Livro inexistente\n");
			return;
		}


		no *No = leNo(f,pos);

		imprimeLivro(&No->liv); //Imprime as informações do livro

		free(No);
		fclose(f);
	}
	else
		printf("Arquivo de dados não encontrado\n");

}


//Função que insere um novo livro a partir do menu
//Entrada: void
//Retorno: void
void inserirNovoLivro(){
	
	livro l;
	char aux[100];
	FILE *f;
	if(f = fopen("data.bin", "r+b")){

		printf("Código do livro: ");
		scanf("%d%*c", &l.code);
		
		if(posLivro(l.code)!=-1){
			printf("Livro com a id %d ja existe na biblioteca\n", l.code);
			return;
		}

		printf("Estante: ");
		scanf("%d%*c", &l.estante);
		
		printf("Prateleira: ");
		scanf("%d%*c", &l.prateleira);
		
		printf("Titulo do Livro: ");
		fgets(aux, 100, stdin);
		stripchar(aux);
		strcpy(l.titulo,aux);

		printf("Nome do Autor: ");
		fgets(aux, 100, stdin);
		stripchar(aux);
		strcpy(l.autor,aux);


		insereLivro(f,&l);
		fclose(f);

		printf("Livro \" %s \" inserido com sucesso\n", l.titulo);
	}
	else
		printf("Arquivo de dados não encontrado\n");

}

//Função que devolve o menor entre os maiores elementos
//Entrada: No raiz a ser verificado
//Retorno: Posição do menor entre os maiores
int menorEntreMaiores(FILE *f, int pos){
	no *No = leNo(f,pos);
	int posprox;
	pos = posprox = No->r;

	while(posprox != -1){
		free(No);
		no *No = leNo(f,posprox);
		pos = posprox;
		posprox = No->l;
	}
	free(No);
	
	return pos;
}

//Função que devolve o maior entre os menores elementos
//Entrada: No raiz a ser verificado
//Retorno: Posição do maior entre os menores
int maiorEntreMenores(FILE *f, int pos){
	no *No = leNo(f,pos);
	int posprox;
	pos = posprox = No->l;

	while(posprox != -1){
		free(No);
		no *No = leNo(f,posprox);
		pos = posprox;
		posprox = No->r;
	}
	free(No);

	return pos;
}

//Função que procura uma posição vazia do arquivo
//Entrada: Arquivo
//Retorno: Posição do primeiro registro vazio
int posVazia(FILE *f){
	cabecalho *cab = lerCab(f);
	int pos = cab->pos_livre;
	free(cab);
	return pos;
}

//Copia o conteúdo do Nó2 para o Nó1
//Entrada: File, Nó 1 e Nó 2
//Retorno: void
void copiaCont(FILE *f, no *No1, no *No2){
	
	int pos1 = posLivro(No1->liv.code);

	escreveNo(f,No2,pos1);
}

// Função auxiliar para remover um elemento do arquivo
// Entrada: File, Posição na árvore, Elemento a ser removido
// Retorno: Pos
int removeLivroAux(FILE *f, cabecalho *cab, int pos, int x){
	if(pos == -1) return -1; //Verifica a existência de um elemento para remover

	no *No = leNo(f,pos);

	if(x < No->liv.code){ //Elemento está a esquerda
		No->l = removeLivroAux(f,cab,No->l,x);
	}
	else
		if(x > No->liv.code){ //Elemento está a direita
			No->r = removeLivroAux(f,cab,No->r,x);
		}
		else // Elemento encontrado
			if((No->l == -1) && (No->r == -1)){ //Elemento folha
					No->r = cab->pos_livre; //Por convenção r é onde fica salvo a lista de nós livres
					cab->pos_livre = pos;
					escreveNo(f,No,pos);
					free(No);
					return -1;
				}
				else if(No->l == -1){ //Sem filhos a esquerda
					no *NoAux = leNo(f,menorEntreMaiores(f,pos));
					copiaLivro((No->liv), (NoAux->liv));
					No->r = removeLivroAux(f,cab,No->r,NoAux->liv.code);
					free(NoAux);
				}
				else{ //Sem filhos a direita ou com filhos para ambos os lados
					no *NoAux = leNo(f,maiorEntreMenores(f,pos));
					copiaLivro((No->liv), (NoAux->liv));
					No->l = removeLivroAux(f,cab,No->l,NoAux->liv.code);
					free(NoAux);
				}

	escreveNo(f,No,pos);
	free(No);

	return pos;
}


//Função que remove um elemento do arquivo
//Entrada: void
//Retorno: void
void removeLivro(){
	int i;

	FILE *f;
	if(f = fopen("data.bin", "r+b")){
		printf("Código do livro a ser removido\n");
		scanf("%d%*c", &i);

		int pos = posLivro(i); //Posição do nó a ser removido
		
		if(pos==-1){
			fclose(f);
			printf("Livro inexistente\n");
			return;
		}

		cabecalho *cab = lerCab(f); //Cabeçalho que será alterado 
		removeLivroAux(f, cab, cab->pos_cabeca, i);
		escreveCabecalho(f,cab);

		free(cab);
		fclose(f);
	}
	else
		printf("Arquivo de dados não encontrado\n");

}


//Função auxiliar que imprime recursivamente no console o acervo de livros ordenados pelo código
//Entrada: File e a posição a ser impressa
//Retorno: void
void imprimirAcervoAux(FILE *f, int pos){

	if(pos != -1){
		no *No = leNo(f,pos);
		imprimirAcervoAux(f, No->l);
		imprimeLivro(&No->liv);
		imprimirAcervoAux(f, No->r);
		free(No);
	}

}

//Função que imprime no console o acervo de livros ordenados pelo código
//Entrada: void
//Retorno: void
void imprimirAcervo(){
	FILE *f;
	if(f = fopen("data.bin", "rb")){
		cabecalho *cab = lerCab(f);

		system("clear");
		imprimirAcervoAux(f,cab->pos_cabeca);

		fclose(f);
		free(cab);
	}
	else
		printf("Arquivo de dados não encontrado\n");

}

//Função que conta recursivamente a quantidade de nós da árvore
//Entrada: File , posição do nó inicial
//Retorno: inteiro referente ao número de nós
int contaNos(FILE *f, int pos){
	if(pos == -1)
		return 0;
	no *No = leNo(f, pos);
	
	int lNos, rNos;

	lNos = contaNos(f,No->l);
	rNos = contaNos(f,No->r);

	free(No);

	return 1 + lNos + rNos;
}

//Função auxiliar que carrega pra memória um vetor de structs
//Entrada: File, ponteiro para o vetor de livros, posição a ser lida, valor de controle da posição no vetor
//Retorno: int
int carregaVetAux(FILE *f, livro *vet, int pos, int i){
	
	if(pos == -1) //Verifica se a árvore é vazia
		return 0;
	
	no *No = leNo(f, pos);

	//Copia as informações do nó para o vetor
	vet[i].code = No->liv.code;
	vet[i].estante = No->liv.estante;
	vet[i].prateleira = No->liv.prateleira;
	strcpy(vet[i].titulo,No->liv.titulo);
	strcpy(vet[i].autor,No->liv.autor);

	if(No->l != -1) //Verifica se o no a direita existe
		i = carregaVetAux(f,vet,No->l,++i);
	if(No->r != -1) //Verifica se o no a esquerda existe
		i = carregaVetAux(f,vet,No->r, ++i);

	free(No);

	return i;
}

//Função que carrega pra memória um vetor de structs
//Entrada: File, ponteiro para o vetor de livros, quantidade de nós da árvore
//Retorno: void
void *carregaVet(FILE *f, livro *vet, int n){
	cabecalho *cab = lerCab(f);

	carregaVetAux(f,vet,cab->pos_cabeca, 0); //Carrega as informações do arquivo binário para o vetor
	
	free(cab);
}


//Função que copia as informações de l2 para l1;
//Entrada: ponteiro para o livro 1 e ponteiro para o livro 2 
//Retorno: void
void copiaLivro(livro *l1, livro *l2){
	l1->code = l2->code;
	l1->estante = l2->estante;
	l1->prateleira = l2->prateleira;
	strcpy(l1->titulo,l2->titulo);
	strcpy(l1->autor,l2->autor);
}


//Função auxiliar do MergeSort
void Intercala(livro vet[],int e, int m, int d)
{
    int i, j, k;
    livro *vet_aux;
    vet_aux = malloc((d-e)*sizeof(livro));
    i = e;
    j = m;
    k = 0;
    while( i < m && j < d)
    {
        if(strcmp(vet[i].titulo, vet[j].titulo) <= 0)
        {
            copiaLivro(&vet_aux[k++], &vet[i++]);
        }
        else copiaLivro(&vet_aux[k++], &vet[j++]);
    }
    while (i < m)
        copiaLivro(&vet_aux[k++], &vet[i++]);
    while (j < d)
        copiaLivro(&vet_aux[k++], &vet[j++]);
    for(i = e; i < d; i++)
        copiaLivro(&vet[i], &vet_aux[i-e]);

    free(vet_aux);
}

//Função responsável por realizar o mergesort de um vetor de livros, com base em seu titulo
//Entrada: Ponteiro para vetor de livros, e = 0, Quantidade de livros
//Retorno: void
void MergeSort(livro vet[],int e, int d)
{
    int m;
    if( e < d-1)
    {
        int m = (e+d)/2;
        MergeSort(vet,e,m);
        MergeSort(vet,m,d);
        Intercala(vet,e,m,d);
    }
}

//Função auxiliar do quicksort
int Separa(livro *vet,int e, int d)
{
    int ax = 0;
    int j = 0,k = 0;
    livro pivo, temp;

    copiaLivro(&pivo,&vet[d]);
    
    j = e;
    for( k = e; k < d; k++){
        if(strcmp(vet[k].autor, pivo.autor) <= 0)
        {
        	copiaLivro(&temp,&vet[j]);
        	copiaLivro(&vet[j],&vet[k]);
        	copiaLivro(&vet[k],&temp);
            j++;
        }
    }

    copiaLivro(&vet[d],&vet[j]);
    copiaLivro(&vet[j],&pivo);

    return j;
}

//Função responsável por realizar o quicksort de um vetor de livros, com base em seu autor
//Entrada: Ponteiro para vetor de livros, e = 0, Quantidade de livros - 1
//Retorno: void
void QuickSort(livro vet[],int e, int d) {
    int j;
    if( e < d){

        j = Separa(vet,e,d);
        QuickSort(vet,e,j-1);
        QuickSort(vet,j+1,d);

    }
}

//Função responsável por gerar um arquivo texto com os elementos de um vetor de livros
//Entrada: Ponteiro para o vetor de livros, quantidade de livros, nome do arquivo
//Retorno: void
void geraArqTxt(livro *vet, int n, char *nome){
	FILE *f = fopen(nome, "w");
	int i;
	for(i = 0; i < n; i++){
		fprintf(f, "Código....: %d\n"  , vet[i].code );
		fprintf(f, "Estante...: %d\n"  , vet[i].estante );
		fprintf(f, "Prateleira: %d\n"  , vet[i].prateleira );
		fprintf(f, "Titulo....: %s\n"  , vet[i].titulo );
		fprintf(f, "Autor.....: %s\n\n", vet[i].autor );
	}
	fclose(f);
}

//Função que gera um arquivo de texto com os livros da árvore ordenados pelo seu título
//Entrada: void
//Retorno: void
void gerarAcervoTitulo(){
	FILE *f;
	if(f = fopen("data.bin", "rb")){
		cabecalho *cab = lerCab(f);

		int n = contaNos(f,cab->pos_cabeca); //Tamanho do vetor

		livro *vet = (livro *) malloc(sizeof(livro)*n); //Aloca o vetor de nós
		carregaVet(f,vet,n);

		fclose(f);
		free(cab);

		int i,j;

		MergeSort(vet,0,n);
			
		geraArqTxt(vet,n,"AcervoOrdenadoTitulo.txt");
		printf("Arquivo \"AcervoOrdenadoTitulo.txt\" gerado com sucesso\n");

		free(vet);
	}
	else
		printf("Arquivo de dados não encontrado\n");

}

//Função que gera um arquivo de texto com os livros da árvore ordenados pelo seu autor
//Entrada: void
//Retorno: void
void gerarAcervoAutor(){
	FILE *f;
	if(f = fopen("data.bin", "rb")){
		cabecalho *cab = lerCab(f);

		int n = contaNos(f,cab->pos_cabeca); //Tamanho do vetor

		livro *vet = (livro *) malloc(sizeof(livro)*n); //Aloca o vetor de nós
		carregaVet(f,vet,n);

		fclose(f);
		free(cab);

		int i,j;

		QuickSort(vet,0,n-1);
			
		geraArqTxt(vet,n,"AcervoOrdenadoAutor.txt");
		printf("Arquivo \"AcervoOrdenadoAutor.txt\" gerado com sucesso\n");

		free(vet);
	}
	else
		printf("Arquivo de dados não encontrado\n");
}
