/*
UNIVERSIDAD NACIONAL DE COSTA RICA
SISTEMAS OPERATIVOS - EIF-212
PROYECTO #1
ESTUDIANTES: DANIEL DAVID BARRIENTOS ARAYA, DANIEL MADRIGAL
PROFESOR: JOSE CALVO SUÁREZ

REFERENCIAS:
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
bool checkPipe(char **);
bool getNextLine(int *);
void pipeProcess(char **);
void pipeProcess2(char **);
void initProcess(char **);
void convertToString(char **);
bool checkAmperson(char**);
void readf();
void writef();
int fd[2];
char str[200];
char aux[200];
int n = 0;
int j=0;
int main()
{
    pid_t pid;
    pid_t pid2;
    bool flag=false;
    bool flag2=false;
    char* argv2[100];
    while (true)
    {
        pipe(fd);
        if (pipe(fd) < 0){
	    printf("Error en la llamada a pipe()");
		exit(1);
	}
	
        pid = fork();
        if (pid < 0)
        {
            printf("Error en la llamada a fork()");
            exit(1);
        }
        if (pid == 0)
        {   readf();
            convertToString(argv2);
            flag=checkPipe(argv2);
            if(flag){
                pipeProcess(argv2);
            }
            int p = execv(argv2, argv2);
        printf("%s\n", "Comando o ruta no encontrada");
                exit(-1);   
         }
        else
        {
            
            getNextLine(&pid);
            convertToString(argv2);
            flag2 = checkAmperson(argv2);
            flag=checkPipe(argv2);
            if(flag2){
                 waitpid(pid, NULL, WNOHANG);
            }else if (flag){
                pid2 = fork();
                if (pid2 < 0)
                {
                    printf("Error en la llamada a fork()");
                    exit(1);
                }
                if (pid2 == 0)
                {
                readf();
            convertToString(argv2);
            checkPipe(argv2);
            pipeProcess2(argv2);
            initProcess(argv2);
            printf("%s\n", "Comando o ruta no encontrada");
                exit(-1);   
                }
                else
                {
                    waitpid(pid, NULL, 0);
                    waitpid(pid2, NULL, 0);
                }
            }else
            {
              wait(NULL);
            }
        }
    }
    return 0;
}

void pipeProcess(char **argv)
{
    dup2(fd[1], STDOUT_FILENO);
    close(fd[0]);
    close(fd[1]);
    char *token;
    //cat shell.c --> se va| less...
    for (int i = j; i < n; i++)
    {
        token = argv[i];
        argv[i] = NULL; //cat Shell.c
    }
}
bool checkAmperson(char ** argv){
    char* ret;
    if(argv[n-1]!=NULL){
     ret = strchr(argv[n - 1], '&');
    }else{
        return false;
    }
    if (ret != NULL){
        argv[n - 1] = NULL;
        return true;
    } //ls -a
    ////////
    return false;
}
void pipeProcess2(char **argv)
{
    dup2(fd[0], STDIN_FILENO);
    close(fd[0]);
    close(fd[1]);
    char *argv2[100];
    int contador = 0;
    for (int i = j + 1; i < n; i++)
    {
        argv2[contador] = argv[i];
        contador++;
    }
    argv2[contador] = NULL;
    initProcess(argv2);
     printf("%s\n", "Comando o ruta no encontrada");
                exit(-1);
}

void convertToString(char **worker)
{
    char *token;
    token = strtok(str, " ");
    int i = 0;
    while (token != NULL)
    {
        worker[i] = token;
        token = strtok(NULL, " ");
        i++;
    }
    worker[i] = NULL;
}
bool checkPipe(char **argv)
{
    char *helpme;
    n = strlen(argv);
    for (int i = 0; i < n - 1; i++)
    {
        if(argv[i]==NULL){
            return false;
        }
        helpme = strchr(argv[i], '|');
        
        if (helpme != NULL)
        {   argv[n-1]=NULL;
            return true;
        }
        j++;
    }
    return false;
}

void readf(){
FILE* f=fopen("historial.txt","r");
 fgets(str,200,f);
    fclose(f);
}
void writef(){
FILE* f=fopen("historial.txt","w");
 fprintf(f,aux);
    fclose(f);
}
bool getNextLine(int *pid)
{
    char *username;
    username = getlogin();
    strcpy(str, "");
    printf("%s%s%s", "@", username, ">>>$");
    gets(str);
    strcpy(aux,str);
    if (strcmp(str, "exit") == 0)
    {
        kill(*pid, SIGKILL);
        exit(1);
    }
    writef();
    return true;
}
void initProcess(char **argv)
{
    int p = execvp(argv[0], argv);
    printf("%s\n", "Comando o ruta no encontrada");
    exit(-1);
}
