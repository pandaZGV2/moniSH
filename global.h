#ifndef GLOBAL_H
#define GLOBAL_H

#include <assert.h>
#include <dirent.h>
#include <errno.h>
#include <pwd.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdio_ext.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#define _XOPEN_SOURCE 500
#define __USE_XOPEN_EXTENDED 1
#include <ctype.h>
#include <fcntl.h>
#include <ftw.h>
#include <grp.h>
#include <math.h>
#include <signal.h>
#include <termios.h>
#include <time.h>

#include "src/args_vector.h"
#include "src/cd.h"
#include "src/echo.h"
#include "src/history.h"
#include "src/ls.h"
#include "src/pinfo.h"
#include "src/prompt.h"
#include "src/run_bg.h"
#include "src/run_fg.h"
#include "src/track_bg.h"

#include "utils/clear_variables.h"
#include "utils/init.h"
#include "utils/pid_name_pair.h"
#include "utils/stack.h"

extern char *username;
extern char *cmd;
extern char *hostname;
extern char *home_folder;
extern char *previous_directory;
extern struct termios orig_termios;
extern deque *hist;

#endif