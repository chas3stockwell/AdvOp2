#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void errorMessage(){
    char error_message[30] = "An error has occurred\n";
    fprintf(stderr, error_message, strlen(error_message));
    exit(1);

}


int wishLoop(){
    
    char *buffer;
    size_t bufsize = 32;
    size_t characters;

    /*error check the buffer works for buffer*/
    buffer = (char *)malloc(bufsize * sizeof(char));

    if( buffer == NULL){ errorMessage(); }

    
    
    do{
        printf("wish> ");
        characters = getline(&buffer, &bufsize, stdin);
        
    }while(strncmp(buffer, "exit", strlen(buffer)-1) != 0);


    return(0);
}



int main(int argc, char *argv[] ){
    if ( argc <= 1 ) {
        /*no arguments are entered*/
        wishLoop();
    }else{
        errorMessage();
    }



    
    return(0);
}
