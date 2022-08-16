#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <stdio.h>
#include <stdlib.h>
#include "heap.h"
#include "freq_table.h"
#include "code_table.h"
#include "bitmap.h"

/*
ESSA BIBLIOTECA CONTÉM FUNCOES QUE NAO CABEM A UM TIPO ESPECIFICO
PORÉM PARA A TAREFA DE CODIFICAÇÃO E DECODIFICAÇÃO, SÃO NECESSÁRIAS
*/

/* 
'enche' a heap com a tabela de frequencias formada pela leitura de um arquivo alvo
*/
void fill_heap_with_freq_table(binary_heap* b, Freq_Table* f_tbl);

/*
contrutor da arvore de huffman, utiliza-se uma heap ordenada para tal feito
*/
tree* huffman_tree_constructor(binary_heap* b);

/*
'enche' a tabela de códigos usando uma árvore
*/
void fill_code_table(Code_Table* c_tbl, tree* a );


/*
função utilizada para quando se deseja unzipar(descompactar) um arquivo '.comp'
*/
void unzip(char ** argv );

/*
função utilizada para quando se deseja zipar(compactar) um arquivo com qualquer terminação desejada
*/
void zip(char ** argv);

/*
executa as funcao de unzipar ou zipar dependendo dos argumentos passados pelo terminal do cliente
*/
int execute(int argc, char* argv[]);

#endif 