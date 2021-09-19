#include "../global.h"

extern char *prompt;
char *distdir;
char *username;
char *cmd;
char *hostname;
char *home_folder;
char *previous_directory;
proc_list *bg_list;
stack *status_list;
struct deque *hist;

void init_program()
{
    username = (char *)malloc(sizeof(char) * 30);
    assert(username);
    prompt = (char *)malloc(sizeof(char) * 1001);
    assert(prompt);
    cmd = (char *)malloc(30);
    assert(cmd);
    hostname = (char *)calloc(sizeof(char), HOST_NAME_MAX + 1);
    assert(hostname);
    home_folder = (char *)calloc(sizeof(char), PATH_MAX);
    assert(home_folder);
    if (getcwd(home_folder, PATH_MAX) != NULL)
    {
        // printf("Current working dir: %s\n", cwd);
    }
    else
    {
        perror("getcwd() error");
        exit(1);
    }
    previous_directory = (char *)malloc(1000);
    previous_directory[0] = '\0';
    assert(previous_directory);

    int userid = geteuid();

    char cwd[1000];
    struct passwd *pw = getpwuid(userid);
    assert(pw);
    strcpy(username, pw->pw_name);

    gethostname(hostname, HOST_NAME_MAX + 1);

    bg_list = NULL;
    bg_list = init_proclist(bg_list);

    status_list = NULL;
    status_list = initstack();

    hist = NULL;
    hist = init_hist(hist);
}
