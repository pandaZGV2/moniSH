#include "../global.h"

extern bool exit_prog;

void die(const char *s)
{
    perror(s);
    exit(1);
}

struct termios orig_termios;

void disableRawMode()
{
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios) == -1)
        die("tcsetattr");
}

/**
 * Enable row mode for the terminal
 * The ECHO feature causes each key you type to be printed to the terminal, so you can see what you’re typing.
 * Terminal attributes can be read into a termios struct by tcgetattr().
 * After modifying them, you can then apply them to the terminal using tcsetattr().
 * The TCSAFLUSH argument specifies when to apply the change: in this case, it waits for all pending output to be written to the terminal, and also discards any input that hasn’t been read.
 * The c_lflag field is for “local flags”
 */
void enableRawMode()
{
    if (tcgetattr(STDIN_FILENO, &orig_termios) == -1)
        die("tcgetattr");
    atexit(disableRawMode);
    struct termios raw = orig_termios;
    raw.c_lflag &= ~(ICANON | ECHO);
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1)
        die("tcsetattr");
}

void prompter()
{
    setbuf(stdout, NULL);
    strcpy(prompt, "<");
    strcat(prompt, username);
    strcat(prompt, "@");
    strcat(prompt, hostname);
    strcat(prompt, ":");
    char cwd[PATH_MAX];
    if (getcwd(cwd, sizeof(cwd)) != NULL)
    {
        ;
    }
    else
    {
        perror("getcwd() error");
        exit(1);
    }
    if (strncmp(home_folder, cwd, strlen(home_folder)) == 0)
    {
        strcat(prompt, "~");
        strcat(prompt, &cwd[strlen(home_folder)]);
    }
    else
    {
        strcat(prompt, cwd);
    }
    strcat(prompt, "> ");
    printf("%s", prompt);
    //    printf("\n");
    char clr_buf[20];
    char *command = (char *)calloc(10000, sizeof(char));
    int pt = 0;
    char c;
    bool up_arrow = false;
    node_hist *tmp = hist->head;
    while (read(STDIN_FILENO, &c, 1) == 1)
    {
        if (iscntrl(c))
        {
            if (c == 10)
            {
                printf("\n");
                break;
            }
            else if (c == 27)
            {
                char buf[3];
                buf[2] = 0;
                if (read(STDIN_FILENO, buf, 2) == 2)
                { // length of escape code
                    if (strcmp(buf, "[A") == 0)
                    {
                        up_arrow = true;
                        if (tmp == NULL)
                        {
                            ;
                        }
                        else
                        {
                            if (tmp == NULL)
                            {
                                continue;
                            }
                            for (int k = 0; command[k] != '\0'; k++)
                            {
                                command[k] = 0;
                            }
                            strcpy(command, tmp->element);
                            pt = strlen(tmp->element);
                            command[pt] = '\0';
                            tmp = tmp->next;
                            printf("\33[2K\r");
                            printf("%s%s", prompt, command);
                        }
                    }
                }
            }
            else if (c == 127)
            { // backspace
                if (pt > 0)
                {
                    if (command[pt - 1] == 9)
                    {
                        for (int i = 0; i < 7; i++)
                        {
                            printf("\b");
                        }
                    }
                    command[--pt] = '\0';
                    printf("\b \b");
                }
            }
            else if (c == 9)
            { // TAB character
                command[pt++] = c;
                for (int i = 0; i < 8; i++)
                { // TABS should be 8 spaces
                    printf(" ");
                }
            }
            else if (c == 4)
            {
                exit(0);
            }
            else
            {
                printf("%d\n", c);
            }
        }
        else
        {
            command[pt++] = c;
            printf("%c", c);
        }
    }
    command[pt] = '\0';
    char filename[1000];
    strcpy(filename, home_folder);
    strcat(filename, "/");
    strcat(filename, "history.log");
    int fd = open(filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
    if (fd < 0)
    {
        perror("Failed to open history");
    }
    char write_cmd[2000];
    sprintf(write_cmd, "%s\n", command);
    write(fd, write_cmd, strlen(write_cmd));
    close(fd);
    char delimit[] = " \t\n\f\r\v";
    char *saveptr = NULL, *saveptr1 = NULL;
    deque_pushfront(hist, command);
    char *token = strtok_r(command, "\n;", &saveptr);
    while (token != NULL)
    {
        args_vector *argv = convert_to_argv(token);
        char *endptr = NULL;
        if (argv->used == 0 || argv->argv[0] == NULL)
        {
            free_argv(argv);
            break;
        }
        long n = 1;
        if (strcmp(argv->argv[0], "repeat") == 0)
        {
            n = strtol(argv->argv[1], &endptr, 10);
            if (endptr == argv->argv[1])
            {
                fprintf(stderr, "No digits were found\n");
                goto L;
            }
            char *new_cmd = make_command(argv, 2);
            free_argv(argv);
            argv = NULL;
            ;
            argv = convert_to_argv(new_cmd);
            free(new_cmd);
        }
        while (n-- > 0)
        {
            if (strcmp(argv->argv[0], "cd") == 0)
            {
                int cd_status = 0;
                if (argv->used > 2)
                {
                    fprintf(stderr, "Too many arguments!\n");
                    goto L;
                }
                else if (argv->used == 1)
                {
                    char *prev_tmp;
                    prev_tmp = getcwd(NULL, 4096);
                    cd_status = cd("~");
                    if (cd_status == 0)
                        strcpy(previous_directory, prev_tmp);
                    free(prev_tmp);
                }
                else
                {
//                                        if(strcmp(argv->argv[1], "-") != 0)
//                                        {
//                                            printf("Getting OLDPWD\n");
//                                            printf("%ld\n", sizeof(previous_directory));
//                                            perror("OLDPWD");
//                                        }
                    char *prev_tmp;
                    prev_tmp = getcwd(NULL, 4096);
                    cd_status = cd(argv->argv[1]);
                    if (cd_status == 0)
                        strcpy(previous_directory, prev_tmp);
                    free(prev_tmp);
                }
            }
            else if (strcmp(argv->argv[0], "pwd") == 0)
            {
                if (argv->used != 1)
                {
                    printf("Invalid use of pwd\n");
                    goto L;
                }
                char current[PATH_MAX];
                getcwd(cwd, sizeof(cwd));
                printf("%s\n", cwd);
            }
            else if (strcmp(argv->argv[0], "echo") == 0)
            {
                echo(argv);
            }
            else if (strcmp(argv->argv[0], "exit") == 0)
            {
                exit_prog = true;
                goto L;
                return;
            }
            else if (strcmp(argv->argv[0], "pinfo") == 0)
            {
                if (argv->used == 1)
                {
                    pinfo(NULL);
                }
                else if (argv->used == 2)
                {
                    pinfo(argv->argv[1]);
                }
                else
                {
                    printf("Incorrect usage: pinfo\n");
                    goto L;
                }
            }
            else if (strcmp(argv->argv[0], "ls") == 0)
            {
                int opt;
                bool a = false;
                bool l = false;
                for (int i = 0; i < argv->used; i++)
                {
                    int cnt_flags = 0;
                    if (argv->argv[i][0] == '-')
                    {
                        for (int j = 1; argv->argv[i][j] != '\0'; j++)
                        {
                            if (argv->argv[i][j] == 'l')
                            {
                                l = true;
                            }
                            else if (argv->argv[i][j] == 'a')
                            {
                                a = true;
                            }
                            else
                            {
                                printf("Unknown option: %c\n", argv->argv[i][j]);
                            }
                            cnt_flags++;
                        }
                        if (cnt_flags == 0)
                        {
                            printf("Error: No option available\n");
                        }
                    }
                }
                int cnt_ls = 0;
                for (int i = 1; i < argv->used; i++)
                {
                    if (argv->argv[i][0] == '-')
                    {
                        continue;
                    }
                    ls(argv->argv[i], a, l);
                    cnt_ls++;
                }
                if (cnt_ls == 0)
                {
                    ls(NULL, a, l);
                }
            }
            else if (strcmp(argv->argv[0], "history") == 0)
            {
                int num = 10;
                endptr = NULL;
                if (argv->used == 1)
                {
                    history(hist, &num);
                }
                else
                {
                    num = strtol(argv->argv[1], &endptr, 10);
                    if (endptr == argv->argv[1])
                    {
                        fprintf(stderr, "No digits were found\n");
                        goto L;
                    }
                    history(hist, &num);
                }
            }
            else
            {
                if (strcmp(argv->argv[argv->used - 1], "&") == 0)
                {
                    run_bg(argv);
                    add_arg(argv, "&");
                }
                else
                {
                    run_fg(argv);
                }
            }
        }
    L:;
        free_argv(argv);
        argv = NULL;
        token = strtok_r(token + strlen(token) + 1, "\n;", &saveptr);
    }
    free(command);
    fflush(stderr);
    fflush(stdout);
    usleep(1000000);
}