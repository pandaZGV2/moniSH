#include "global.h"

bool exit_prog = false;

int main(void)
{
    exit_prog = false;
    init_program();
    assert(bg_list);
    signal(SIGCHLD, track_bg);
    setbuf(stdout, NULL);
    while (!exit_prog)
    {
        print_stack(status_list);
        enableRawMode();
        prompter();
        disableRawMode();
    }
    clear_vars();

    return 0;
}