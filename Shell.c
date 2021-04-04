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
int fd[2];
char str[80];
int n = 0;
bool checkPipe(char**);
bool getNextLine(char**);
void pipeProcess();
void pipeProcess2();
void initProcess();
void  convertToString (char**);
int main()
{
    pid_t pid;
    pid_t pid2;
    bool flag=false;
    char* argv[100];
    if (pid < 0)
    {
        printf("Error en la llamada fork()");
        _exit(-1);
    }
    else if (pid == 0)
    {       read(fd[0], str, sizeof(str));
                convertToString(argv);
                flag=checkPipe(argv);
                initProcess();
    }
    else
    {
        while (true)
        {   pipe(fd);
            pid = fork();
            if (pid == 0)
            {   read(fd[0], str, sizeof(str));
                convertToString(argv);
                flag=checkPipe(argv);
                initProcess();
            }
            convertToString(argv);
            flag=getNextLine(argv);
            if(flag){
                pid2=fork();
                 if(pid < 0){
        printf("Error en la llamada fork()");

        _exit(-1);
    }else if(pid==0){
       read(fd[0], str, sizeof(str));
                convertToString(argv);
                flag=checkPipe(argv);
                initProcess();
    }else{
       write(fd[1], str, sizeof(str));
    }
            }
            if (strcmp(str, "exit") == 0)
            {
                kill(pid, SIGKILL);
                break;
            }
            n = strlen(str);
            close(fd[1]);
            close(fd[0]);
            if (str[n - 1] == '&')
            {
                waitpid(pid, NULL, WUNTRACED);
                  int c = getchar();
            }
            else
            {
                if(flag){
                    waitpid(pid,NULL,0);
                waitpid(pid2,NULL,0);
                }else{
                wait(NULL);
                }
            }
        }
    }
    return 0;
}

void pipeProcess(){
        dup2(fd[1],STDOUT_FILENO);
                close(fd[0]);
                close(fd[1]);
}
void pipeProcess2(){
    dup2(fd[0],STDIN_FILENO);
        close(fd[0]);
        close(fd[1]);
}

void  convertToString (char** worker){
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
bool checkPipe(char** argv){
    char* helpme;
for (int i = 0; i < n; i++)
        { 
            helpme = strchr(argv[i], '|');
            if (helpme != NULL)
            {
                return true;
            
            }
        }
    }
bool getNextLine(char** ar){
        char *username;
         username = getlogin();
            strcpy(str, "");
            printf("%s%s%s", "@", username, ">>>$");
            gets(str);
            convertToString(ar);
            if(checkPipe(ar)){
            write(fd[1], str, sizeof(str));
            return true;
            }
            write(fd[1], str, sizeof(str));
            return false;
}
void initProcess()
{ //hace faltar hacer los if & y |
    n = strlen(str);
    char *argv[100];
    int j = 0;
    char aux[80] = "";
    if (str[n - 1] == '&')
    {
        str[n - 1] = (char)13;
    }
    convertToString(argv);
    int p = execvp(argv[0], argv);
    printf("%s\n", "Comando o ruta no encontrada");
    exit(-1);
}
