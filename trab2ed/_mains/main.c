#include <stdio.h>
#include "main.h"

/********************************************************************************************************************************/

    /*                                                                                                                      
     *
     *
     *  DESCRIÇÃO PARA A EXECUÇÃO DO PROGRAMA: 
     *  
     *  FORMATO :  digite no terminal --> "./(nome do executável) (alvo) (operação)"
     * 
     *   
     *  
     *  1 - Para zipar um arquivo:
     *      
     *      (*)   Crie uma pasta chamada "compfile" no mesmo diretório do executável, nessa será colocado o arquivo zipado
     * 
     *      (**) Crie uma pasta chamada "target" no mesmo diretório do executável, nessa você deve colocar o ALVO
     * 
     *      Ex.: ./ruffman texto.txt zip
     * 
     *      
     *      
     *  2 - Para unzipar um arquivo:
     *      
     *      (*)  Crie uma pasta chamada "newfile" no mesmo diretório do executável, nessa será colocado o arquivo unzipado
     * 
     *      (**) O Alvo deve estar em uma pasta chamada "compfile", está pasta deve estar no mesmo diretório do executável
     * 
     *      Ex.: ./ruffman texto.comp unzip
     * 
     * 
     *  
    */

/********************************************************************************************************************************/



#include "headers.h"
int main(int argc, char* argv[]) {

    execute(argc,  argv);
    
    return 0;
}



