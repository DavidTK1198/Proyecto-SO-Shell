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
int main(){

    pid_t pid;
    char* username;
    username=getlogin();
     bool flag=false;
     char str[300]="";
    while (flag==false)
    {
    strcpy(str,"");
      printf("%s%s%s","@",username,">>>");
      gets(str);
      int n=strlen(str);
      printf("%s\n",str);
      if(strcmp(str,"exit")==0){
          flag=true;
      }
      
    }
    pid=fork();
    if(pid < 0){
        printf("Error en la llamada fork()");

        _exit(-1);
    }else if(pid==0){
        execlp("/bin/ps","ps",NULL);
    }else{
        wait(NULL);
        printf("hola soy el padre\n");
    }
    return 0;
    //hola
}