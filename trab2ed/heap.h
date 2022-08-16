//
// Created by user on 7/24/2022.
//




#ifndef TRAB2ED_HEAP_H
#define TRAB2ED_HEAP_H

#include "heap.h"
#include "tree.h"
#include <stdio.h>
#include <stdlib.h>

typedef struct binary_heap binary_heap;

binary_heap* new_binary_heap();

void insert(binary_heap* a, tree* x);

tree* remove_min(binary_heap* a);

void delete_binary_heap(binary_heap* a);

void show_binary_heap(binary_heap* a );

int get_tam_binary_heap(binary_heap* a );

#endif //TRAB2ED_HEAP_H
