#include "freq_table.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define ASCII 256

#define forn(i , n) for( int i =0; i < n; i++)

struct freq_table{
    unsigned long long int freq_vector[ASCII];
};

Freq_Table* init_freq_table(void){
    Freq_Table* f_tbl = (Freq_Table*)calloc(1,sizeof(Freq_Table));

    return f_tbl;
}

/*
aumenta a frequencia na posição de inserção 
*/
void insert_freq_table(Freq_Table* f_tbl, int i){
    f_tbl->freq_vector[i]++;
    return ;
}

/*
retorna a frequecia de um index do vetor de frequencias
*/
long long unsigned int get_freq_table(Freq_Table* f_tbl, int i){
    return f_tbl->freq_vector[i];
}

/*
libera a tabela de frequencias
*/
void free_freq_table(Freq_Table* f_tble){
    free(f_tble);
    return;
}

/*
imprime a tabela de frequencias
*/
void show_freq_table(Freq_Table* f_tbl){
    forn(i , ASCII){
        if(f_tbl->freq_vector[i]) printf("%c: %lld\n", (char)i, f_tbl->freq_vector[i]);
    }
    return ;
}

/*
lê-se os bytes do arquivo e insere as frequencias na tabela até o fim do arquivo
*/
void fread_freq_table(Freq_Table* f_table, FILE * file){
    unsigned char  c = 0;
    c= fgetc(file);
    while(!feof(file)){
        f_table->freq_vector[c] ++ ;
        c= fgetc(file);
    }
}