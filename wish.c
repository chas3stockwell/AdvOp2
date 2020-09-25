#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

void errorMessage(){
    char error_message[30] = "An error has occurred\n";
    fprintf(stderr, error_message, strlen(error_message));
    exit(0);

}

int is_empty(const char *s) {
  while (*s != '\0') {
    if (!isspace((unsigned char)*s))
      return 0;
    s++;
  }
  return 1;
}

int cdcommand(char buffer[]){
    //printf("%s", buffer);
    printf("reached cd method"); 
    return 0; 
}


int lscommand(char buffer[]){
    printf("reached ls method");
    return 0; 
}



int wishLoop(){
    
    char *buffer;
    size_t bufsize = 32;
    size_t bufread = 0;
    size_t characters;


    /*error check the buffer works for buffer*/
    buffer = (char *)malloc(bufsize * sizeof(char));

    if( buffer == NULL){ errorMessage(); }

   
    do{
        printf("wish> ");        
        characters = getline(&buffer, &bufsize, stdin);

        char *end, *r, *tok;
        r = end = strdup(buffer);
        bool command = false;

        while ((tok = strsep(&end, " ")) != NULL) {
            printf("%s\n", tok);
            printf("%lu", strlen(tok)); 
            
            
            
            if( (strcmp(tok, "cd") == 0) || (strcmp(tok, "cd\n") == 0) ){
                
                cdcommand(buffer);
                command = true; 

            }else if( (strcmp(tok, "ls") == 0) || (strcmp(tok, "ls\n") == 0) ){
                lscommand(buffer);
                command = true;
            }


        }
        

        
        
        
    }while( (strncmp(buffer, "exit", strlen(buffer)-1) != 0) || (is_empty(buffer) == 1) );


    return(0); 
}



int main(int argc, char *argv[] ){
    if ( argc <= 1 ) {
        /*no arguments are entered*/
        wishLoop(); 
    }else{
        wishLoop(); 
    }



    
    return(0);
}