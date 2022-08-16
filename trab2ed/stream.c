#include <stdio.h>
#include <stdlib.h>
#include "stream.h"

struct stream{
    FILE* f;
    unsigned long long int file_size;
    unsigned char * arr;
    unsigned int index;
    unsigned int leght;
    unsigned int MAX_SIZE;
    int stream_feof;
};

/*
inicia e retorna uma stream
*/

Stream* init_stream(FILE* f,unsigned int MAX_SIZE_bytes){
    Stream* s = (Stream*)calloc(1 , sizeof(Stream));
    s->arr = (char*)calloc(MAX_SIZE_bytes, sizeof(char));
    s->index =0 ;
    s->leght =0 ;
    s->MAX_SIZE = MAX_SIZE_bytes;

    s->stream_feof = feof(f) ;
    s->f =f ;
    unsigned long long int pos = ftell(f);
    fseek(f, 0 , SEEK_END);
    s->file_size = ftell(f);
    fseek(f, pos , SEEK_SET);

    return s ;
}


/*
preenche uma stream com o seu tamanho total, ou o que resta do arquivo atrelado, se o arquivo tive vazio, retorna
*/

void fill_stream( Stream* s ){
    if(s->stream_feof == 1){  
        return;
    }
    FILE* f_in = s->f;
    s->leght =0 ;
    s->index =0 ;

    unsigned long long int pos = ftell(f_in);
    unsigned long long int  TAM = s->file_size;
 
    char * arr = s->arr;

    if((TAM - pos) > s->MAX_SIZE){
        fread((void*)arr, sizeof(char), s->MAX_SIZE, f_in);
        s->leght = s->MAX_SIZE;
    }else{
        s->stream_feof = 1;
        fread((void*)arr, sizeof(char), (TAM - pos), f_in);
        s->leght = TAM - pos;
    }
}

/*
retorna flag de stream vazia == não há mais dados para serem lidos do arquivo
*/
int stream_feof(Stream* s){
    return s->stream_feof;
}

/*
seta o que é apontado por 'c' como o valor do byte na posição atual do vetor da stream, se a stream já tiver
sido lida completamente, e tiver dados disponiveis no arquivo, preenche ela com mais dados (bytes)
*/
int read_by_stream(Stream* s , unsigned char * c ){

    if(empty_stream(s)){
        if(stream_feof(s)){
            return 0;
        }
        fill_stream(s);
    }
    *c = s->arr[s->index];
    s->index ++ ; 
    return 1;
}

/*verifica se o vetor da stream chegou ao final*/
int empty_stream(Stream* s){
    if(s->index == s->leght) return 1 ;
    else return 0;
}

/*libera a stream, nao fecha o arquivo atrelado*/
void free_stream(Stream* s ){
    free(s->arr);
    free(s);
    return;
}