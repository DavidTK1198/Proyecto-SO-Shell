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
https://linux.die.net/man/2/waitpid
https://man7.org/linux/man-pages/man2/waitid.2.html
https://stackoverflow.com/questions/21248840/example-of-waitpid-in-use
https://www.tutorialspoint.com/c_standard_library/c_function_strncpy.htm#:~:text=The%20C%20library%20function%20char,be%20padded%20with%20null%20bytes
https://www.tutorialspoint.com/c_standard_library/c_function_strtok.html
https://www.ibm.com/support/knowledgecenter/SSLTBW_2.2.0/com.ibm.zos.v2r2.bpxbd00/rtgtlgr.htm
https://www.youtube.com/watch?v=pO1wuN3hJZ4&t=7s
https://www.youtube.com/watch?v=oxWxcYoJJdM&t=666s
https://www.youtube.com/watch?v=Mqb2dVRe0uo&t=560s
https://www.youtube.com/watch?v=6xbLgZpOBi8
*/

#include <sys/types.h>
#include <unistd.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <stdlib.h>
#include <wait.h>
void get_Line(bool,char*);
void writef();
void append();
//bool checkPipe(char **);
//void pipeProcess(char **);
//void pipeProcess2(char **);
//void initProcess(char **);
//bool checkAmperson(char**);
char str[300];
char commad_list[100][100];
int p=0;
bool init_Array();
void print_Array();
void mov_Array(char**,int);
char* get_Element(int);
int main(int argc, char *argv[])
{
    pid_t pid;
    pid_t pid2;
    int n = argc;
    int fd[2];
    char* ruta=argv[0];
    bool flag = false;
    bool flag2 = false;
    pipe(fd);
    char *ret;
    char *helpme;
    if(n>=2){
        move_Array(argv,argc);
        n--;
    }
    int j = 0;
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
    if (ret != NULL)
    {
        argv[n - 1] = NULL;
        flag = true;
    } //
    ////////
    ret = strchr(argv[0], '/');
    if (n < 2 && ret != NULL)
    {
        get_Line(false,ruta);
    }

    pid = fork();
    if (pid < 0)
    {
        printf("Error en la llamada fork()");
        _exit(-1);
    }
    else if (pid == 0)
    {
        if (flag2)
        {
            dup2(fd[1], STDOUT_FILENO);
            close(fd[0]);
            close(fd[1]);
            char *token;
            //cat shell.c --> se va| less ssss s...
            for (int i = j; i < n; i++)
            {
                token = argv[i];
                argv[i] = NULL; //cat Shell.c
            }
            int c = execvp(argv[0], argv);
        }
        ///////////////
        int p = execvp(argv[0], argv);
        printf("%s\n", "Comando o ruta no encontrada");
        exit(-1);
    }
    else
    {
        if (flag2)
        {
            pid2 = fork();
            if (pid2 == 0)
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
                int pp = execvp(argv2[0], argv2);
                printf("%s\n", "Comando o ruta no encontrada");
                exit(-1);
            }
            else
            {
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
    if (flag2)
    {
        waitpid(pid, NULL, 0);
        waitpid(pid2, NULL, 0);
    }
    get_Line(true,ruta);
    return 0;
}
void append()
{
    FILE *f = fopen("historial.txt", "a");
    fprintf(f, str);
    fclose(f);
}

void writef()
{
    FILE *f = fopen("historial.txt", "w");
    fprintf(f, str);
    fclose(f);
}
void print_Array(){

    int n=p;
        for(int i=n-1;i>=0;i--){
        printf("%d%s%s\n",i+1," ",commad_list[i]);
    }
}

void move_Array(char** argv,int k){
    for(int i=0;i<k;i++){
        argv[i]=argv[i+1];
    }
}

bool init_Array()
{
    int contador=0;
    int j=0;
    p=0;
    FILE *f = fopen("historial.txt", "r");
    if (f == NULL)
    {
        printf("No se encuentra el archivo");
    }
    else{
    while (!feof(f))
    {
        fgets(str, 300, f);
        contador++;
        p++;
    }
    contador--;
    p--;
    fseek(f,0,SEEK_SET);
    while (!feof(f))
    {
        fgets(str, 300, f);
        j=strlen(str);
        str[j-1]='\0';
        strcpy(commad_list[contador-1],str);
        contador--;
    }
    fclose(f);
    return true;
}
}
void get_Line(bool flag,char* argv)
{
    int n;
    char *username;
    username = getlogin();
    char *token;
    char aux[80];
    strcpy(str, "");
    if (flag)
    {   if(p==0){
        init_Array();
        }
    }
    printf("%s%s%s", "@", username, ">>>$");
    gets(str);
    if (strcmp(str, "exit") == 0)
    {
        exit(1);
    }
    if (strcmp(str, "historial") == 0)
    {   print_Array();
        append();
        get_Line(true,argv);   
    }
    bool ayudante = false;
    if(str[0] == '!'){
        for(int p=0; p<(strlen(str)-1);p++){
            if(isdigit(str[p])){
               
            }else{
                ayudante = true;
                break;
            }

        }
    }
    strcpy(aux, str);
    n = strlen(str);
    str[n] = '\n';
    if (flag == false)
    {
        writef();
    }
    else
    {
        append();
    }
    strcpy(str, aux);
    char *argv2[100];
    n = strlen(str);
    token = strtok(str, " ");
    int i = 1;
    if (strcmp(str, "exit") == 0)
    {
        exit(1);
    }
    argv2[i-1]=argv;
    while (token != NULL)
    {
        argv2[i] = token;
        token = strtok(NULL, " ");
        i++;
    }
    argv2[i] = NULL; //ls-a
    int p = execv(argv, argv2);
}
