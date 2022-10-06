#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>

#define LEN 20
#define CMD_NAME_LEN 20
#define MAX_ARGS 10
#define ARG_LEN 50

typedef struct cmd cmd;
struct cmd {
    char** name;
    char type; // exec ' ', will implement later: pipe '|', redir '<' or '>'
};

char* get_cmd_name(char* buf)
{
    char* cmd_name = (char*) malloc( sizeof(char) * CMD_NAME_LEN );

    if(cmd_name == NULL)
    {
        dprintf(STDERR_FILENO, "Error: buffer could not be allocated\nExiting...\n");
        exit(EXIT_FAILURE);
    }

    memset(cmd_name, 0, CMD_NAME_LEN);

    for(size_t i = 0; i < CMD_NAME_LEN-1; ++i)
    {
        if(buf[i] != ' ' && buf[i] != '\0')
            cmd_name[i] = buf[i];
        else
            break;
    }

    return cmd_name;
}

int cmd_get_args(char* buf, char** argv)
{
    size_t i = 1;
    int arg_c = 1;

    for(; i < MAX_ARGS; ++i)
    {
        while(*buf == ' ') ++buf;
        if(*buf == '\0') return arg_c;

        for(size_t k=0; k<ARG_LEN-1; ++k)
        {
            if (*buf == '\0') break;
            else if (*buf != ' ') argv[i][k] = *buf;
            else
            {
                while(*buf == ' ' && *buf != '\0') ++buf;
                break;
            }

            ++buf;

            if(k == ARG_LEN-2 || *buf == '\0' || *buf == ' ') {
                argv[i][k+1] = '\0';
                ++arg_c;
                break;
            }
        }
    }

    return arg_c;
}

char* cmd_input(char* buf)
{
    char chr = 'x';
    size_t i = 0;

    while(i < LEN-1 && chr != '\n')
    {
        scanf("%c", &chr);
        if(chr != '\n') buf[i] = chr;
        else break;
        ++i;
    }

    return buf;
}

char* remove_whitespaces(char* buf)
{
    while(*buf == ' ' && *buf != '\0') {
        ++buf;
    }

    // if(*buf == '\0')
    //     return buf;

    return buf;
}

void cmd_parse_run(char* buf)
{
    buf = remove_whitespaces(buf);

    //extract the cmd name
    char* cmd_name = get_cmd_name(buf);

    //extract the arguments
    while(*buf != ' ' && *buf != '\0') { // go through the cmd_name
        ++buf;
    }
    
    buf = remove_whitespaces(buf);

    char **arg_var = (char**) malloc(MAX_ARGS * sizeof(char*));

    for(int i=0; i < MAX_ARGS; ++i)
    {
        arg_var[i] = (char* ) malloc(ARG_LEN * sizeof(char));
    }

    arg_var[0] = cmd_name;
    
    int arg_c = cmd_get_args(buf, arg_var);
    
    for(int i=arg_c; i < MAX_ARGS; ++i)
    {
        arg_var[i] = NULL;
    }
    
    int pid = fork();

    if(pid == 0)
    {
        execvp(cmd_name, arg_var);
    }
    else
    {
        wait(&pid);
    }
}

int main()
{
    char* buff = (char *)malloc(LEN * sizeof(char)); // buffer for storing user input
    
    if(buff == NULL)
    {
        dprintf(STDERR_FILENO, "Error: buffer could not be allocated\nExiting...\n");
        exit(EXIT_FAILURE);
    }

    memset(buff, 0, LEN * sizeof(char));

    char* const old_buff = buff;

    while(1)
    {
        printf("kash>");
        cmd_input(buff);
        cmd_parse_run(buff);
        buff = old_buff;
        memset(buff, 0, LEN * sizeof(char)); // puts all 0s in the mem location from address stored in buf to buf+LEN
    }
    
    free(buff);

    return EXIT_SUCCESS;
}
