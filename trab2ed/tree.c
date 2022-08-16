#ifndef TREE_C
#define TREE_C

#include "tree.h"
#include "headers.h"
#include <string.h>
//
// Created by user on 7/24/2022.
//


#define BYTE_SIZE 8


typedef struct tree tree;
/*
 * Convenção adotada: caminhar para a esquerda de um nó, equivale a o bit 0, para a direita o bit 1
 */
struct tree{
    unsigned char c;
    llu freq;
    tree* right;
    tree* left;
};

/*////////////////////////////////////////(STATIC)//////////////////////////////////*/

/*

*/
static tree* private_recover_tree(FILE* f_in ,bitmap* map, int * index);

static tree* e(tree* h);

static int max(int a, int b);

static void appendbyte(bitmap* map, unsigned char c);

static int read_bit(bitmap* map, FILE* f_in,int * index);

static char read_byte(bitmap* map, FILE* f_in,int * index);

/*/////////////////////////////////////////////////////////////////////////////////////*/
/*
 * Alocação de memória
 */
tree* new(llu freq, char c){
    tree* new_tree = calloc(1, sizeof(tree));
    if(new_tree == NULL){
        fprintf(stderr,"ERROR, NEW TREE IS NOT ALLOCATED\n");
        exit(1);
    }

    new_tree->c =c;
    new_tree->freq = freq;
    new_tree->left = NULL;
    new_tree->right = NULL;

    return new_tree;
}

/*
 * Une duas arvores e cria um nó pai que não possui um caractere associado
*/
tree* merge(tree* a, tree* b){
    tree* parent = new(0,0);
    parent->left = a;
    parent->right = b;
    parent->freq = a->freq + b->freq;
    return parent;
}

llu get_weight(tree* a){
    return a->freq;
}



static tree* e(tree* h){
    if(h == NULL) return NULL;
    h->right = e(h->right);
	h->left = e(h->left);
	if(h->right == NULL && h->left == NULL){
		free(h);
		return NULL;
	}
}
/*
 *  Desalocação de memória
 */
void erase(tree* root){
    if(root == NULL) return;
    e(root);

}

/*
imprime arvore
*/
void show_tree(tree* a){
    if(!a)return;
    printf("< %c: %lld", a->c, a->freq);
    show_tree(a->left);
    show_tree(a->right);
    printf(" >");
    return;
}

/*
retorna o maximo entre dois inteiros
*/
static int max(int a, int b){
    return ( a > b ) ? a : b;
}

/*
retorna a altura da arvore
*/
int height_tree(tree* a){
    if(a == NULL) return -1;
    return (1 + max(height_tree(a->right), height_tree(a->left)));
}

/*
retorna o char armazenado no nó da arvore especificado
*/
char get_char(tree* a){
    return a->c;
}


/*
verifica se um nó é folha 
*/
int its_leaf(tree*a){
    return (a->left == NULL && a->right == NULL) ? 1 : 0;
}

/*
retorna filho da esquerda
*/
tree* left_child(tree* a ){
    return a->left;
}

/*
retorna filho da direita
*/
tree* right_child(tree* a){
    return a->right;
}


/*
percorre os bits de um byte "c" e vai inserindo no bitmap
insere um byte no vetor de q byte do bitmap
*/

static void appendbyte(bitmap* map, unsigned char c){
    unsigned char * contents = bitmapGetContents(map);
    *contents = c;
    return ;
    /*bitMapSetLenght(map, 0 );
    unsigned char * contents = bitmapGetContents(map);
    memset(contents,0 , sizeof(char));

    for(int i =0x80; i != 0; i >>=1 ){
        if(i & c) bitmapAppendLeastSignificantBit(map, 0x01);
        else bitmapAppendLeastSignificantBit(map, 0 );
    }
    */
}

/*
retonra um bit na posicação atual do byte do bitmap
se o bitmap esvaziar, lê-se um novo byte no arquivo e insere-se no bitmap
*/
static int read_bit(bitmap* map, FILE* f_in,int * index){
    if(*index >=8){
        unsigned char c = fgetc(f_in);
        appendbyte(map, c);
        *index =0 ;
    }
    
    return bitmapGetBit(map, (*index) ++);
}

/*
lê-se um byte inteiro do bitmap de 1 byte, basicamenta chama-se 8 vezes(uma pra cada bit) a funcao read_bit
vai fazendo-se "or's" pra ir inserindo no byte e o retorna
*/
static char read_byte(bitmap* map, FILE* f_in,int * index){
    unsigned char c = 0;
    for(int i =0; i < BYTE_SIZE; i ++){
        c <<=1;
        int n  = read_bit(map, f_in, index);
        c |= n;
    }
    return c;
}

/*
sempre se cria uma arvore, basta-se decidir se ela irá virar folha ou não
funcao que recupera a arvore codificada no arquivo codificado 
se o read_bit retornar 1, significa que é folha, logo teremos um caractere pela frente
se o read_bit retornar 0, significa que é um nó qualquer, entao seguimos normalmente com a recursao
*/
static tree* private_recover_tree(FILE* f_in ,bitmap* map, int * index){
    tree* a = new(0,0);
    a->freq =0;

    if(read_bit(map, f_in, index)){
        unsigned char c = read_byte(map, f_in,index);
        a->c =c;
        a->left = NULL;
        a->right = NULL;
    }else{
        a->left = private_recover_tree(f_in , map, index);
        a->right = private_recover_tree(f_in, map, index);
        a->c = 0;
    }
    return a;
}

/*
funcao que cria uma arvore de huffman decodificando uma arvore codificada no arquivo de entrada
para isso é chamada a funcao private_recover_tree
*/
tree* recover_tree(FILE* f_in){
    bitmap* map = bitmapInit(BYTE_SIZE);
    int index = 0 ;

   unsigned char * contents = bitmapGetContents(map);
    *contents = fgetc(f_in);
    bitMapSetLenght(map, 8 );

    tree* a = private_recover_tree(f_in, map, &index );

    bitmapLibera(map);
    return a;
   
}

#endif