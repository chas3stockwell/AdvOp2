#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <unistd.h>

void errorMessage(){
    char error_message[30] = "An error has occurred\n";
    fprintf(stderr, error_message, strlen(error_message));
    exit(0);

}

void appendChar(char subject[], const char insert[], int pos) {
    char buf[100] = {}; 
    strncpy(buf, subject, pos); 
    int len = strlen(buf);
    strcpy(buf+len, insert); 
    len += strlen(insert);  
    strcpy(buf+len, subject+pos); 
    strcpy(subject, buf);  
}

void failedPathMsg(char buffer[]){
    appendChar(buffer, ":", 2);
    buffer[strcspn(buffer, "\r\n")] = 0;
    char error_message[50] = ": No such file or directory\n";
    strcat(buffer, error_message);
    fprintf(stderr, buffer, strlen(buffer));
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
    
    int count = 0;

    char *end, *r, *tok;
    r = end = strdup(buffer);

    /*check if valid commands are entered*/
    while ((tok = strsep(&end, " ")) != NULL) {
        count += 1; 

    }

    if( (count != 2) || (strcmp(buffer, "cd") == 0) ){
        errorMessage(); 
    }
    //printf("reached cd method"); 
    return 0; 
}


int lscommand(char buffer[]){
  
    int count = 0;

    char *end, *r, *tok;
    r = end = strdup(buffer);

    
    /*check if valid commands are entered*/
    /*while ((tok = strsep(&end, " ")) != NULL) {
        count += 1;
    }*/


    bool IsPath = false;
 
    while ((tok = strsep(&end, " ")) != NULL) {
        count += 1;
        
        if (IsPath){
            
            /*if( (access(tok, X_OK) == -1) && (access( strcat("/usr", tok), X_OK) == -1) ) {
                errorMessage(); 
            }*/
            char new_string[50];
            strcpy(new_string,  "/usr");
            char string_ss[50];
            strcpy(string_ss, tok);
            strcat(new_string, string_ss);

            
            if( (access(tok, X_OK) == -1 ) && (access(new_string , X_OK) == -1)   ){
                failedPathMsg(buffer); 
            }

        }else{
            IsPath = true;
        }
    }

    if( (count != 2) || (strcmp(buffer, "cd") == 0) ){
        errorMessage(); 
    }

    /*if (access())
    //check for file
    if access("/bin/ls", X_OK).
    /usr/bin/ls.*/

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
        //printf("wish> ");
        
        characters = getline(&buffer, &bufsize, stdin);

        char *end, *r, *tok;
        r = end = strdup(buffer);
        bool command = false;

        while ((tok = strsep(&end, " ")) != NULL) {
            //printf("%s\n", tok);
            //printf("%lu", strlen(tok)); 
            
            
            /*check if we need to run our program correctly*/
            if( ((strcmp(tok, "cd") == 0) || (strcmp(tok, "cd\n") == 0)) && (!command) ){
                
                cdcommand(buffer);
                command = true; 

            }else if( ((strcmp(tok, "ls") == 0) || (strcmp(tok, "ls\n") == 0)) && (!command) ){
                lscommand(buffer);
                command = true;
            }


        }    
        
    }while( (strncmp(buffer, "exit", strlen(buffer)-1) != 0) || (is_empty(buffer) == 1) );


    return(0); 
}



int argwishLoop(FILE *fp){
    //printf("%s", arg);
   
    //printf("we got here" );
    char *buffer;
    size_t bufsize = 32;
    size_t bufread = 0;
    size_t characters;


    /*error check the buffer works for buffer*/
    buffer = (char *)malloc(bufsize * sizeof(char));
    if( buffer == NULL){ errorMessage(); }

    
    
    do{
        characters = getline(&buffer, &bufsize, fp);
        char *end, *r, *tok;
        r = end = strdup(buffer);
        bool command = false;
        
        while ((tok = strsep(&end, " ")) != NULL) {
            //printf("%s", tok); 
            /*check if we need to run our program correctly*/
            if( ((strcmp(tok, "cd") == 0) || (strcmp(tok, "cd\n") == 0)) && (!command) ){    
                cdcommand(buffer);
                command = true; 
            }else if( ((strcmp(tok, "ls") == 0) || (strcmp(tok, "ls\n") == 0)) && (!command) ){
                lscommand(buffer);
                command = true;
            }
        }    
    }while( (strncmp(buffer, "exit", strlen(buffer)-1) != 0) || (is_empty(buffer) == 1) );


    /*if( strcmp(argv[1],"cd\n") == 0 ){
        printf("match!"); 
        cdcommand(argv[1]);
    }*/

    return 0; 
}





int main(int argc, char *argv[] ){
    if ( argc <= 1 ) {
        /*no arguments are entered*/
        wishLoop(); 
    }else{
        /*arguments were entered*/
        FILE *fp; 
        fp = fopen(argv[1],"r");

        if (fp == NULL){
            errorMessage(); 
        }

        argwishLoop(fp); 
        /*errorMessage();*/
    }



    
    return(0);
}