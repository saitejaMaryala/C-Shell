#include "headers.h"
#include "display.h"

#include "execute_cmd.h"


// char *read_command()
// {
//     char *command = NULL;
//     char *delim = " \n";
//     size_t n = 10;
//     getline(&command, &n, stdin);

//     return command;
// }


int main()
{
    
    char home[PATH_MAX];
    if (getcwd(home, sizeof(home)) == NULL)
    {
        perror("Error in getting home directory!!");
        return 0;
    }

    set_cwd(home);

    char history_path[4096];
    strcpy(history_path,home);

    strcat(history_path,"/history1.txt");


    set_history_path(history_path);

    while (1)
    {

        user_dir(home);
        time_taken();

        char cmd[1000];
        fgets(cmd, sizeof(cmd), stdin);

        char *dup_cmd = strdup(cmd);

        

        int i = 0;
        while (cmd[i] != '\n')
        {
            i++;
        }
        cmd[i] = '\0';

        tokenize_execute(cmd, home);

        if(get_store() == 1){
            store_events(dup_cmd);
        }else{
            set_store(1);
        }

        continue;
    }

    
}