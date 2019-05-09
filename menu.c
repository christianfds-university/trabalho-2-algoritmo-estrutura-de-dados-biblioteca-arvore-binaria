#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "Structs.h"
#include "funcArq.h"

void separator(){
	printf("||========================================||\n");
}

void menu(){

	int op = 0;

	while(op!=-1){
		system("clear"); 
		separator(); 
		printf("||(%2d) %-35s|| \n",  1,  "Carregar Arquivo de Inicializacao"); 
		separator(); 
		printf("||(%2d) %-35s|| \n",  2,  "Inserir Livro"); 
		printf("||(%2d) %-35s|| \n",  3,  "Retirar Livro"); 
		printf("||(%2d) %-35s|| \n",  4,  "Buscar Dados do Livro"); 
		separator(); 
		printf("||(%2d) %-35s|| \n",  5,  "Imprimir Acervo"); 
		printf("||(%2d) %-35s|| \n",  6,  "Imprimir Arvore Binaria 1"); 
		printf("||(%2d) %-35s|| \n",  7,  "Imprimir Arvore Binaria 2"); 
		separator(); 
		printf("||(%2d) %-35s|| \n",  8,  "Gerar Acervo ordenado por Titulo"); 
		printf("||(%2d) %-35s|| \n",  9,  "Gerar Acervo ordenado por Autor"); 
		separator(); 
		printf("||(%2d) %-35s|| \n", -1,  "Sair"); 
		separator();
		scanf("%d", &op);
		getchar();

		switch(op){
		case 1:
			lerArq();
			getchar();
		break;
		
		case 2:
			inserirNovoLivro();
			getchar();
		break;
		
		case 3:
			removeLivro();
			getchar();
		break;
		
		case 4:
			buscaLivro();
			getchar();
		break;
		
		case 5:
			imprimirAcervo();
			getchar();
		break;
		
		case 6:
			imprimeArvBin1();
			getchar();
		break;
		
		case 7:
			imprimeArvBin2();
			getchar();
		break;
		
		case 8:
			gerarAcervoTitulo();
			getchar();
		break;

		case 9:
			gerarAcervoAutor();
			getchar();
		break;

		}
	}
}
