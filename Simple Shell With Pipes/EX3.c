// Hakam Nabulssi
// ID: 207710443

// libraries used in this exercise
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
bool end = false; // boolean variable to check conditions
char * new_line; // holds the line from the user

int temp_pipe = 0 ;// 0 if there isn't any pipes , 1 if there is a pipe
int general_pipe=0; //the number of the pipes used in each input
int pipe_counter =0;//the total number of pipes used in the shell
char * array1[MAX_CHAR];//array that stores every command before pipe
int array1_length=0; // length of the array holding the first command before pipe
int last_array1_length=0;//stores the last array length
char * array2[MAX_CHAR];//array that stores every command after pipe
int array2_length =0;// length of the array holding the middle command between two pipes
int last_array2_Length=0;//stores the last array length
char * array3[MAX_CHAR];//array that stores every command after the second pipe
int array3_length =0; // // length of the array holding the last command after second pipe
int last_array3_length=0;//stores the last array length

int counter = 0 ;//commands counter in one line
int last_args_counter;//stores the last counter value
int length_of_command = 0; // length of the command
int number_of_commands = 0; // the number of the commands

// functions used in this exercise
void prompt(); // printing the prompt of the commands
void read_line(char line[]); // receiving the commands from the user
void remove_end_of_line(char line[]); // removing the \n form the end of the string had entered by the user ( by using fgets )
int parse_line(char* args[],char line[]); // splitting the line entered by the user in the wanted manner
void commands_in_line(char * str); // counts the number of commands in one line
void spacing(char * string); // correcting the spaces for using pipe (make a space before and after the pipe) for making it works in all the situations
void replace_char(char* str, char find, char replace); // replacing the quotations marks with a space for removing them
void moving_to_arrays(char **args, char * line); // moving the commands to an arrays as a first command, middle command, and last command for piping
int read_parse_line(char* args[], char line[]); // calling all the methods in an ordered manner


