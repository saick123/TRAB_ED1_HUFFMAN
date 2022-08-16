#ifndef STREAM_H
#define STREAM_H

#include <stdio.h>
#include <stdlib.h>

typedef struct stream Stream;

/*cria uma stream que está atrelada a um arquivo de entrada, usa-se o arquivo de entra como input da stream até ele ficar vazio, o vetor tem tamanho máximo igual ao valor passado*/
Stream* init_stream(FILE* f,unsigned int MAX_SIZE_bytes);

/*preenche o vetor da stream lendo os bytes do arquivo atrelado*/
void fill_stream( Stream* s );

/*verifica se o arquivo atrelado chegou ao seu fim*/
int stream_feof(Stream* s);

/*usa-se a stream como buffer, passando um ponteiro para byte, é colocado no que é apontadp por ele o byte da determinada posção da stream*/
int read_by_stream(Stream* s , unsigned char * c );

/*verifica se o vetor da stream já foi lido completamente, ou seja, precisa ser preenchido novamente*/
int empty_stream(Stream* s);

/*libera a stream, OBS: nao fecha o arquivo atrelado*/
void free_stream(Stream* s );

#endif 