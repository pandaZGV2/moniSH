# Operating Systems and Networks - Assignment 2

The goal of this assignment is to create a simple shell that implements some basic commands such as `pwd`, `ls`, `cd`, and more.
The assignment folder consists of a `src` folder that implements the commands required for the shell and a `utils` directory that contains essential functions to support the shell.

## Running the Code

To run the code

```shell
make rebuild_cache
make
./OS_Assignment-2
```

Alternatively,

```shell
gcc -g main.c src/*.c utils/*.c -lm -o moniSH
./moniSH
```

## Details

### Prompt

When the shell executes a prompt appears asking the user for commands to execute. The prompt gives us information about the current working directory as well as the username and the system name.

The prompt is of the form

```shell
<username@system_name:curr_dir>
```

We consider the directory where the shell was invoked as the home (`~`) directory of the shell.
This feature is implemented using `prompt.c` in `src/`. `prompt.c` is also responsible for detecting control characters such as newline, tab, arrow keys, etc. This is helpful when implementing the "loop through history" feature of the shell described in the bonus questions. It is also responsible for parsing the given command. It detects commands that have been chained using `;` and tokenizes them into separate commands. It then converts the command into a vector representation that makes it easier to handle the different arguments. Using the first argument of the vector, `prompt.c` detects whether a user defined command or a system command is to be executed. The parsing of the commands is done using `strtok_r()` and `args_vector.c` in `src/`.

### Built-commands

This shell uses certain built-in commands such as `pwd`, `echo`, and `cd`. The commands are first parsed in `prompt.c` and converted into a vector form. Using the first argument of the vector we can detect which built-in command is to be used and the appropriate functions are called.

#### cd command

The cd command moves us from the current directory to the specified directory. It is implemented using `cd()` from `cd.c`.

**Usage:**
- `~` refers to the home directory of the shell.
- We also implement flags such as '.', '..', '-' to be used for the command as it is in the bash shell.
- If no directory or flag is specified, we switch to the home directory.

#### pwd command

The pwd command prints the current working directory of the shell. It prints the absolute path of the directory with respect to the Linux file system.
The command is handled by the `prompter()` function.

#### echo command

The `echo` command prints a line of text given to it as an argument. We do not handle escape codes and quotes, but we remove excess spaces and tabs between arguments before printing it to the terminal.
The `echo` command is handled by `echo()` from `echo.c` that parses the command before printing the arguments.

#### ls command

The `ls` command lists a given file or the contents of the given directory. It accepts flags `-a` and `-l`. It also handles incorrect flags and multiple directories.
The `-a` flag indicates that all files in a given directory, including the hidden ones, are to be printed. The `-l` command indicates that we must list information about the contents of the file along with the name of the file. We provide the permissions given to the file or directory, the username, the groupname, the size, the number of links, the last date of modification along with the name of the files and the directories.
If there are multiple directories or files, we apply the same flags to each of them, printing the output for each of them one after the other.
The `ls` command is handled by `ls()` from `ls.c` which determines whether the given path refers to a directory or a file. If it refers to a directory we pass it to `ls_dir()` to print the contents of the directory. If not we pass it to `ls_file()` that simply prints the file name along with any extra ifnormation if the `-l` flag is specified.

#### history command

We also implement the `history` command that behaves like the `history` command in bash. We print the most recent command provided to the shell. 

The usage of the command is as follows:
```shell
history <num>
```

Here `<num>` specifies the number of previous commands that must be printed. If the command is specified with no arguments then we print the 10 most recent commands on the shell.
We also store atmost 20 commands in the shell at any point in time. Each of these commands is written to `history.log` in order to maintain the history across all sessions of the shell.
The `history` command is implemented using `history.c` that stores the executed commands in a deque and prints them when required. It also uses `history.log` to get the commands of previous sessions.

#### pinfo command

The pinfo command prints the Process ID, Process Status, Memory Used and the executable path of the process whose pid is specified. If no pid is specified the pid of the calling process (the shell) is used.
This command is implemented in `pinfo.c` by reading through the stat, status, exe, and statm files of the specified process and printing the necessary information.

#### repeat command

The repeat command is of the form:
```shell
repeat <num> cmd
```

The repeat command executes `cmd` `<num>` number of times. This is implemented in `prompter()` in `prompt.c` along with appropriate error handling.

### Foreground and Background Processes

When a system command is executed using the shell it runs as a foreground process and we have to wait for it to complete. If we use `&` as an argument of the command, we execute the command as a background process. The process then runs behind the senes without any user interventions.
We run a foreground process using `run_fg.c` and a background process using `run_bg.c`. The functions in both programs execute the process with appropriate error-handling.
We set `track_bg()` from `track_bg.c` as the signal handler for `SIGCHLD`. This means that whenever we receive the `SIGCHLD` signal we call the `track_bg` function. In this fucntion we wait on the child processes of the parent process (the shell). If a child terminates we call the `wait()` system call on it and prevent it from becoming a zombie process. We also get the exit status of the child process and determine if the child executed normally or abnormally.
The status updates are stores on a stack and printed before the next prompt appears.

### UP Arrow Key

We can use the UP arrow key to loop through previously executed commands in the history. On clicking the UP arrow key, you must loop over the previous  commands present in your shell’s history and show them on the prompt. In case you reach the first command or have no history, then stay on the same command if UP is pressed.
This functionlity is implemented in prompter by reading through each character put in the input buffer one at a time and checking if an up arrow key is pressed.


