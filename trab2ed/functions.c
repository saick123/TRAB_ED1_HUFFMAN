
#include <stdio.h>
#include <stdlib.h>
#include "heap.h"
#include "freq_table.h"
#include "code_table.h"
#include "tree.h"
#include <string.h>
#include "bitmap.h"
#include "stream.h"
#define ASCII 256

#define BYTE_SIZE 8
#define BITS_READ (unsigned int)8*1024*1024*64


#define forn(i, n) for(int i =0; i < n ; i ++)


/*headers *****************************************************************************************************/

/*//////////////////////////////////////////////////(STATICS)//////////////////////////////////////////////////*/

/*
funcao que faz o papel de codicar os dados do arquivo de entrada e escrever resultante o bitmpap no arquivo de sáida
o tamanho passado é o tamanho máximo do bitmap
*/
static void code_and_write_bitmap(FILE* f_in, FILE* f_out,Code_Table* c_table, int * rem, unsigned int MAX_SIZE);

/*
decodifica os dados do arquivo de entrada e escreve no arquivo de sáida
*/
static void write_uncoded_for_unzip(FILE* f_out,FILE * f_in , tree* huffman_coded);


/*
procura por um determinado byte na arvore de codificação de forma recursiva
ret = ponteiro que ira conter o byte depois que encontrado
index = indice do bitmap de 8 bits, usado para percorrer no bitmap, quando o bitmap acaba, lê-se um novo byte do arquivo codificado
bitmap = contem um byte do arquivo codificado, sera percorrido e os calores dos seus bits serão utilizados para decodifcar qual byte na arvore ele na realidade representa
f_in = arquivo de entrada (arquivo codificado), usado para que, em caso do bit map esteja percorrido completamente, seja lido um novo byte de seu conteudo
*/
static void search_for_char( FILE* f_in, bitmap* map, tree* huffman,unsigned char* ret, int * index, long long  int * i);

static void private_fill_code_table(Code_Table* c_tbl , tree* a, char * string, int index);

static int get_size_coded_tree(tree* a );

static void imprime_arvore_cd(tree* a);

/*//////////////////////////////////////////////////////////////////////////////////////////////////////////////*/

/*
faz os devidos procedimentos para zipar o arquivo
*/
void private_zip(FILE* f, Code_Table* c_tbl, tree* huffman, char ** _argv );

/*
faz os devidos provedimentos para unzipar o arquivo
*/
void private_unzip(char * dir );

/*
chama a private_unzip
*/
void unzip(char ** argv );

/*
chama a private_zip
*/
void zip(char ** argv);

/***************************************************************************************************************/

/*
insere na heap nós de arvores de todos os bytes encontrados no arquivo original
*/
void fill_heap_with_freq_table(binary_heap* b, Freq_Table* f_tbl){
    forn(i , ASCII){
        if(get_freq_table(f_tbl, i)){
            tree* aux = new(get_freq_table(f_tbl, i), (char)i);
            insert(b, aux);
        }
    }
}

/*
algoritmo que retira dois elementos minimos de uma heap(arvores), junta eles em uma nova arvore, e insere o resultado na heap
o provesso continua até que haja somente um elemento na heap, o qual é a arvore de huffman
*/
tree* huffman_tree_constructor(binary_heap* b){
    while(get_tam_binary_heap(b) >=2){
        insert(b, merge(remove_min(b), remove_min(b)));
    }
    return remove_min(b);
}

/*
uma string com tamanho maximo da arvore é passada, assim ela tem o tamanho do maior código que pode ser formado na arvore construida
percorre-se a arvore, quando achamos uma folha colocamos um \0 na posição indicando que queremos que a string termine ali
senao colocamos 0 na posicao e chamamos pra esquerda
depois colocamos 1 na posição e chamamos para a direita
a própria recursividade acerta os valores dos indices, que são decrementados na volta da recursão
quando uma folha é achada, insere-se uma copia da string resultante na tabela de frequencias na posição que reprensenta aquele caractere
*/

static void private_fill_code_table(Code_Table* c_tbl , tree* a, char * string, int index){
    if(!a) return;
    if(its_leaf(a)){
        string[index] = '\0';
        insert_code_table(c_tbl, string , get_char(a));
    }else{
        string[index] = '0';
        private_fill_code_table(c_tbl,left_child(a), string ,index + 1 );
        string[index] = '1';
        private_fill_code_table(c_tbl, right_child(a), string ,index + 1 );
    }
    return ;

}

