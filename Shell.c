/*
UNIVERSIDAD NACIONAL DE COSTA RICA
SISTEMAS OPERATIVOS - EIF-212
PROYECTO #1
ESTUDIANTES: DAVID BARRIENTOS, DANIEL MADRIGAL
PROFESOR: JOSE CALVO SUÁREZ
*/

#include <sys/types.h>
#include <unistd.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
bool shell_Loop();
int main(){
    pid_t pid;
    bool flag;
    shell_Loop();
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
}

bool shell_Loop(){
    bool flag=false;
     char *str, *fgets_return;
      int temp;
  
    char* command;
    while (flag==false)
    {
        strcpy(str, "");
        printf("%s\n","@User>>>");
      fgets_return = fgets( str, sizeof(str), stdin);
      printf("%s\n",str);
      if(str=="exit"){
          flag=true;
      }
    }
    return flag;
}
