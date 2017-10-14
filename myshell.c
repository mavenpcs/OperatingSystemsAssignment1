#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/types.h>
#include <unistd.h>
void timePrompt(); // print current system time to prompt
void sigHandler (int signum); // signal handler. Keeps the process alive when CTRL+C is pressed
char *getCmd(); // receive user input
char **parseCmd (char *line); // tokenize the user input into separate strings
int execCmd (char **cmd); // execute the command

int main ()
{
    char *buf;
    while (1)
    {
    signal(SIGINT, sigHandler); // catches signal when CTRL+C is pressed
    buf = getCmd(); //   prompt starts
    execCmd(parseCmd(buf)); // parse user input into command lines and execute
    }
    return 0;
}

void timePrompt ()
{
    char buf[50];
    time_t timeNow = time(NULL); // initialize current system time
    struct tm *curTime = localtime(&timeNow); // convert system time to local time
    strftime(buf, 50, "%H:%M:%S#", curTime); // format system time as desired
    fputs(buf, stdout); // output system time as prompt
}
void sigHandler (int signum)
{  // does not do anything other than its normal function
}
char *getCmd ()
{
    static char buf[50];
    while(1)
    {
        timePrompt();
        fgets(buf, 50, stdin);
        buf[strlen(buf)-1] = '\0'; // null terminate user input
        return buf;
    }
}
char **parseCmd (char *input)
{
    static char *cmd[50];
    char *line;
    int i = 0; // initialize loop variable
    line = strtok (input, " "); // tokenize user input
    while (line != NULL)
    {   // parse user input into the command vector
        cmd[i] = line;
        line = strtok (NULL, " ");
        i++;
    }
    cmd[i] = NULL; // null terminate the command vector
    return cmd;
}
int execCmd (char **cmd)
{
    int pid, status;
    if (cmd[0] == NULL)
    {   // user did not enter any input, continue running the shell
        return 1;
    }
    if (strcmp(cmd[0], "exit") == 0)
    {   // execute exit command
        exit(1);
    }
    pid = fork(); // fork: return positive value if successful, negative value if failed
    if (pid == 0)
    {   // Working in child process
        if (execvp(cmd[0], cmd) == -1)
        {
            printf("Invalid command \n");
        }
        exit(1);
    }
    else if (pid < 0)
    {
        printf("Fork failed");
    }
    else
    {
        while (wait(&status) != pid)
        {   // Working in parent process
            // Wait for child to finish
            printf("Execution from parent process. \n");
        }
    }
    return 1;
}
