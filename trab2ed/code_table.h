#ifndef CODE_TABLE
#define CODE_TABLE

typedef struct code_table Code_Table;


/*retorna um tabela de códigos com os vetores vazios*/
Code_Table* init_code_table(void);

/*insere na tabela de codificão no index especificado uma string que representada o código do caracter de mesmo valor ASCII  que o index*/
void insert_code_table(Code_Table* c_table, char* string , unsigned char index);

/*libera a tabela de codificação*/
void free_code_table(Code_Table* c_table);

/*retorna um string que representa o código do caractere do mesmo valor ASCII que o index passado*/
char * get_code_table(Code_Table* c_table, int index);

/*imprime a tabela de codificação para melhor depuração e reconhecimento do fluxo do programa*/
void show_code_table(Code_Table* c_table);


#endif 