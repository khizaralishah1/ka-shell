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
        if(buf[i] != ' ' && buf[i] != '\0')
            cmd_name[i] = buf[i];
        else
            break;
    }

    //printf("cmd_name = %s.\n", cmd_name);

    return cmd_name;
}

int cmd_get_args(char* buf, char** argv)
{
    //size_t i = 1;
    int i = 1;
    int arg_c = 1;

    for(; i < MAX_ARGS; ++i)
    {
        while(*buf == ' ')
            ++buf;
        if(*buf == '\0')
        {
            //argv[i] = (char*) malloc( sizeof(char) * ARG_LEN );
            //memset(argv[i], 0, ARG_LEN);
            return arg_c;
            //argv[i] = NULL;
        }

        //argv[i] = (char*) malloc( sizeof(char) * ARG_LEN );
        //memset(argv[i], 0, ARG_LEN);
        //char temp_arg[ARG_LEN];

        for(int k=0; k<ARG_LEN-1; ++k) // ARG_LEN=10, k<9
        {
            //printf("lp = *buf = %c.\n", *buf);
            
            if (*buf == '\0')
            {
                //argv[i] = (char*) malloc( sizeof(char) * ARG_LEN );
                //memset(argv[i], 0, ARG_LEN);
                //argv[i] = NULL;
                break;
            }
            else if (*buf != ' ') {
                //printf("cxc\n");
                argv[i][k] = *buf;//temp_arg[k] = *buf;
                //printf("cxccc\n");
                //printf("argv[%d][%d] = %c.\n", i, k, argv[i][k]);
            }
            else
            {
                while(*buf == ' ' && *buf != '\0') ++buf;
                break;
            }

            ++buf;

            if(k == ARG_LEN-2 || *buf == '\0' || *buf == ' ') {
                argv[i][k+1] = '\0';//temp_arg[k+1] = '\0';
                ++arg_c;
                //argv[i] = temp_arg;
                //printf("in, argv[%d] = %s.\n", i, argv[i]);
                break;
            }
        }
    }

    //argv[i] = (char*) malloc( sizeof(char) * ARG_LEN );
    //argv[i] = NULL;

    //return argv;
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

void cmd_parse_run(char* buf)
{
    int x = 0;

    // for(int i=0; i < LEN - x; ++i)
    // {
    //     printf("buf[%d] = %c.\n", i, buf[i]);
    // }

    while(*buf == ' ' && *buf != '\0') { // removes initial whitespaces
        ++buf;
        ++x;
    }

    if(*buf == '\0')
        exit(EXIT_SUCCESS);

    //extract the cmd name
    char* cmd_name = get_cmd_name(buf);

    // for(int i=0; i < LEN - x; ++i)
    // {
    //     printf("buf[%d] = %c,\n", i, buf[i]);
    // }

    //extract the arguments
    while(*buf != ' ' && *buf != '\0') { // go through the cmd_name
        ++buf;
        ++x;
    }
    while(*buf == ' ' && *buf != '\0') { // go through spaces after it
        ++buf;
        ++x;
    }

//    printf("seg\n");

    char **arg_var = (char**) malloc(MAX_ARGS * sizeof(char*));

    for(int i=0; i < MAX_ARGS; ++i)
    {
        arg_var[i] = (char* ) malloc(ARG_LEN * sizeof(char));
    }

    arg_var[0] = cmd_name;

    // for(int i=1; i<MAX_ARGS; ++i)
    // {
    //     arg_var[i] = NULL;
    // }
    //arg_var[0] = (char *) malloc(sizeof(char) * ARG_LEN);
    //arg_var[0] = cmd_name;

    //printf("seg2, *buf == %c\n", *buf);
    
    //if(*buf != '\0')
    int arg_c = cmd_get_args(buf, arg_var); // the first character of first argument is inclusive
    //printf("seg3\n");
    
    for(int i=arg_c; i < MAX_ARGS; ++i)
    {
        arg_var[i] = NULL;
    }

    // for(int i=0; i<MAX_ARGS; ++i)
    // {
    //     printf("arg[%d] = %s.\n", i, arg_var[i]);
    // }
    
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
    char * buff = (char *)malloc(LEN * sizeof(char)); // buffer for storing user input
    memset(buff, 0, LEN * sizeof(char));

    char * const old_buff = buff;

    while(1)
    {
        printf("kash>");
        cmd_input(buff);
        cmd_parse_run(buff);
        memset(buff, 0, LEN * sizeof(char)); // puts all 0s in the mem location from address stored in buf to buf+LEN
        buff = old_buff;
    }
    
    free(buff);

    return EXIT_SUCCESS;
}

