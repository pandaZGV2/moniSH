#ifndef PROMPT_H
#define PROMPT_H

extern char *username;
char *prompt;
void prompter();

void die(const char *s);
void disableRawMode();
void enableRawMode();
#endif