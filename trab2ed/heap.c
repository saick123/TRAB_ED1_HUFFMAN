#ifndef HEAP_C
#define HEAP_C

#include "heap.h"
//
// Created by user on 7/24/2022.
//




#define MAX	512

struct binary_heap{
    tree* h[MAX];
    int n;
};

typedef struct binary_heap binary_heap;

binary_heap* new_binary_heap(){
    binary_heap* new_heap = calloc(1, sizeof(binary_heap));
    if(new_heap==NULL) {
        fprintf(stderr,"ERROR, NEW HEAP IS NOT ALLOCATED\n");
        exit(1);
    }
    return new_heap;
}

static void swap(tree** x, tree** y){
    tree* temp = *x;
    *x = *y;
    *y = temp;
}

void insert(binary_heap* a, tree* x){

    a->h[a->n]=x;
    a->n++;

    int i = (a->n)-1;
    while(i > 0 && get_weight(a->h[i]) < get_weight(a->h[(i-1)/2]) ){
        swap( &(a->h[i]), &(a->h[(i-1)/2]) );
        i = (i-1)/2;
    }
}

tree* remove_min(binary_heap* a){

    swap( &(a->h[a->n-1]), &(a->h[0]));
    a->n--;

    int i = 0, j= 0;

    //1 >= child
    while( 2*i + 1 < a->n){

        //min between two children
        j = 2*i + 1;
        if( (2*i + 2) < (a->n) && get_weight(a->h[2*i+2]) < get_weight(a->h[j]) ){
            j = 2*i + 2;
        }
        //keeps the property
        if( get_weight(a->h[j]) >= get_weight(a->h[i]) ){
            break;
        }
        else {
            swap(&(a->h[j]), &(a->h[i]));
            i = j;
        }

    }

    return a->h[a->n];
}


void delete_binary_heap(binary_heap* a){
    free(a);
}


void show_binary_heap(binary_heap* a ){
    if(!a) return;
    for(int i =0; i < a->n; i ++){
        show_tree(a->h[i]);
    }
    return ;
}

int get_tam_binary_heap(binary_heap* a ){
    return a->n;
}

#endif
