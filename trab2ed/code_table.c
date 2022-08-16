#include "code_table.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>



#define ASCII 256

typedef struct code Code;

struct code {
    char * string;
};


struct code_table{
    Code code_arr[256];
};

Code_Table* init_code_table(void){
    Code_Table* c_table= (Code_Table*)calloc(1,sizeof(Code_Table));
    for(int i =0; i< ASCII; i ++){
        c_table->code_arr[i].string = NULL;
    }
    return c_table;
}

/*
cria uma string de mesma strutura e insere na posição desejada
*/

void insert_code_table(Code_Table* c_table, char* string , unsigned char index){
    if(!c_table) return;
    c_table->code_arr[(unsigned int )index].string = strdup(string);
}

/*
libera as strings(códigos) e a tabela de fequencia
*/

void free_code_table(Code_Table* c_table){
    if(!c_table) return;
    for(int i =0; i < ASCII; i ++){
        if(c_table->code_arr[i].string != NULL) free(c_table->code_arr[i].string);

    }
    free(c_table);
}


/*
retorna o valor da posição desejada
*/
char * get_code_table(Code_Table* c_table, int index){
    return c_table->code_arr[index].string;
}


/*
imprime o caractere e seu código correspondente
*/

void show_code_table(Code_Table* c_table){
    if(!c_table) return;
    for(int i =0; i < ASCII; i ++){
        if(c_table->code_arr[i].string != NULL) printf("%c : %s\n", (char)i ,c_table->code_arr[i].string);
    }
    return;
}