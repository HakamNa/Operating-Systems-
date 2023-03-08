//Name : Hakam Nabulssi

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_SIZE 510

int main()
{
    char *str;
    //allocate memory dynamically
    str=(char*)malloc(MAX_SIZE*sizeof(char));
    int count = 0,i = 0, counter =0;
    int last_was_space = 1;
    while(2)
    {
        printf("Enter the string:\n");
        gets(str);
        if(strcmp(str,"exit")!=0)
        {
            for (i = 0; str[i] != '\0'; i++)
            {

                if (str[i] == ' ')
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
                    if (str[i] != '\n')
                        last_was_space = 0;
                }
            }
            if (last_was_space == 0)
                ++count; // Count the last word if there wasn't a space before

            printf("%d words\n", count );
            count = 0 ;
            for(i=0; str[i]!='\0'; i++)
            {
                if(str[i]!=' '&&(str[i+1]==' '||str[i+1]=='\0'))
                {
                    printf("%c", str[i]);

                    printf(": %d \n",counter+1 );

                    counter=0;
                }
                else if(str[i]!=' ')
                {
                    printf("%c", str[i]);
                    counter++;



                }
                else
                    counter=0;
            }

        }
        else
            exit(0);
        //free dynamically allocated memory
        free(str);
    }
    return 0;

}
