#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <unistd.h>

//global path variable
char Path[100] = "/bin";

void errorMessage(){
    char error_message[30] = "An error has occurred\n";
    fprintf(stderr, error_message, strlen(error_message));
    exit(0);

}

void removeNewline(char buffer[]){
    buffer[strcspn(buffer, "\r\n")] = 0;
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
    removeNewline(buffer);
    
    char error_message[50] = ": No such file or directory\n";
    strcat(buffer, error_message);
    fprintf(stderr, buffer, strlen(buffer));
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

int cd(char *path) {
    return chdir(path);
}


int cdcommand(char buffer[]){
    //printf("get here?");
    int count = 0;

    char *end, *r, *tok;
    r = end = strdup(buffer);
    bool isPath = false; 
    /*check if valid commands are entered*/
    while ((tok = strsep(&end, " ")) != NULL) {
        count += 1;
        if (isPath) {
            
            char s[100];
            
            //appendChar(tok, "/", 0);
            
            removeNewline(tok);
            if( cd(tok) == -1){
                errorMessage(); 
            }else{
                cd(tok);
            }
            

        }else{
            isPath = true; 
        }

    }
    
    if( (count != 2) || (strcmp(buffer, "cd") == 0) ){
        errorMessage(); 
    }
     
    return 0; 
}


int lscommand(char buffer[]){
    //printf("%s", buffer);
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
            
            
            char new_string[50];
            strcpy(new_string,  "/usr");
            char string_ss[50];
            strcpy(string_ss, tok);
            strcat(new_string, string_ss);

            
            if( (access(tok, X_OK) == -1 ) && (access(new_string , X_OK) == -1)   ){
                failedPathMsg(buffer); 
            }else{
                
                char *args[2];
                char strcommand[100];
                strcpy(strcommand, Path);
                strcat(strcommand, "/ls");
                
                args[0] = strcommand;        // first arg is the full path to the executable
                args[1] = NULL;
                
                execv(args[0], args);
            }



        }else{
            IsPath = true;
        }
    }

    if(count == 1){
        char *args[2];
        args[0] = "/bin/ls";        
        args[1] = NULL;
        execv(args[0], args);
    }
    

    return 0; 
}

int pathcommand(char buffer[]){
    
    char *end, *r, *tok;
    r = end = strdup(buffer);
    bool isPath = false;
    /*adjust to path*/
    int count = 0;  
    while ((tok = strsep(&end, " ")) != NULL) {
        count += 1;
        


        if (isPath){
            
            //char new_path[50];
            //appendChar(tok, "/", strlen(tok)-1);

            strcpy(Path,  tok);
           

        }else{
            isPath = true;
        }
    }

    if(count == 1){
        strcpy(Path, "");
    }

    
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

void forkedCommand(){
    
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
        if ( (characters = getline(&buffer, &bufsize, fp)) == -1){
            buffer = "exit";
        }
        char *end, *r, *tok;
        r = end = strdup(buffer);
        bool command = false;
        //bool exit_command = false;
        
        while ((tok = strsep(&end, " ")) != NULL) {
            
            /*check if we need to run our program correctly*/
            if( ((strcmp(tok, "cd") == 0) || (strcmp(tok, "cd\n") == 0)) && (!command) ){    
                cdcommand(buffer);
                command = true; 
            }else if( ((strcmp(tok, "ls") == 0) || (strcmp(tok, "ls\n") == 0)) && (!command) ){
                
                lscommand(buffer);
                command = true;
            }else if( ((strcmp(tok, "exit") == 0) || (strcmp(tok, "exit\n") == 0)) && (!command) ){
                errorMessage(); 
            }else if( ((strcmp(tok, "path") == 0) || (strcmp(tok, "path\n") == 0)) && (!command) ){
                pathcommand(buffer);
                command = true;
            //try opening up the file 
            
            }else if(command == false){
                
                char strcommand[50];
                strcpy(strcommand, Path);
                appendChar(strcommand, "/", strlen(strcommand));

                strcat(strcommand, tok);

                pid_t pid;
                char *const parmList[] = {"/bin/sh", strcommand,  NULL};
                

                
                if (access(strcommand, X_OK) == -1 ){
                    errorMessage(); 
                }

                pid = fork(); 
                if (pid == -1) { 
                    errorMessage();
                        
                     
                }else if (pid == 0) {
                    execv("/bin/sh", parmList);
                                  
                    
                }

                

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
        
    }
    
    return(0);
}