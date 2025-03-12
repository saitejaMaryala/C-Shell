#include "log.h"


char filename[4096];

void set_history_path(char *path)
{
    strcpy(filename, path);
}

int count_lines()
{
    FILE *file = fopen(filename, "a+");

    if (file == NULL)
    {
        perror("Error opening the file  (count lins)\n");
        return 0;
    }
    int lines = 0;
    char ch;

    while ((ch = getc(file)) != EOF)
    {
        if (ch == '\n')
        {
            lines++;
        }
    }

    fclose(file);
    return lines;
}

void show_past_events()
{

    FILE *file = fopen(filename, "r");
    if (file == NULL)
    {
        perror("error while opening the file(show events of past)\n");
        return;
    }

    char line[1024];

    while (fgets(line, sizeof(line), file))
    {
        printf("%s\n", line);
    }
    fclose(file);
}
void purge()
{

    FILE *file = fopen(filename, "w");
    if (file == NULL)
    {
        perror("Error Truncating the history\n");
        return;
    }
    fclose(file);
}

char *history_cmd(int ln)
{

    // int ln=atoi(num);

    if(ln<=0 || ln>count_lines()){
        printf("Enter a valid number !!!\n");
        return NULL;
    }

    int total_lines = count_lines();
    int exec_num = (total_lines - ln + 1);
    FILE *file = fopen(filename, "r");

    if (file == NULL)
    {
        perror("error while opening the file(getting history)\n");
        return NULL;
    }

    int cur_line = 0;
    char *cmd = (char *)malloc(1024 * sizeof(char));
    if (cmd == NULL)
    {
        perror("Unable to allocate memory\n");
        fclose(file);
        return NULL;
    }
    int cmd_found = 0;

    while (fgets(cmd, 1024, file))
    {
        cur_line++;
        if (cur_line == exec_num)
        {
            cmd_found = 1;
            break;
        }
    }

    fclose(file);

    if (cmd_found)
    {
        return cmd;
    }
    else
    {
        free(cmd);
        printf("Error while fetching the command from history\n");
        return NULL;
    }
}

int is_integer(const char *str)
{
    if (*str == '\0')
    {
        return 0; // No digits after the sign
    }
    while (*str)
    {
        if (!isdigit(*str))
        {
            return 0; // Non-digit character found
        }
        str++;
    }
    return 1; // All characters are digits
}

void delete_first_line()
{

    FILE *file = fopen(filename, "r");
    if (file == NULL)
    {
        perror("Error opening the file(deleting)");
        return;
    }

    FILE *temp_file = fopen("temp_file.txt", "w");
    if (temp_file == NULL)
    {
        perror("Error opening the file");
        return;
    }

    int first_line = 1;
    char buffer[1024];

    while (fgets(buffer, sizeof(buffer), file))
    {
        if (first_line)
        {
            first_line = 0;
            continue;
        }
        fputs(buffer, temp_file);
    }

    fclose(file);
    fclose(temp_file);

    remove(filename);
    rename("temp_file.txt", filename);
}


// void delete_first_line(){
//     FILE *file = fopen(filename, "r+");
//     if (file == NULL) {
//         perror("Error opening file");
//         return;
//     }

//     // Find the first newline character to determine the end of the first line
//     int ch;
//     long pos = 0;
//     while ((ch = fgetc(file)) != EOF) {
//         if (ch == '\n') {
//             break;
//         }
//         pos++;
//     }

//     // If there is no newline character, the file might be empty or malformed
//     if (ch == EOF) {
//         fclose(file);
//         return;
//     }

//     // Move the file pointer to the start of the second line
//     fseek(file, pos + 1, SEEK_SET);

//     // Create a temporary buffer to hold the rest of the file content
//     // Note: This approach works if the file size is not too large.
//     fseek(file, 0, SEEK_END);
//     long end_pos = ftell(file);
//     fseek(file, pos + 1, SEEK_SET);

//     // Get the size of the remaining content
//     long size = end_pos - ftell(file);
//     char *buffer = malloc(size);
//     if (buffer == NULL) {
//         perror("Memory allocation error");
//         fclose(file);
//         return;
//     }

//     // Read the remaining content into the buffer
//     fread(buffer, 1, size, file);

//     // Move back to the position of the first line and truncate the file
//     fseek(file, 0, SEEK_SET);
//     ftruncate(fileno(file), pos);

//     // Write the remaining content back to the file
//     fwrite(buffer, 1, size, file);

//     free(buffer);
//     fclose(file);
// }


void store_events(char *data)
{

    int lines = count_lines();

    if (lines > 0)
    {
        char *prev_data = history_cmd(1);

        if (strcmp(data, prev_data) == 0)
        {
            return;
        }
    }

    if (lines >= Max_FILES)
    {
        delete_first_line();
    }

    FILE *file = fopen(filename, "a+");

    if (file == NULL)
    {
        perror("Error opening the file (storing)\n");
        return;
    }

    fputs(data, file);

    fclose(file);
}

