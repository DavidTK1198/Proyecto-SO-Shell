/*
UNIVERSIDAD NACIONAL DE COSTA RICA
SISTEMAS OPERATIVOS - EIF-212
PROYECTO #1
ESTUDIANTES: DANIEL DAVID BARRIENTOS ARAYA, DANIEL MADRIGAL
PROFESOR: JOSE CALVO SUÁREZ

REFERENCIAS:
https://linux.die.net/man/2/waitpid
https://overiq.com/c-programming-101/array-of-strings-in-c/
https://www.guru99.com/c-tokens-keywords-identifier.html
https://stackoverflow.com/questions/21248840/example-of-waitpid-in-use
https://www.tutorialspoint.com/c_standard_library/c_function_strtok.html
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
void initProcess();
void get_Line();        //primero  * segundo
char str[80];       //cat shell.c | less los 2
int main(int argc, char *argv[])
{
    pid_t pid;
    pid_t pid2;
    int n = argc;
    int fd[2];
    bool flag = false;
    bool flag2 = false;
    pipe(fd);
    char *ret;
    char *helpme;
    int j=0;
    if (n > 2)
    {
        for (int i = 0; i < n; i++)
        { //cat Shell.c | less
            helpme = strchr(argv[i], '|');
            if (helpme != NULL)
            {
                flag2 = true;
                break;
            }
            j++;
        }
    }
    //////////////
    ret = strchr(argv[n - 1], '&');
    if (ret != NULL){
        argv[n - 1] = NULL;
        flag = true;
    } //ls -a
    ////////
    ret = strchr(argv[0], '/');
    if (n < 2 && ret != NULL){
        get_Line();
    }

        pid = fork();
        if (pid < 0)
        {
            printf("Error en la llamada fork()");
            _exit(-1);
        }
        else if (pid == 0){
            if (flag2){  
                dup2(fd[1],STDOUT_FILENO);
                close(fd[0]);
                close(fd[1]);
                char* token;
                //cat shell.c --> se va| less ssss s...
                for(int i=j;i<n;i++){
                    token=argv[i];
                    argv[i]=NULL;//cat Shell.c
                }
                int c = execvp(argv[0], argv);
            }
            ///////////////
            int p = execvp(argv[0], argv);
            printf("%s\n", "Comando o ruta no encontrada");
            exit(-1);
        }else{
        if(flag2){
            pid2=fork();
            if(pid2==0){
                dup2(fd[0],STDIN_FILENO);
                close(fd[0]);
                close(fd[1]);
                char *argv2[100];
                int contador=0;
                for(int i=j+1;i<n;i++){
                    argv2[contador]=argv[i];
                    contador++;
                }
                argv2[contador]=NULL;
                int pp = execvp(argv2[0], argv2);
                printf("%s\n", "Comando o ruta no encontrada");
                exit(-1);
            }else{
            goto next;
            }
        }
    }

    if (flag)
    {
        waitpid(pid, NULL, WNOHANG);
    }
    else
    {
        wait(NULL);
    }
   
    if (flag)
    {
        int c = getchar();
    }
    next:
    close(fd[0]);
    close(fd[1]);
    if(flag2){
        waitpid(pid,NULL,0);
        waitpid(pid2,NULL,0);
    }
    get_Line();
    return 0;
}

void get_Line()
{//.exe 1 34 5 ping  | se
    int n;
    char *username;
    username = getlogin();
    char *token;
    strcpy(str, "");
    printf("%s%s%s", "@", username, ">>>$");
    gets(str);
    char *argv2[100];
    n = strlen(str);
    token = strtok(str, " ");
    int i = 0;
    if (strcmp(str, "exit") == 0)
    {
        exit(1);
    }
    while (token != NULL)
    {
        argv2[i] = token;
        token = strtok(NULL, " ");
        i++;
    }
    argv2[i] = NULL; //ls-a
    int p = execv("./a.out", argv2);
}
