#include "reveal.h"

int hidden = 0;
int extra_info = 0;

int is_hidden(char *fname)
{
    return (fname[0] == '.');
}
int is_directory(char *fname)
{
    struct stat statbuf;

    if (stat(fname, &statbuf) != 0)
    {
        perror("stat error\n");
        return 0;
    }
    return S_ISDIR(statbuf.st_mode);
}
int is_executable(char *fname)
{
    struct stat statbuf;

    if (stat(fname, &statbuf) != 0)
    {
        perror("stat error at executable file check!!\n");
        return -1;
    }

    if (stat(fname, &statbuf) == 0 && statbuf.st_mode & S_IXUSR)
    {
        return 1;
    }
    return 0;
}

int compare(const void *a, const void *b)
{
    return strcmp(*(const char **)a, *(const char **)b);
}

void set_color(char *fname)
{
    if (is_directory(fname))
    {
        blue();
    }
    else if (is_executable(fname))
    {
        green();
    }
    else
    {
        reset();
    }
}

bool containsOnlyAandL(char *str)
{
    while (*str)
    {
        if (*str != 'a' && *str != 'l')
        {
            return false; // Return false if a character other than 'a' or 'l' is found
        }
        str++;
    }
    return true; // Return true if only 'a' and 'l' are found
}

int set_hidden_and_extrainfo(char **input, int size,int* index)
{
    for (int i = 0; i < size; i++)
    {
        if (input[i][0] == '-' && strlen(input[i])>1)
        {
            if (containsOnlyAandL(input[i] + 1))
            {

                if (strchr(input[i], 'a'))
                {
                    hidden = 1;
                }
                if (strchr(input[i], 'l'))
                {
                    extra_info = 1;
                }
                (*index)++;
            }else{
                printf("Invalid flags!!!\n");
                return 0;
            }
        }
    }
    return 1;
}

void print_file_info(char *filename)
{
    reset();
    struct stat statbuf;

    // Get file status
    if (stat(filename, &statbuf) == -1)
    {
        perror("stat");
        return;
    }

    // Print file permissions
    printf((S_ISDIR(statbuf.st_mode)) ? "d" : "-");
    printf((statbuf.st_mode & S_IRUSR) ? "r" : "-");
    printf((statbuf.st_mode & S_IWUSR) ? "w" : "-");
    printf((statbuf.st_mode & S_IXUSR) ? "x" : "-");
    printf((statbuf.st_mode & S_IRGRP) ? "r" : "-");
    printf((statbuf.st_mode & S_IWGRP) ? "w" : "-");
    printf((statbuf.st_mode & S_IXGRP) ? "x" : "-");
    printf((statbuf.st_mode & S_IROTH) ? "r" : "-");
    printf((statbuf.st_mode & S_IWOTH) ? "w" : "-");
    printf((statbuf.st_mode & S_IXOTH) ? "x " : "- ");

    // Print number of hard links
    printf("%lu ", statbuf.st_nlink);

    // Print owner and group
    struct passwd *pw = getpwuid(statbuf.st_uid);
    struct group *gr = getgrgid(statbuf.st_gid);
    printf("%s %s ", pw->pw_name, gr->gr_name);

    // Print file size
    printf("%5ld ", statbuf.st_size);

    // Print last modification time
    char time_buf[64];
    strftime(time_buf, sizeof(time_buf), "%b %d %H:%M", localtime(&statbuf.st_mtime));
    printf("%s ", time_buf);

    // Print file name
    set_color(filename);
    printf("%s\n", filename);
}

void reveal(char **input, int size, char *home)
{

    int is_File = 0;
    char file_name[400];

    char curr_dir[4096];
    getcwd(curr_dir, 4096);
    int changed_dir = 0;

    char dir_p[4096];
    getcwd(dir_p, 4096);

    // int cnt = 0;

    int index=0;

    if(!set_hidden_and_extrainfo(input, size,&index)){
        return;
    }

    if(size > index+2){
        printf("too many arguments!!!\n");
        return;
    }

    if(index+1<size){


        if (strcmp(input[index+1], "-") == 0)
        {
            char *prev_dir = get_old_cwd();

            strcpy(dir_p, prev_dir);

            chdir(prev_dir);
            changed_dir = 1;

        }

        if (input[index+1][0] != '-')
        {
            if (strcmp(input[index+1], "~") == 0)
            {
                strcpy(dir_p, home);
                chdir(home);
                changed_dir = 1;

            }
            else if (strcmp(input[index+1], ".") == 0)
            {

            }
            else if (strcmp(input[index+1], "..") == 0)
            {
                chdir("..");
                getcwd(dir_p, 4096);
                changed_dir = 1;

            }
            else
            {

                if (is_directory(input[index+1]))
                {
                    get_path_of_dir(input[index+1], dir_p);
                    chdir(dir_p);
                    changed_dir = 1;
    
                }
                else
                {
                    is_File = 1;
                    strcpy(file_name, input[index+1]);
                }
            }
        }
    }


    if (is_File)
    {
        set_color(file_name);
        if (hidden == 1)
        {
            if (extra_info == 1)
            {
                print_file_info(file_name);
            }
            else
            {
                printf("%s\n", file_name);
            }
        }
        else
        {
            if (!is_hidden(file_name))
            {

                if (extra_info == 1)
                {
                    print_file_info(file_name);
                }
                else
                {
                    printf("%s\n", file_name);
                }
            }
        }
        return;
    }

    DIR *dir = opendir(dir_p);
    if (!dir)
    {
        perror("Unable to open the directory !!!\n");
        return;
    }

    struct dirent *dir_files = NULL;

    char *files[MAX_FILES];
    int count = 0;

    while ((dir_files = readdir(dir)) != NULL)
    {

        if (count < MAX_FILES)
        {
            files[count++] = dir_files->d_name;
        }
        else
        {
            printf("Too many files in the directory!!!\n");
            closedir(dir);
            return;
        }
    }

    qsort(files, count, sizeof(char *), compare);

    printf("Total:%d\n",count);

    for (int i = 0; i < count; i++)
    {
        set_color(files[i]);
        if (hidden == 1)
        {
            if (extra_info == 1)
            {
                print_file_info(files[i]);
            }
            else
            {
                printf("%s\n", files[i]);
            }
        }
        else
        {
            if (!is_hidden(files[i]))
            {

                if (extra_info == 1)
                {
                    print_file_info(files[i]);
                }
                else
                {
                    printf("%s\n", files[i]);
                }
            }
        }
    }

    if (changed_dir)
    {
        chdir(curr_dir);
    }

    hidden = 0;
    extra_info = 0;
    closedir(dir);
}
