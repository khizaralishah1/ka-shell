#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>

#define LEN 10
#define CMD_NAME_LEN 10

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
        if(buf[i] != ' ')
            cmd_name[i] = buf[i];
        else
            break;
    }

    return cmd_name;
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
};

void cmd_parse(char* buf)
{
    while(*buf == ' ') ++buf; // removes initial whitespaces

    //extract the cmd name
    char* cmd_name = get_cmd_name(buf);

    int pid = fork();

    if(pid == 0)
        execvp(cmd_name, NULL);
    else
    {
        wait(NULL);
        free(cmd_name);
    }     
};

int main()
{
    
    // printf("a\n");
    // for(int i=0; i<LEN; ++i)
    // {
    //     printf("%p = %c\n", &buff[i], buff[i]);
    // }
    char * buff = (char *)malloc(LEN * sizeof(char)); // buffer for storing user input
    char * const old_buff = buff;
    while(strcmp(buff))
    {
        memset(buff, 0, LEN * sizeof(char)); // puts all 0s in the mem location from address stored in buf to buf+LEN
        printf("kash>");
        cmd_input(buff);
        cmd_parse(buff);
        buff = old_buff;
    }
    
    free(buff);

    // for(int i=0; i<LEN; ++i)
    // {
    //     printf("%p = %c\n", &buff[i], buff[i]);
    // }

    // for(int i=0; i<LEN; ++i)
    // {
    //     if(buff[i] == ' ')
    //         printf("spaces: %i\n", i);
    //     else break;
    // }

    // printf("second\n");
    
    // for(int i=0; i<LEN; ++i)
    // {
    //     printf("%p = %c\n", &buf2[i], buf2[i]);
    // }

    return EXIT_SUCCESS;
}

