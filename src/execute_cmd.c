#include "execute_cmd.h"

static int store = 1;

void set_store(int n)
{
    store = n;
}
int get_store()
{
    return store;
}

int bg_num = 0;
pid_t bg_pids[1000] = {0};

int cnf = 0;

void execute_cmd(char *input, char *home)
{

    time_t start_time, end_time;

    start_time = time(NULL);

    fflush(stdout);

    if (strchr(input, '\"') == NULL)
    {
        remove_extra_spaces(input);
    }

    if (strchr(input, '&') != NULL)
    {
        backg_and_foreg(input, home);
    }
    else
    {
        char *base_cpy = strdup(input);
        if (base_cpy == NULL)
        {
            perror("error copying into base_Cpy");
            return;
        }
        char *cpy = strdup(input); // Copy input string
        if (base_cpy == NULL)
        {
            perror("error copying into base_Cpy");
            return;
        }
        int size = 0;                              // Initialize size variable
        char **cmds = tokenize(cpy, &size, " \n"); // Tokenize the input

        char *time_cmd = strdup(cmds[0]);

        // Ensure cmds is valid
        if (cmds != NULL && size > 0)
        {
            if (strcmp(cmds[0], "hop") == 0)
            {
                for (int i = 1; i < size; i++)
                {
                    hop(cmds[i], home);
                }
            }
            else if (strcmp(cmds[0], "log") == 0)
            {

                set_store(0);
                if (size > 1 && strcmp(cmds[1], "execute") == 0 && isdigit(cmds[2][0]))
                {

                    int num = atoi(cmds[2]);
                    char *log_cmd = history_cmd(num);

                    if (log_cmd == NULL)
                    {
                        return;
                    }

                    tokenize_execute(log_cmd, home);

                    free(log_cmd);
                }
                else
                {
                    log_exec(cmds, size);
                }
            }
            else if (strcmp(cmds[0], "reveal") == 0)
            {
                reveal(cmds, size, home);
            }
            else if (strcmp(cmds[0], "proclore") == 0)
            {
                int pid;
                if (size == 1)
                {
                    pid = getpid();
                    proclore(pid, home);
                }
                else if (size == 2)
                {
                    pid = atoi(cmds[1]);
                    proclore(pid, home);
                }
                else
                {
                    printf("Invalid command!!\n");
                    return;
                }
            }
            else if (strcmp(cmds[0], "seek") == 0)
            {
                seek(cmds, size, home);
            }
            else if (strcmp(cmds[0], "exit") == 0)
            {

                exit(1);
            }
            else
            {
                execute_basic_cmd(base_cpy, cmds, size); // Execute other commands
            }

            free(cmds);
        }

        // Free the duplicated input string
        free(cpy);
        free(base_cpy);

        end_time = time(NULL);

        double d = difftime(end_time, start_time);

        if (d > 1)
        {

            int time = (int)d;

            set_time_name(time, time_cmd);
        }
        free(time_cmd);
    }
}

void backg_and_foreg(char *input, char *home)
{

    int size;

    int curr_bg_num = back_process_count(input);

    bg_num = curr_bg_num + bg_num;
    char **cmds = tokenize(input, &size, "&");

    printf("\n");

    for (int i = 0; i < curr_bg_num; i++)
    {
        pid_t pid = fork();

        if (pid < 0)
        {
            perror("Error while forking!!\n");
            return;
        }
        else if (pid == 0)
        {

            fflush(stdout);

            pid_t grand_child = fork();

            if (grand_child == 0)
            {

                printf("%d\n\n", getpid());

                setpgid(0, 0);

                int dev_null = open("/dev/null", O_RDWR);
                if (dev_null == -1)
                {
                    perror("open /dev/null failed");
                    exit(EXIT_FAILURE);
                }

                // Save original file descriptors
                int orig_stdin = dup(STDIN_FILENO);
                int orig_stdout = dup(STDOUT_FILENO);
                int orig_stderr = dup(STDERR_FILENO);

                // Redirect stdin, stdout, and stderr to /dev/null
                dup2(dev_null, STDIN_FILENO);
                dup2(dev_null, STDOUT_FILENO);
                dup2(dev_null, STDERR_FILENO);

                close(dev_null);

                usleep(5000);
                execute_in_exec(cmds[i]);

                perror("execvp failed");
                dup2(orig_stdin, STDIN_FILENO);
                dup2(orig_stdout, STDOUT_FILENO);
                dup2(orig_stderr, STDERR_FILENO);

                close(orig_stdin);
                close(orig_stdout);
                close(orig_stderr);

                exit(0);
            }
            else
            {

                int status;
                pid_t wpid = waitpid(grand_child, &status, 0);

                if (wpid < 0)
                {
                    // Error in waitpid
                    perror("waitpid");
                    exit(EXIT_FAILURE);
                }

                if (WIFEXITED(status))
                {
                    int exit_status = WEXITSTATUS(status);
                    if (exit_status == 0)
                    {
                        // Child exited normally
                        printf("%s (%d) exited normally\n", cmds[0], grand_child);
                    }
                    else
                    {
                        // Child exited with a non-zero status (could be an error)
                        printf("%s (%d) exit status %d\n", cmds[0], grand_child, exit_status);
                    }
                }
                else if (WIFSIGNALED(status))
                {
                    // Child terminated by a signal
                    int term_signal = WTERMSIG(status);
                    printf("%s (%d) terminated by signal %d\n", cmds[0], grand_child, term_signal);
                }
                else
                {
                    // Some other abnormal termination
                    printf("%s (%d) terminated abnormally\n", cmds[0], grand_child);
                }
                exit(1);
            }
        }
    }
    if (curr_bg_num < size)
    {

        int fg_pid = fork();

        if (fg_pid < 0)
        {
            perror("Error while forking!!\n");
            return;
        }
        else if (fg_pid == 0)
        {

            execute_cmd(cmds[size - 1], home);
            exit(1);
        }
        int status;
        waitpid(fg_pid, &status, 0);
    }
}

