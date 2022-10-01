#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>

#define LEN 15
#define CMD_NAME_LEN 10
#define MAX_ARGS 10
#define ARG_LEN 10

typedef struct cmd cmd;
struct cmd {
    char** name;
    char type; // exec ' ', will implement later: pipe '|', redir '<' or '>'

};

char* get_cmd_name(char* buf)
{
    char* cmd_name = (char*) malloc( sizeof(char) * CMD_NAME_LEN );
    memset(cmd_name, 0, CMD_NAME_LEN);

    for(size_t i = 0; i < CMD_NAME_LEN-1; ++i)
    {
        if(buf[i] != ' ' && buf[i] != '\n')
            cmd_name[i] = buf[i];
        else
            break;
    }

    printf("cmd_name = %s\n", cmd_name);

    return cmd_name;
}

char** cmd_get_args(char* buf)
{
    char** argv = NULL;
    size_t i = 0;

    for(; i < MAX_ARGS; ++i)
    {
        if(*buf == '\0' || *buf == '\n')
        {
            argv[i] = (char*) malloc( sizeof(char) * ARG_LEN );
            argv[i] = NULL;
            return argv;
        }

        argv[i] = (char*) malloc( sizeof(char) * ARG_LEN );
        memset(argv[i], 0, ARG_LEN);

        for(int k=0; k<ARG_LEN; ++k)
        {
            if(*buf == '\0')
            {
                argv[i] = (char*) malloc( sizeof(char) * ARG_LEN );
                argv[i] = NULL;
                return argv;
            }
            if(*buf != ' ') argv[i][k] = *buf;
            else
            {
                while(*buf == ' ') ++buf;
                break;
            }
            ++buf;
        }
    }

    argv[i] = (char*) malloc( sizeof(char) * ARG_LEN );
    argv[i] = NULL;

    return argv;
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

void cmd_parse_run(char* buf)
{
    while(*buf == ' ' && *buf != '\0') ++buf; // removes initial whitespaces

    if(*buf == '\0')
        exit(EXIT_SUCCESS);

    //extract the cmd name
    char* cmd_name = get_cmd_name(buf);

    for(int i=0; i<LEN; ++i)
    {
        printf("buf[%d] = %c\n", i, buf[i]);
    }

    //extract the arguments
    while(*buf != ' ' && *buf != '\0') ++buf; // go through the cmd_name
    while(*buf == ' ' && *buf != '\0') ++buf; // go through spaces after it

    char** arg_var = NULL;
    
    if(*buf != '\0')
        arg_var = cmd_get_args(buf); // the first character of first argument is inclusive

    for(int i=0; i<MAX_ARGS; ++i)
    {
        printf("arg[%d] = %s\n", i, arg_var[i]);
    }
    
    int pid = fork();

    if(pid == 0)
    {
        execvp(cmd_name, arg_var);
    }
    else
    {
        wait(&pid);
        free(cmd_name);
        free(arg_var);
    }
}

int main()
{
    char * buff = (char *)malloc(LEN * sizeof(char)); // buffer for storing user input
    memset(buff, 0, LEN * sizeof(char));
    char * const old_buff = buff;

    while(1)
    {
        memset(buff, 0, LEN * sizeof(char)); // puts all 0s in the mem location from address stored in buf to buf+LEN
        printf("kash>");
        cmd_input(buff);
        cmd_parse_run(buff);
        buff = old_buff;
    }
    
    free(buff);

    return EXIT_SUCCESS;
}

