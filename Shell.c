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
int n = 0;
int tuberia[2];
char str[80];
//char test[15][15]={};
void initProcess();
int main()
{
    pid_t pid;
    char *username;
    username = getlogin();
    pipe(tuberia);
    char *argv[100];
    if (pid < 0)
    {
        printf("Error en la llamada fork()");
        _exit(-1);
    }
    else if (pid == 0)
    {
        initProcess();
    }
    else
    {

        while (true)
        {
            pid = fork();
            if (pid == 0)
            {
                initProcess();
            }
            strcpy(str, "");
            printf("%s%s%s", "@", username, ">>>$");
            gets(str);

            if (strcmp(str, "exit") == 0)
            {
                kill(pid, SIGKILL);
                break;
            }
            write(tuberia[1], str, sizeof(str));
            n = strlen(str);
            if (str[n - 1] == '&')
            {
                waitpid(pid, NULL, WUNTRACED);
            }
            else
            {
                wait(NULL);
            }
        }
    }
    close(tuberia[1]);
    close(tuberia[0]);
    return 0;
}
void initProcess()
{ //hace faltar hacer los if & y |
    read(tuberia[0], str, sizeof(str));
    n = strlen(str);
    char *token;
    char *argv[100];
    int j = 0;
    char aux[80] = "";
    if (str[n - 1] == '&')
    {
        str[n - 1] = (char)13;
    }
    token = strtok(str, " ");
    int i = 0;
    while (token != NULL)
    {
        argv[i] = token;
        token = strtok(NULL, " ");
        i++;
    }
    argv[i] = NULL;
    int p = execvp(argv[0], argv);
    printf("%s\n", "Comando o ruta no encontrada");
    exit(-1);
}