/*
faz uma string de tamanho maximo da arvore + 1 (o +1 é pra o \0) e chama a private
*/
void fill_code_table(Code_Table* c_tbl, tree* a ){
    int a_height = height_tree(a);
    char string [a_height + 1 ];
    for(int i =0; i < a_height + 1 ; i ++){
        string[i] =0;
    } 
    private_fill_code_table(c_tbl, a, string , 0);
    return ;

}

/*
retorna a altura da arvore
*/
static int get_size_coded_tree(tree* a ){
    if(!a) return 0;
    if(its_leaf(a)){
        return BYTE_SIZE + 1 ;
    }else{
        return 1 + get_size_coded_tree(left_child(a)) + get_size_coded_tree(right_child(a));
    }
}

/*
funcao de depuração
*/
static void imprime_arvore_cd(tree* a){
    if(!a) return ;
    if(its_leaf(a)){
        printf("1%c", get_char(a));
    }else{
        printf("0");
        imprime_arvore_cd(left_child(a));
        imprime_arvore_cd(right_child(a));
    }
    return ;
}

/*
funcao que cofifica a arvore de huffman e escreve no arquivo de saida
folha -> append( 1 e escreve byte no bit map )
senao -> append(0)
*/
static void write_coded_tree(tree* a, FILE* f, bitmap* map){
    if(!a) return ;

    if(its_leaf(a)){
        bitmapAppendLeastSignificantBit(map, 0x01);
        unsigned int aux = 0x80;
        unsigned char c = get_char(a);
        for(; aux !=0; aux >>= 1){
            unsigned int and = aux & c;
            if(and) bitmapAppendLeastSignificantBit(map, 0x01);
            else bitmapAppendLeastSignificantBit(map, 0);
        }
    }else{
        bitmapAppendLeastSignificantBit(map, 0);
        write_coded_tree(left_child(a),f , map);
        write_coded_tree(right_child(a), f ,map);
    }
}

/*
cofica e escreve arvore, abre arquivo de saida e chama 'code_and_write_bitmap'
*/

void private_zip(FILE* f, Code_Table* c_tbl, tree* huffman, char ** _argv ){

    char * dir = _argv[1]; 
    char separator[2] = ".";
    char * aux  = strtok(dir , separator);
    char * f_zip_dir = (char*)calloc(1 , sizeof(char)*(strlen(aux) + 17));
    sprintf(f_zip_dir,"./compfile/%s.comp", aux);

    FILE* f_zip = fopen(f_zip_dir,"wb");
    if (f_zip == NULL ) exit(1);
    
    aux = strtok(NULL, separator);

    unsigned char c_fwrite = strlen(aux);
  

    /*escrevendo bits de controle para o tipo de extensão anterior a compressao*/
    /*na ordem, é impresso quantos bytes tem a terminação e a terminação em si*/
    fwrite((void*)&c_fwrite, sizeof(char), 1, f_zip);
    fwrite((void*)aux , sizeof(char)*((unsigned int )c_fwrite) , 1, f_zip);
       
    unsigned int coded_tree_size = get_size_coded_tree(huffman); 
    unsigned int coded_tree_size_bytes = (coded_tree_size + 7)/8;
    //printf("\n%d\n", coded_tree_size);
    /*escrevendo em dois bytes o tamanho da arvore codificada
    **é um numero binario impresso ao contrario (AB)- > (BA)*/
   
    bitmap * map_coded_tree = bitmapInit(coded_tree_size);
    write_coded_tree(huffman, f_zip, map_coded_tree );

    unsigned char * contents = bitmapGetContents(map_coded_tree);

    /*escrevendo arvore no arquivo*/
    fwrite((void*)contents, sizeof(char), coded_tree_size_bytes, f_zip);
   
    bitmapLibera(map_coded_tree);

    code_and_write_bitmap(f, f_zip, c_tbl, 0 , BITS_READ);

    fclose(f_zip);
    free(f_zip_dir);

    return ;
}

