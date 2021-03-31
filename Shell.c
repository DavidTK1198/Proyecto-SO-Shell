/*
UNIVERSIDAD NACIONAL DE COSTA RICA
SISTEMAS OPERATIVOS - EIF-212
PROYECTO #1
ESTUDIANTES: DAVID BARRIENTOS, DANIEL MADRIGAL
PROFESOR: JOSE CALVO SUÁREZ

REFERENCIAS:
https://www.ibm.com/support/knowledgecenter/SSLTBW_2.2.0/com.ibm.zos.v2r2.bpxbd00/rtgtlgr.htm
*/

#include <sys/types.h>
#include <unistd.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <wait.h>
int main(){

    pid_t pid;
    int n = 0;
    char* username;
    username=getlogin();
    bool flag=false;
    char str[80]="";
    int tuberia[2];
    pipe(tuberia);
    char* test[] = {};
    pid=fork();
    
    while (flag==false)
    {
    if(strcmp(str,"exit")==0){
        flag=true;
    }
    if(pid < 0){
        printf("Error en la llamada fork()");

        _exit(-1);
    }else if(pid==0){
        raise(SIGSTOP);
        sleep(5);
        printf("hola soy el hijo");
        int contador = 0;
        close(tuberia[1]);
        read(tuberia[0],str,sizeof(str));
        n = strlen(str);
        char aux[30] = "";
        for(int i=0; i<n-1; i++){
            if(str[i] == ' '){
                test[contador] = aux;
                contador++;
            }
            aux[i] = str[i];
        }
        execlp(test[0],test[0],NULL);


    }else{
        waitpid(pid,NULL,WUNTRACED);
        close(tuberia[0]);
        strcpy(str,"");
        printf("%s%s%s","@",username,">>>");
        gets(str);
        printf("%s\n",str);
        write(tuberia[1],str,sizeof(str));
        printf("hola soy el padre\n");
        wait(NULL);
        kill(pid,SIGCONT);
        close(tuberia[1]);
    }
      
    }
    
    return 0;
    //hola
}