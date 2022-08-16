//
// Created by user on 7/24/2022.
//

#ifndef TRAB2ED_TREE_H
#define TRAB2ED_TREE_H





typedef struct tree tree;

#include <stdio.h>
#include <stdlib.h>
#include "bitmap.h"
typedef long long unsigned llu;
typedef long long ll;
typedef long double ld;

/*
 * Alocação de memória
 */
tree* new(llu freq, char c);

/*
 * Une duas arvores e cria um nó pai
 */
tree* merge(tree* a, tree* b);

/*
 * Retorna o peso do nó
 */
llu get_weight(tree* a);

/*
* Retorna o char do nó
*/
char get_char(tree* a);

/*
 *  Desalocação de memória
 */
void erase(tree* a);


/*
*
* imprime a arvore
*/

void show_tree(tree* a);


/*
*
*   Calcula a altura da árvore
*/
int height_tree(tree* a);


/*
*
*   Verifica se é folha
*/
int its_leaf(tree*a );

/*
* retorna a avore filho da esquerda
*/
tree* left_child(tree* a );
/*
* retorna a arvore filho da direita
*/
tree* right_child(tree* a);

/*
* retorna uma arvore especial codificada por um bitmap 
*/


tree* recover_tree(FILE* f_in);

#endif //TRAB2ED_TREE_H
