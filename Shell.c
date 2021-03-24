/*
UNIVERSIDAD NACIONAL DE COSTA RICA
SISTEMAS OPERATIVOS - EIF-212
PROYECTO #1
ESTUDIANTES: DAVID BARRIENTOS, DANIEL MADRIGAL
PROFESOR: JOSE CALVO SUÁREZ
*/
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdbool.h>
bool shell_Loop();
int main(){
    pid_t pid;
    bool flag;
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