void execute_in_exec(char *input)
{

    int size = 0;                              // Initialize size variable
    char **cmds = tokenize(input, &size, " \n"); // Tokenize the input

    execvp(cmds[0],cmds);
}
int back_process_count(char *input)
{
    int cnt = 0;
    int i = 0;
    while (input[i])
    {
        if (input[i] == '&')
            cnt++;

        i++;
    }
    return cnt;
}
void log_exec(char **cmds, int size)
{
    if (size == 1)
    {
        show_past_events();
    }
    else if (size == 2 && strcmp(cmds[1], "purge") == 0)
    {
        purge();
    }
    else
    {
        printf("Invalid command!!\n");
        return;
    }
}

char **tokenize(char *input, int *size, char *delim)
{

    char *cpy1 = strdup(input); // Duplicate the input for token counting
    char *cpy2 = strdup(input); // Duplicate the input for actual tokenization
    char *token = NULL;
    int len = 0, i = 0;

    // Count tokens using cpy1
    token = strtok(cpy1, delim);
    while (token)
    {
        len++;
        token = strtok(NULL, delim);
    }
    // free(cpy1); // Free cpy1 after counting tokens

    // Allocate memory for tokens
    char **tokens = malloc(sizeof(char *) * len);
    if (tokens == NULL)
    {
        perror("malloc");
        exit(1);
    }

    // Tokenize using cpy2
    token = strtok(cpy2, delim);
    while (token)
    {
        tokens[i] = strdup(token);
        if (tokens[i] == NULL)
        {
            perror("strdup");
            exit(1);
        }

        i++;
        token = strtok(NULL, delim);
    }

    *size = len;
    free(cpy2); // Free cpy2 after tokenizing

    return tokens;
}

void tokenize_execute(char *input, char *home)
{
    char *cpy = strdup(input);
    char *token = NULL;
    char *delim = ";";

    input[strcspn(input, "\n")] = 0;

    token = strtok(input, delim);

    int len = 0, i = 0;

    while (token)
    {
        len++;
        token = strtok(NULL, delim);
    }

    char **tokens = malloc(sizeof(char *) * len);
    token = strtok(cpy, delim);

    while (token)
    {
        tokens[i++] = token;
        token = strtok(NULL, delim);
    }

    for (i = 0; i < len; i++)
    {
        execute_cmd(tokens[i], home);
    }

    free(cpy);
}

void remove_extra_spaces(char *str)
{
    int i = 0, j = 0;
    int in_word = 0; // Track if we're inside a word

    while (str[i])
    {
        if (!isspace((unsigned char)str[i]))
        {
            str[j++] = str[i];
            in_word = 1;
        }
        else if (in_word)
        {
            str[j++] = ' '; // Replace the first space or tab after a word with a single space
            in_word = 0;
        }
        i++;
    }

    // Remove trailing space if there is one
    if (j > 0 && str[j - 1] == ' ')
    {
        j--;
    }

    str[j] = '\0'; // Null-terminate the modified string
}
