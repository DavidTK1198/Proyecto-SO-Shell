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
#include <wait.h>
void VerificacionDeProcesos(pid_t pid ,char test [15][15],char str [] );
int main(int argc, char *argv[]){

    pid_t pid;
    int n = argc;
    char* username;
    username=getlogin();
    char str[80] = "";
    int tuberia[2];
    pipe(tuberia);
    char test[15][15]={};
    pid = fork();
    
    if (pid < 0)
    {
        printf("Error en la llamada fork()");
        _exit(-1);
    }
    else if (pid == 0)
    {
        printf("hola soy el hijo");
        read(tuberia[0], str, sizeof(str));
   sleep(3);
        n = strlen(str);
        int contador=0;
        char aux[30] = "";
        for (int i = 0; i <= n - 1; i++)
        {
            if (str[i] == ' ')
            {
          strcpy(test[contador], aux);
            //strncpy(aux, "\0", sizeof(aux));// ./bin/ps ps 
            contador++;
            }
            aux[i] = str[i];
        }
        execlp(test[0], test[0], NULL);
    }
    else
    {
        while (true)
        {

            if (strcmp(str, "exit") == 0)
            {
                break;
                close(tuberia[1]);
                close(tuberia[0]);
            }
        strcpy(str, "");
        printf("%s%s%s", "@", username, ">>>");
        gets(str);
        write(tuberia[1],str,sizeof(str));
            wait(NULL);
            pid = fork();
            VerificacionDeProcesos(pid,test,str);
  
        }
    }
    
    return 0;
    //hola
}

void VerificacionDeProcesos(pid_t pid,char test [15][15], char str[]){
    sleep(2);
}