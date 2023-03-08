// Hakam Nabulssi

#include <pwd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdbool.h>
#define MAX_WORD 50
#define MAX_CHAR 511

// global variables
bool end = false;
int length_of_command = 0;
int number_of_commands = 0;

void prompt()
{
    if(end == false)
    {
        char *directory; // storing the directory and the path
        directory = (char *) malloc(MAX_CHAR * sizeof(char)); //allocating memory dynamically
        struct passwd *p;
        p = getpwuid(getuid());
        if (directory == NULL)  // if failed to get the directory,throw an error
        {
            perror("ERROR");
            exit(1);
        }
        getcwd(directory, MAX_CHAR);

        printf("%s@%s>", p->pw_name, directory); // printing out the directory and the path
        free(directory); // free the allocated memory
    }
}

void remove_end_of_line(char line[]) // removing the \n form the end of the string
{
    int i =0;
    while(line[i] != '\n') // getting the end of the array
        i++;
    line[i] = '\0'; // replace last index of the array with "\0"
}

void read_line(char line[]) // getting the string from the user, Dividing, counts the number of words
{
    double average;
    int count = 0,i ;
    int last_was_space = 1;

    char* ret = fgets(line,MAX_CHAR,stdin);
    remove_end_of_line(line); // calling the function that remove the end of the line "\n" and replacing it with "\0"
    length_of_command+= strlen(line); // length of the indexes of all the commands together

    if(strcmp(line,"done") !=0 || ret !=NULL){ // if the user didnt entered done, count the words
        for (i = 0; line[i] != '\0'; i++)
        {
            if (line[i] == ' ')
            {
                if (last_was_space == 0)
                {
                    count++; // Only count when last char wasn't a space
                    last_was_space = 1;
                }
            }
            else
            {
                // Update flag (unless this char is a newline)
                if (line[i] != '\n')
                    last_was_space = 0;
            }
        }
        if (last_was_space == 0)
            ++count; // Count the last word if there wasn't a space before

        number_of_commands++; //  number of commands entered by the user
    }
    if(strcmp(line,"done") ==0 || ret ==NULL) // if the user entered done, or Ctrl-D
    {
        average = (length_of_command/number_of_commands); // the average between the length of the commands the the number of them
        printf("Number of commands %d\n",number_of_commands); // printing the number of the commands
        printf("Total length of all commands : %d\n",length_of_command); // printing the length of all commands together including "done"
        printf("average length of all commands : %lf\n",average); // printing the average
        printf("See you Next time ! "); // printing "See you Next time !" after in the end of the process
        end = true; // if the process end then, end = true .
        exit(0);
    }
}

int parse_line(char* args[],char line[]) // parsing the string
{
    int i = 0;
    args[i] = strtok(line," "); // replacing the space with "\0"
    if(args[i] == NULL) // if the first index of the line was empty
    {
        return 1;
    }
    while(args[i] != NULL)// if isn't empty
    {
        i++; // the first index checked above
        args[i] = strtok(NULL," ");// replacing the space with "\0"
    }
    return 1 ;
}

int read_parse_line(char* args[], char line[]) // calling read / parse string methods
{
    prompt(); // calling the prompt
    read_line(line); // getting the line from the user
    parse_line(args,line); // parsing the line that entered by the user
    if(args[0] == NULL)//  check if the user enter nothing(enter click "\n") as command
    {
        printf("NO COMMAND\n"); // print "NO COMMAND" if the user entered "\n"
    }
    return 1;
}

int main()
{
    char* args[MAX_WORD]; // to save the commands
    char line[MAX_CHAR]; // to save the line entered by the user

    while (read_parse_line(args,line) && end!=true)
    {
        pid_t pid1,pid2; // madding two processes(child & parent process)
        pid1 = fork();
        if(pid1 == -1) // if the fork return -1 , throw error
        {
            perror("System Call Failed");
            exit(1);
        }

        else if(pid1 == 0)
        {

            // im in child
            if(strcmp(args[0] ,"cd")==0) // checking if the the command is "cd"
            {
                printf("Command is not supported (yet) \n");
                exit(0);

            }
            if(strcmp(args[0],"cd")==0|| strcmp(args[0],"done")==0|| strcmp(args[0],"")==0||args[0]==NULL) // checking if the command is illegal
            {
                exit(0);
            }

            pid2=fork();
            if(pid2==-1) // if the second fork return -1, throw error
            {
                perror("System Call Failed");
                exit(1);
            }
            else if(pid2==0)
            {
                char command[MAX_CHAR+2];
                snprintf( command, MAX_CHAR+2,"/proc/%d/sched", getppid());
                char *scheduler[] = {"cat", command, NULL};
                execvp(scheduler[0], scheduler); // executing the scheduler
                perror("System Call Failed");// throw error if the child cannot execute
                exit(1);
            }
            else
            {
                // im in parent
                wait(NULL); // wait the child till he end
                execvp(args[0], args);// executing the scheduler
                perror("System Call Failed"); // throw error if the child cannot execute
                exit(1);
            }
        }
        else
        {
            // im in parent
            wait(NULL);// wait the child till he end
        }
    }
    return 0;
}