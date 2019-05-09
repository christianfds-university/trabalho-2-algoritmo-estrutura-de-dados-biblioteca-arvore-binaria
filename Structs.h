#ifndef STRUCTS_H
#define STRUCTS_H
//Estrutura referente ao cabeçalho do arquivo
typedef struct {
	int pos_cabeca;	//Inicio da Lista
	int pos_topo;	//Posição livre no final do arquivo
	int pos_livre;	//Lista de nós livres
} cabecalho;

//Estrutura de informações de um livro
typedef struct{
	int  code;			//Código de identificação do livro
	int  estante;		//Endereço fisico da Estante
	int  prateleira;	//Endereço fisico da Prateleira
	char titulo[100];	//Titulo do livro
	char autor[100];	//Autor do livro
} livro;

//Estrutura de nós
typedef struct{
	livro liv;	//Informações sobre o livro
	int l;		//Posição do elemento a esquerda na arvore
	int r;		//Posição do elemento a direita na arvore
	//A lista de nós livres será salva nos elementos r dos nós que não são mais utilizados
} no;

#endif