/*
cria um bitmap e uma stream de acordo com o valor passado
a stream é atrelada ao arquivo de entrada(arquivo a ser codificado)
lê-se da stream bytes do arquivo original que serão codificados
retorna a string referente ao codigo daquele byte 
vai inserindo os bits das strings no bitmap até que ele esteja cheio, quando ele encher, ele é escrito no arquivo de saída(arquivo codificado)
calcula quantos bits faltaram para completar o ultimo byte a ser escrito no arquivo de saida e escreve no final do arquivo
*/
static void code_and_write_bitmap(FILE* f_in, FILE* f_out,Code_Table* c_table, int * rem, unsigned int MAX_SIZE){

    fseek(f_in, 0, SEEK_SET);
    bitmap* b_writer = bitmapInit(MAX_SIZE);
    Stream* s = init_stream(f_in ,MAX_SIZE/8);

    unsigned char c =0;
    
    while(read_by_stream(s, &c)){
            
        char * string = get_code_table(c_table, (unsigned int )c);
        int index =0;
        int tam_string = strlen(string);

        while(index < tam_string){

            bitmapAppendLeastSignificantBit(b_writer, string[index]);
            if(bitmapGetLength(b_writer) == bitmapGetMaxSize(b_writer)){
                char * contents = bitmapGetContents(b_writer);
                fwrite((void*)contents, sizeof(char)*(MAX_SIZE/8) , 1, f_out);
                bitmapLibera(b_writer);
                b_writer = bitmapInit(MAX_SIZE);
            }
            index ++;
        }
    }

    unsigned int bit_map_leght = bitmapGetLength(b_writer);
    unsigned char  n_aprox = bit_map_leght%8;
  
    if(bit_map_leght != 0 && n_aprox == 0){
        n_aprox = 8;
    }

    char * contents = bitmapGetContents(b_writer);
    unsigned int lenght_byte= (bit_map_leght + 7)/8;
   
    /*se ainda restar coisas no bit map, que sejam escritas no arquivo*/
    fwrite((void*)contents, sizeof(char)*lenght_byte , 1, f_out);
    /*numero que guarda o o quanto faltou para completar o ultimo byte*/    
    fwrite((void*)&n_aprox, sizeof(char), 1, f_out);
  

    free_stream(s);
    bitmapLibera(b_writer);
}


/*
fucao que abre o arquivo de saida, decodifica a arvore que estava no arquivo e, por fim chama  write_uncoded_for_unzip, que decodificará
o conteudo do arquivo codificado e escreverá em outro arquivo a tradução
*/
void private_unzip(char * dir ){

    int tam_dir = strlen(dir);
    char *modify_dir = (char*)calloc(1,sizeof(char)*(tam_dir + 12));
    sprintf(modify_dir, "./compfile/%s", dir);

    FILE* f_in = fopen(modify_dir, "rd");
    if(f_in == NULL) exit(2);

    fseek(f_in, 0 , SEEK_SET);
    
    char * aux ;
    sscanf(dir, "%m[^.]", &aux);
    unsigned char c_read = fgetc(f_in);

    int tam = c_read;
    char terminator[tam + 1];

    for(int i =0; i < tam; i ++){
        c_read = fgetc(f_in);
        terminator[i] = c_read;
    }
    terminator[tam] = '\0';

    char * new_dir = (char*)calloc(1, sizeof(char)*(strlen(aux) + c_read + 2 + 10));
    sprintf(new_dir, "./newfile/%s.%s", aux, terminator );

    
    free(aux);

    FILE* f_out  = fopen(new_dir, "wb");
    if(f_out == NULL) exit(3);

 
    tree* huffman_decoded = recover_tree(f_in);
   

    write_uncoded_for_unzip(f_out,f_in, huffman_decoded);
        
    fclose(f_in);
    fclose(f_out);
    free(new_dir);
    free(modify_dir);
    erase(huffman_decoded);
    return ;


}

/*
funcao que decodifica os bytes presentes no arquivo de entrada
usa-se um bitmap de 8 bits para isso e vai-se percorrendo por ele
quando o bitmap acaba, ele é preenchido novamente e seu index é zerado
se chegamos a uma folha, faz ser o que é apontado por ret (retorno) o byte correspondente a aquela folha, e retornamos a recursao
senao, lemos o bit do bitmap, se for 0 precisamos ir para a esquerda, se for, precisamos ir para a direita
*/
static void search_for_char( FILE* f_in, bitmap* map, tree* huffman,unsigned char* ret, int * index, long long  int * i){
    //verifica se um dos filhos é null, se for é folha, pois a arvore de huffman é completa
    if(right_child(huffman) == NULL){
        *ret = get_char(huffman);
        return ;
    } 
    /*bitmap vazio sendo preenchido*/
    if(*index >= BYTE_SIZE){
        char * contents = bitmapGetContents(map);
        *contents= fgetc(f_in);
        *i = *i  + 1  ;
        *index = 0;
    }
    int n = bitmapGetBit(map, *index);
    //se n == 1, vai pra direita, senão vai para esquerda
    if(n){
        *index = *index +  1;
        search_for_char(f_in, map, right_child(huffman), ret, index , i );
    }else{
        *index = *index + 1;
        search_for_char(f_in, map, left_child(huffman), ret, index , i );
    }
    return;
}