void prompt() //printing the prompt of the command
{
    if(end == false)
    {
        char *directory; // storing the directory and the path
        directory = (char *) malloc(MAX_CHAR * sizeof(char)); //allocating memory dynamically
        struct passwd *p;
        p = getpwuid(getuid());
        if (directory == NULL) // if failed to get the directory,throw an error
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

        number_of_commands++; // number of commands entered by the user
    }
    if(strcmp(line,"done") ==0 || ret ==NULL) // if the user entered done, or Ctrl-D
    {
        number_of_commands--;

        printf("Number of commands %d\n",number_of_commands); // printing the number of the commands

        printf("Number of Pipes %d\n",pipe_counter ); // printing the number of the pipes

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

void commands_in_line(char * str) // counts the number of commands in one line
{
    int i ;
    for(i=0; str[i]!='\0'; i++)
        if(str[i]!=' '&&(str[i+1]==' '||str[i+1]=='\0'))
            counter++;
}


void spacing(char * string) // correcting the spaces for using pipe
{
    int i ;
    char * space = " ";
    char Pipes ='|';
    new_line = (char *) malloc(MAX_CHAR * sizeof(char)); // allocating memory for the string
    int length = strlen(string);

    for(i = 0; i < length; i++)
    {
        if( string[i]== '|' ) // if pipe have found
        {
            temp_pipe=1;
            general_pipe++;
            pipe_counter ++;
            strncat(new_line, space , 1); // function that concatenates two strings.
        }
        else
        {
            if(temp_pipe == 1) // if pipe haven't found
            {
                strncat(new_line, &Pipes , 1);
                strncat(new_line, space , 1);
                temp_pipe=0;
            }
            strncat(new_line, &string[i], 1);
        }
    }
}


void replace_char(char* str, char find, char replace) // replacing quotation marks with a spaces
{
    char *current_pos = strchr(str,find);
    while (current_pos)
    {
        *current_pos = replace;
        current_pos = strchr(current_pos,find);
    }
}


void moving_to_arrays(char **args, char * line) // filling the array to hold the first, middle, last commands when piping have found
{
    if(general_pipe == 1) // first pipe has found
    {
        int i;
        int j =0;

        for( i =0; i < counter ; i++ )
        {
            if( *args[i] == '|' )
            {
                i++;
                int x ;
                int n =0;
                for( x = i; x < counter ; x++ )
                {
                    array2[n] = (char *) malloc(MAX_CHAR * sizeof(char));
                    strcpy(array2[n] , args[x]);

                    array2_length++;
                    n++;
                }
                break;

            }
            else
            {
                array1[j] = (char *) malloc(MAX_CHAR * sizeof(char));
                strcpy(array1[j] , args[i]);

                array1_length++;
                j++;
            }
        }
        counter =0;
    }

    if(general_pipe == 2) // if another pipe has found
    {
        int i;
        int j =0;

        for( i =0; i < counter ; i++ )
        {
            if( *args[i] == '|' )
            {
                i++;
                int x ;
                int n =0;
                for( x = i; x < counter ; x++ )
                {
                    if(*args[x] == '|')
                    {
                        x++;
                        int l ;
                        int k=0;

                        for( l = x; l < counter ; l++ )
                        {
                            array3[k] = (char *) malloc(MAX_CHAR * sizeof(char));
                            strcpy(array3[k] , args[l]);
                            array3_length++;
                            k++;
                        }
                        break;

                    }
                    else
                    {
                        array2[n] = (char *) malloc(MAX_CHAR * sizeof(char));
                        strcpy(array2[n] , args[x]);

                        array2_length++;
                        n++;
                    }
                }
                break;
            }
            else
            {
                array1[j] = (char *) malloc(MAX_CHAR * sizeof(char));
                strcpy(array1[j] , args[i]);

                array1_length++;
                j++;

            }
        }
        counter =0;
    }
}

int read_parse_line(char* args[], char line[]) // calling all the methods , and while this method is work the program will still works
{
    prompt(); // calling the prompt

    read_line(line); // getting the line from the user

    spacing(line);//copies a new string called new line that contains the string with spaces before the pipe and after

    replace_char(new_line, '"', ' ');//replaces each " with a space within the input

    commands_in_line(new_line);//counts how many commands are there in each input and stores is in counter

    parse_line(args, new_line); // parsing the line that entered by the user

    moving_to_arrays(args, new_line);//it moves each command to a specific array before pipe symbol and after

    if(args[0] == NULL)// check if the user enter nothing(enter click "\n") as command
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
        if(general_pipe == 1) // if first pipe has been found
        {
            int i;

            pid_t left,right;

            int pipefd[2];
            int pipefd2[2];

            if (pipe(pipefd) == 0)
            {
                if ((left = fork()) ==0)
                {
                    dup2(pipefd[1], STDOUT_FILENO);
                    close(pipefd[0]);
                    close(pipefd[1]);
                    execvp(array1[0], array1 );
                    fprintf(stdout, "piping has failed to execute \n");
                    exit(1);
                }
                else if ((right=fork()) ==0)
                {
                    dup2(pipefd[0], STDIN_FILENO);
                    close(pipefd[0]);
                    close(pipefd[1]);
                    execvp(array2[0], array2 );
                    fprintf(stdout, "piping has failed to execute \n");
                    exit(1);
                }
                else
                {
                    close(pipefd[0]);
                    close(pipefd[1]);
                    wait(NULL);
                    wait(NULL);

                }
            }
            else
            {
                perror("Piping Failed To Execute");
                exit(1);
            }
        }
        if(general_pipe == 2) // if second pipe has been found
        {
            pid_t left,mid,right;

            int pipefd[2];
            int pipefd2[2];

            if (pipe(pipefd) == 0 && pipe(pipefd2) == 0)
            {
                if ((left = fork()) ==0)
                {
                    dup2(pipefd[1], 1);
                    close(pipefd[0]);
                    close(pipefd2[0]);
                    close(pipefd[1]);
                    close(pipefd2[1]);

                    execvp(array1[0], array1 );
                    fprintf(stdout, "Piping Failed \n"); // if piping has failed, throw an error
                    exit(1);
                }
                else if ((mid = fork()) ==0)
                {
                    dup2(pipefd[0], 0);
                    dup2(pipefd2[1], 1);
                    close(pipefd[0]);
                    close(pipefd2[0]);
                    close(pipefd[1]);
                    close(pipefd2[1]);

                    execvp(array2[0], array2 );
                    fprintf(stdout, "Piping Failed \n"); // if piping has failed, throw an error
                    exit(1);

                }
                else if ((right = fork()) ==0)
                {
                    dup2(pipefd2[0], 0);
                    close(pipefd[0]);
                    close(pipefd2[0]);
                    close(pipefd[1]);
                    close(pipefd2[1]);

                    execvp(array3[0], array3 );
                    fprintf(stdout, "Piping failed \n");
                    exit(1);

                }
                else
                {
                    close(pipefd[0]);
                    close(pipefd2[0]);
                    close(pipefd[1]);
                    close(pipefd2[1]);

                    wait(NULL);
                    wait(NULL);
                    wait(NULL);

                }
            }
            else
            {
                perror("piping failed to execute");
                exit(1);
            }
        }
        if(general_pipe == 0) // if no pipes has been found
        {
            pid_t pid = fork();
            if(pid == -1) // if the fork return -1, throw error
            {
                perror("System Call Failed");
                exit(1);
            }
            else if(pid == 0) // if the fork return 0 , im in child
            {
                // im in child
                if(strcmp(args[0] ,"cd")==0)// checking if the the command is "cd"
                {
                    printf("Command Is Not Supported (Yet)! \n");
                    exit(0);
                }
                else
                {
                    execvp(args[0],args) ; // executing the main work in child process
                    perror("System Call Failed"); // throw error if the child cannot be executed
                    exit(0);
                }
            }

            else
            {
                // im in parent
                wait(NULL); // wait the child till the end
            }
        }

        int i;
        //replacing all the cells with spaces after each execution to be ready for the next commands
        for ( i =0; i<counter; i++)
            strcpy(args[i], " ");

        for ( i =0; i < array1_length; i++)
            strcpy(array1[i], " ");

        for ( i =0; i < array2_length; i++)
            strcpy(array2[i], " ");

        for ( i =0; i < array3_length; i++)
            strcpy(array3[i], " ");


        // saving last value of each array
        last_args_counter=counter;
        last_array1_length=array1_length;
        last_array2_Length=array2_length;
        last_array3_length=array3_length;

        // returns the value 0 as their lengths so it can be ready for the upcoming new commands
        array1_length =0;
        array2_length =0;
        array3_length =0;
        counter=0;
        general_pipe=0;
    }

    //free allocated memory
    int i;

    for ( i =0; i < last_args_counter; i++)
        free(args[i]);

    for ( i =0; i < last_array1_length; i++)
        free(array1[i]);

    for ( i =0; i < last_array2_Length; i++)
        free(array2[i]);

    for ( i =0; i < last_array3_length; i++)
        free(array3[i]);

    return 0;
}