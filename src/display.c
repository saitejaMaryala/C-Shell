#include "display.h"

void user()
{
    uid_t uid = getuid();
    struct passwd *pid = getpwuid(uid);
    char username[300];
    strcpy(username, pid->pw_name);

    char sysname[400];
    gethostname(sysname, sizeof(sysname));
    // strcat(username, "@");

    // strcat(username, sysname);

    printf("<");
    blue();
    printf("%s", username);
    reset();
    printf("@");
    printf("%s",sysname);
}
char *relative_path(char *base_path, char *target_path)
{

    int cmn = 0;
    while (base_path[cmn] == target_path[cmn] && base_path[cmn] != '\0' && target_path[cmn] != '\0')
    {
        cmn++;
    }

    int size = strlen(target_path) - cmn + 1;
    char *rel_path = (char *)malloc(size);
    rel_path[0] = '\0';

    strcat(rel_path, target_path + cmn);

    return rel_path;
}
char *curr_working_dir()
{
    char *cwd = (char *)malloc(256 * sizeof(char));
    getcwd(cwd, 256);

    return cwd;
}

void print_dir(char *home)
{
    // char* rel_path=relative_path(curr_dir,curr_working_dir());
    // printf(":~%s\n",rel_path);
    
    char cwd[PATH_MAX];

    if (getcwd(cwd, sizeof(cwd)) != NULL)
    {
        // Calculate the relative path with respect to the home directory
        if (strncmp(cwd, home, strlen(home)) == 0)
        {
            // Print relative path from home directory
            printf(":~%s ", cwd + strlen(home));
        }
        else
        {
            // Print full path if not inside home directory
            printf(":%s ", cwd);
        }
    }
    else
    {
        perror("getcwd() error");
        return ;
    }
}

void user_dir(char *curr_dir)
{
    user();
    print_dir(curr_dir);
}