/*
funcao que escreve o conteudo decodificado no arquivo de saida
um bitmap de tamanho fixo é usado para escrever no arquivo de saida
o loop funciona até que cheguemos no penultimo byte do arquivo, porque no ultimo byte está a infomação da aproximação do SO
quando o bitmap enche, ele é escrito no arquivo de saida;
quando saimos do loop, verificamos se faltou escrever no arquivo o bitmap que pode nao estar vazio
*/
static void write_uncoded_for_unzip(FILE* f_out,FILE * f_in , tree* huffman_coded){

    long long unsigned int pos = ftell(f_in);
    fseek(f_in, 0, SEEK_END);
    long long unsigned int TAM = ftell(f_in);
    fseek(f_in, pos, SEEK_SET);
    
    bitmap* map = bitmapInit(BYTE_SIZE);
    bitMapSetLenght(map, BYTE_SIZE);
    int index =8 ;
    long long unsigned  int i = 0;

    bitmap* map_write = bitmapInit(BITS_READ);
    unsigned char * char_arr = bitmapGetContents(map_write);
    unsigned int index_arr = -1 ;
    unsigned int tam_arr = (BITS_READ + 7)/8;
    unsigned char ret =0;
    
    /*o loop sempre irá quebrar quando chegar ao penultimo byte, após isso precisamos somente verificar se analisamos o ultimo byte todo*/
   while( i < (TAM - pos - 1)){
        index_arr ++;
        bitMapSetLenght(map_write , 8*(index_arr + 1) );
        search_for_char(f_in, map, huffman_coded,&ret, &index , &i);

        char_arr[index_arr] = ret;

        if(index_arr == tam_arr - 1 ){
            index_arr = -1;
            fwrite((void*)char_arr, sizeof(char)*tam_arr, 1, f_out);
            bitMapSetLenght(map_write, 0 );
        }
    }

    long unsigned int tam_map = bitmapGetLength(map_write);
    if(tam_map){
        fwrite((void*)char_arr, sizeof(char)*(tam_map/8), 1, f_out);
    }

    /*conserta o valor do index caso tenha saido da funcao com valor igual a 8*/
    if(index >= 8){
        index =0;
    }

    /*resgata o ultimo byte do arquivo contendo as informações de quantos bits do penultimo byte realmente foram escritos*/
   unsigned char aux = fgetc(f_in);

        /*enquanto o penultimo byte nao for totalmente percorrido até um indice valido, decodifica-se bytes na arvore*/
         while(aux - 1>= index ){
         search_for_char(f_in, map, huffman_coded, &ret, &index, &i);
         fwrite((void*)&ret, 1, sizeof(char), f_out);
        }
    
    
    bitmapLibera(map);
    bitmapLibera(map_write);

    return;

}


/*
funcao que chama a private_zip
monta-se a arvore de huffman e a tabela de codigos pra mandar pra private_zip
*/
void zip(char ** argv){

    int tam  =strlen(argv[1]);
    
    char * dir = (char*)calloc(1, sizeof(char)*(tam + 10 ));
    sprintf(dir, "./target/%s", argv[1]);
    FILE* f = fopen(dir, "rb");
    if(f == NULL ) exit (1);
    Freq_Table * f_tbl = init_freq_table();


    fread_freq_table(f_tbl, f);
    
    binary_heap * b = new_binary_heap();
    fill_heap_with_freq_table(b, f_tbl);
    tree* huffman = huffman_tree_constructor(b);
    

    Code_Table* c_tbl = init_code_table();
    
    fill_code_table(c_tbl, huffman);

    private_zip(f, c_tbl, huffman, argv);

    free_code_table(c_tbl);
    free_freq_table(f_tbl);
    free(dir);
    fclose(f);
    delete_binary_heap(b);
    erase(huffman);
}

/*
só precisa chamar a private_unzip
*/
void unzip(char ** argv){
    private_unzip(argv[1]);
}

/*
executa uma determinada operação (zip ou unzip) de acordo com o que o cliente digitou no terminal
*/
int execute(int argc, char* argv[]){

    if(argc < 2){
        printf("\nFormato de execução digitado incorretamente!\n");
        return -1;
    }
    
    if(strcmp("zip", argv[2]) == 0 ){
        zip(argv);
    }else if(strcmp("unzip", argv[2]) == 0){
        unzip(argv);
    }else{
        printf("\nOperação inválida digitada!\n");
        return -1;
    }
    printf("\nOpeação realizada com sucesso\n");
    
    return 1;
}