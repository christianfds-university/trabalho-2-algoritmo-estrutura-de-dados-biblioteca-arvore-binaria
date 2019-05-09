#ifndef FUNCARQ_H
#define FUNCARQ_H
#include "Structs.h"

//Função responsável por substituir os \n por \0 de uma string
//Entrada: Ponteiro pra cabeça de uma string
//Retorno: void
void stripchar(char *s);

//Função responsável por ler uma string referente a um livro
//Entrada: Ponteiro para a string, Ponteiro para o livro onde as informações devem ser salvas
//Retorno: void
void lerLivro(char *buffer, livro *l);

//Função responsável por imprimir no console as informações de um livro
//Entrada: Ponteiro para o livro
//Retorno: void
void imprimeLivro(livro *l);

//Função responsável por escrever um novo cabeçalho no arquivo
//Entrada: Ponteiro para o arquivo, ponteiro para o cabeçalho
//Retorno: void
void escreveCabecalho(FILE* arq, cabecalho* cab);

//Função responsável por escrever um novo nó no arquivo
//Entrada: Ponteiro para o arquivo, ponteiro para o no, inteiro referente a posição
//Retorno: void
void escreveNo(FILE* arq, no* x, int pos);

//Função responsável por criar um cabeçalho vazio
//Entrada: Ponteiro para o arquivo
//Retorno: void
void cria_lista_vazia(FILE* arq);

//Função responsável por ler o cabeçalho de um arquivo binário
//Entrada: Ponteiro para o arquivo
//Retorno: Ponteiro para o cabeçalho lido
cabecalho* lerCab(FILE *arq);

//Função responsável por gerar um novo nó a partir das informações de um livro
//Entrada: Ponteiro para o livro
//Retorno: Ponteiro para o nó criado
no *novoNo(livro *l);

//Função responsável por ler um nó do arquivo
//Entrada: Ponteiro para o arquivo, inteiro referente a posição do no
//Retorno: Ponteiro para o nó lido
no* leNo(FILE* arq, int pos);

//Função responsável por inserir novos livros no arquivo binário
//Entrada: Ponteiro para o livro
//Retorno: void
void insereLivro(FILE *f, livro *l);

//Função responsável por ler o arquivo de inicialização
//Entrada: void
//Retorno: void
void lerArq();

//**************************************************************************************************
//******************************code by: Narue || from DANIWEB**************************************
//**************************************************************************************************
//Código responsável por imprimir recursivamente a arvore
void padding ( char ch, int n );

void structure (FILE *f, int pos, int level );
//**************************************************************************************************
//**************************************************************************************************

//Função auxiliar responsável por imprimir recursivamente uma arvore binária
//Entrada: void
//Retorno: void
void imprimeArvBinAux(FILE *f, int i);

//Função responsável por imprimir uma arvore binária
//Entrada: void
//Retorno: void
void imprimeArvBin1();

//Função responsável por imprimir uma arvore binária
//Entrada: void
//Retorno: void
void imprimeArvBin2();

//Função que realiza uma busca pelo codigo de um livro no arquivo de dados
//Entrada: código referente ao livro
//Retorno: posição do livro no arquivo, ou -1 caso não exista
int posLivro(int code);

//Função que realiza a busca pelas informações de um livro
//Entrada: código referente ao livro buscado
//Retorno: void
void buscaLivro();


//Função que insere um novo livro a partir do menu
//Entrada: void
//Retorno: void
void inserirNovoLivro();

//Função que devolve o menor entre os maiores elementos
//Entrada: No raiz a ser verificado
//Retorno: Posição do menor entre os maiores
int menorEntreMaiores(FILE *f, int pos);

//Função que devolve o maior entre os menores elementos
//Entrada: No raiz a ser verificado
//Retorno: Posição do maior entre os menores
int maiorEntreMenores(FILE *f, int pos);

//Função que procura uma posição vazia do arquivo
//Entrada: Arquivo
//Retorno: Posição do primeiro registro vazio
int posVazia(FILE *f);

//Copia o conteúdo do Nó2 para o Nó1
//Entrada: File, Nó 1 e Nó 2
//Retorno: void
void copiaCont(FILE *f, no *No1, no *No2);

// Função auxiliar para remover um elemento do arquivo
// Entrada: File, Posição na árvore, Elemento a ser removido
// Retorno: Pos
int removeLivroAux(FILE *f, cabecalho *cab, int pos, int x);

//Função que remove um elemento do arquivo
//Entrada: void
//Retorno: void
void removeLivro();

//Função auxiliar que imprime recursivamente no console o acervo de livros ordenados pelo código
//Entrada: File e a posição a ser impressa
//Retorno: void
void imprimirAcervoAux(FILE *f, int pos);

//Função que imprime no console o acervo de livros ordenados pelo código
//Entrada: void
//Retorno: void
void imprimirAcervo();

//Função que conta recursivamente a quantidade de nós da árvore
//Entrada: File , posição do nó inicial
//Retorno: inteiro referente ao número de nós
int contaNos(FILE *f, int pos);

//Função auxiliar que carrega pra memória um vetor de structs
//Entrada: File, ponteiro para o vetor de livros, posição a ser lida, valor de controle da posição no vetor
//Retorno: int
int carregaVetAux(FILE *f, livro *vet, int pos, int i);

//Função que carrega pra memória um vetor de structs
//Entrada: File, ponteiro para o vetor de livros, quantidade de nós da árvore
//Retorno: void
void *carregaVet(FILE *f, livro *vet, int n);

//Função que copia as informações de l2 para l1;
//Entrada: ponteiro para o livro 1 e ponteiro para o livro 2 
//Retorno: void
void copiaLivro(livro *l1, livro *l2);

//Função auxiliar do MergeSort
void Intercala(livro vet[],int e, int m, int d);

//Função responsável por realizar o mergesort de um vetor de livros, com base em seu titulo
//Entrada: Ponteiro para vetor de livros, e = 0, Quantidade de livros
//Retorno: void
void MergeSort(livro vet[],int e, int d);

//Função auxiliar do quicksort
int Separa(livro *vet,int e, int d);

//Função responsável por realizar o quicksort de um vetor de livros, com base em seu autor
//Entrada: Ponteiro para vetor de livros, e = 0, Quantidade de livros - 1
//Retorno: void
void QuickSort(livro vet[],int e, int d);

//Função responsável por gerar um arquivo texto com os elementos de um vetor de livros
//Entrada: Ponteiro para o vetor de livros, quantidade de livros, nome do arquivo
//Retorno: void
void geraArqTxt(livro *vet, int n, char *nome);

//Função que gera um arquivo de texto com os livros da árvore ordenados pelo seu título
//Entrada: void
//Retorno: void
void gerarAcervoTitulo();

//Função que gera um arquivo de texto com os livros da árvore ordenados pelo seu autor
//Entrada: void
//Retorno: void
void gerarAcervoAutor();

#endif