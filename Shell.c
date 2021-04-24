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
https://www.youtube.com/watch?v=PErrlOx3LYE
https://stackoverflow.com/questions/1023955/signals-dont-re-enable-properly-across-execv
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
#include <ctype.h>
char str[300];
char commad_list[100][100];//almacena los comandos del usuario
int p = 0;
char *ruta;
bool init_Array();
void print_Array();
void mov_Array(char **, int);
char *get_Element(int);
void get_Line(bool);
void writef();
void append();
void handler(int);
bool pipeProcess(char **, int *, int *);
void secondChildProcess(char **, int *,int*);
void firstChildProcess(char **, int *,int*);
int main(int argc, char *argv[])
{
    pid_t pid;
    pid_t pid2;
    struct sigaction sa;      //Necesario para realizar el background
    sa.sa_flags = SA_NODEFER; //referencia arriba
    sa.sa_handler = &handler;
    sigemptyset(&sa.sa_mask);
    int n = argc;
    int fd[2];
    ruta = argv[0];
    bool flag = false;//banderas para capturar el | o &
    bool flag2 = false;
    pipe(fd);
    char *ret;
    if (n >= 2){
        move_Array(argv, argc);
        n--;
    }
    int j = 0;
    if (n > 2){
        flag2 = pipeProcess(argv, &n, &j);
    }
    //Verificación del &
    ret = strchr(argv[n - 1], '&');
    if (ret != NULL){
        argv[n - 1] = NULL;
        flag = true;
        sigaction(SIGCHLD, &sa, 0);
    } //activa la  funcion para correr en el backround
    
    ret = strchr(argv[0], '/');//En caso de ser la 
    if (n < 2 && ret != NULL){//primera ejecución
        get_Line(false);    //se vuelve a llamar
    }               // a si mismo para rellenar argv
    pid = fork();
    if (pid < 0)
    {
        printf("Error en la llamada fork()");
        _exit(-1);
    }
    else if (pid == 0)
    {
        if (flag)
        {
            printf("[1] %d\n", getpid());
        }
        if (flag2)
        {
            
            dup2(fd[1], STDOUT_FILENO);
            close(fd[0]);
            close(fd[1]);
            firstChildProcess(argv,&n,&j);
//se llama esta funcion en caso de encontrar un | 
        }        
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
               secondChildProcess(argv,&n,&j);
//continuación del pipe por medio del segundo hijo
            }
            else
            {
                goto next;
            }
        }
    }

    if (flag)
    {
        waitpid(pid, NULL, 0);
    }
    else
    {
        wait(NULL);
    }
next:
    close(fd[0]);
    close(fd[1]);
    if (flag2)
    {
        waitpid(pid, NULL, 0);
        waitpid(pid2, NULL, 0);
    }
    get_Line(true);
    return 0;
}

bool pipeProcess(char **argv, int *n, int *j)
{
    char *helpme;
    for (int i = 0; i < *n; i++)
    {
        helpme = strchr(argv[i], '|');
        if (helpme != NULL)
        {
            return true;
        }
        *j = *j + 1;
    }
    return false;
}

void firstChildProcess(char ** argv, int * n,int* j){
         char *token;
            
            for (int i = *j; i < *n; i++)
            {
                token = argv[i];
                argv[i] = NULL; 
            }
            int c = execvp(argv[0], argv);
}
void secondChildProcess(char ** argv, int * n,int* j)
{
    char *argv2[100];
    int contador = 0;
    for (int i = *j + 1; i < *n; i++){
        argv2[contador] = argv[i];
        contador++;
    }
    argv2[contador] = NULL;
    int pp = execvp(argv2[0], argv2);
    printf("%s\n", "Comando o ruta no encontrada");
    exit(-1);
}
void handler(int sig)
{
    printf("[1] + Hecho\n");
    get_Line(true);
}
void append()
{
    FILE *f = fopen("historial.txt", "a");
    fprintf(f, str);
    fclose(f);
}

char *get_Element(int s)
{
    s--;
    for (int i = 0; i < p; i++)
    {
        if (i == s)
        {
            return commad_list[i];
        }
    }
    return NULL;
}
void writef()
{
    FILE *f = fopen("historial.txt", "w");
    fprintf(f, str);
    fclose(f);
}
void print_Array()
{

    int n = p;
    for (int i = n - 1; i >= 0; i--)
    {
        printf("%d%s%s\n", i + 1, " ", commad_list[i]);
    }
}

void move_Array(char **argv, int k)
{
    for (int i = 0; i < k; i++)
    {
        argv[i] = argv[i + 1];
    }
}

bool init_Array()
{
    int contador = 0;
    int j = 0;
    p = 0;
    FILE *f = fopen("historial.txt", "r");
    if (f == NULL)
    {
        printf("No se encuentra el archivo");
    }
    else
    {
        while (!feof(f))
        {
            fgets(str, 300, f);
            contador++;
            p++;
        }
        contador--;
        p--;
        fseek(f, 0, SEEK_SET);
        while (!feof(f))
        {
            fgets(str, 300, f);
            j = strlen(str);
            str[j - 1] = '\0';
            strcpy(commad_list[contador - 1], str);
            contador--;
        }
        fclose(f);
        return true;
    }
}
void get_Line(bool flag)
{
    int n;
    char *username;
    username = getlogin();
    char *token;
    char aux[80];
    if (flag)
    {
        init_Array();
    }
    printf("%s%s%s", "@", username, ">>>$");
    strcpy(str, "");
    memset(str, '\0', sizeof(str));
    gets(str);
    if (strcmp(str, "exit") == 0)
    {
        exit(1);
    }
    if (strcmp(str, "historial") == 0)
    {
        n = strlen(str);
        str[n] = '\n';
        append();
        init_Array();
        print_Array();
        get_Line(true);
    }
    bool ayudante = false;
    if (str[0] == '!')
    {
        n = strlen(str);
        for (int p = 0; p < n - 1; p++)
        {
            str[p] = str[p + 1];
            if (isdigit(str[p]))
            {
            }
            else
            {
                ayudante = true;
                printf("Error!! se encontro un caracter\n");
                break;
            }
        }
        str[n - 1] = '\0';
        if (ayudante == false)
        {
            n = atoi(str);
            token = get_Element(n);
            if (token != NULL)
            {
                printf("El comando elegido fue %s\n", token);
                get_Line(true);
            }
            else
            {
                printf("No se encontro el comando");
                get_Line(true);
            }
        }
        else
        {
            get_Line(true);
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
    argv2[i - 1] = ruta;
    while (token != NULL)
    {
        argv2[i] = token;
        token = strtok(NULL, " ");
        i++;
    }
    argv2[i] = NULL; //ls-a
    int p = execv(ruta, argv2);
}
