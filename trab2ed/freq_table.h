#ifndef FREQ_TABLE
#define FRAQ_TABLE

#include <stdio.h>
#include <stdlib.h>

typedef struct freq_table Freq_Table;

/*inicia uma tabela de frequencia em que cada posição do vetor tem-se a frequencia correspondida ao caractere de mesmo valor que o index do vetor em ASCII*/
Freq_Table* init_freq_table(void);

/*de acordo com o indice passado , acrescenta-se um no valor da fequencia do caractere em que aquele index corresponde*/
void insert_freq_table(Freq_Table* f_tbl, int i );

/*retorna a frequencia de um determinado caractere representado pelo mesmo valor do index acessado em ASCII*/
long long unsigned int get_freq_table(Freq_Table* f_tbl, int i);

/*faz a liberação de memória da tabela de frequecia*/
void free_freq_table(Freq_Table* f_tble);

/*imprime a tabela de frequencia, usada para depuração do código*/
void show_freq_table(Freq_Table* f_tbl);

/*por meio de arquivo de entrada, preenche-se a tabela de frequencia com as frequencias dos bytes presentes no arquivo*/
void fread_freq_table(Freq_Table* f_table, FILE * file);

#endif