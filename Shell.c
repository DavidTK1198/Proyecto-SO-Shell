/*
UNIVERSIDAD NACIONAL DE COSTA RICA
SISTEMAS OPERATIVOS - EIF-212
PROYECTO #1
ESTUDIANTES: DAVID BARRIENTOS, DANIEL MADRIGAL
PROFESOR: JOSE CALVO SUÁREZ

REFERENCIAS:
https://overiq.com/c-programming-101/array-of-strings-in-c/
https://www.ibm.com/support/knowledgecenter/SSLTBW_2.2.0/com.ibm.zos.v2r2.bpxbd00/rtgtlgr.htm
*/
#include <sys/types.h>
#include <unistd.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <stdlib.h>
#include <wait.h>
//int n=0;
int tuberia[2];
char str[80];
 //char test[15][15]={};
 void initProcess();
int main(){
    pid_t pid;
    char* username;
    username=getlogin();
    pipe(tuberia);
    pid = fork();
    if (pid < 0)
    {
        printf("Error en la llamada fork()");
        _exit(-1);
    }
    else if (pid == 0){
        printf("hola soy el hijo");
        initProcess();
    }
    else{
        while (true){
        strcpy(str, "");
        printf("%s%s%s", "@", username, ">>>");
        gets(str);
        if (strcmp(str, "exit") == 0){
                close(tuberia[1]);
                close(tuberia[0]);
                break;
                }
        write(tuberia[1],str,sizeof(str));
            wait(NULL);
            pid = fork();
            if(pid==0){
            initProcess();
            }
  
        }
    }
    
    return 0;
    
}

void initProcess(){
    read(tuberia[0], str, sizeof(str));
        sleep(3);
        //n = strlen(str);
        //int contador=0;
        //char aux[30] = "";
        //int j=0;
         char *token;
         char* argv[100];                   //split command into separate strings
    token = strtok(str," ");
    int i=0;
    while(token!=NULL){
        argv[i]=token;      
        token = strtok(NULL," ");
        i++;
    }
    argv[i]=NULL;
        //for (int i = 0; i <= n; i++)
        //{
            ///if (str[i] == ' ' || str[i]=='\0'){
          //strcpy(test[contador], aux);
            //memset(aux, 0, sizeof(aux));//./bin/ps ps 
            //contador++;
            //j=0;
            //}else{ //" shell.c"
            //aux[j] = str[i];
            //j++;
          //  }
            
        //}
        int p=execvp(argv[0],argv);
        printf("%s","Comando o ruta no encontrada"); 
        exit(-1);
}