String Parser
Authored by : Hakam Nabulssi


==Description==

Spliting a string into a series of tokens based on a particular delimiter. A token is a substring extracted from the original string. Whereas, in addition to that, it counts how many tokens there are in the string and calculates the length of each word in the string


Functions:
this proccesse has only the main function, We allocate the memory dynamically to store the string that we've received by the user. After getting a string, we enter the first loop that counts the number of words in the string .
the main contain also a second loop that counts the length of each word of the string. 
NOTE: The process will be repeated infinite times until the user enters the "exit" word as a string


==Program Files==
EX1.c - contains only the main


==compiling Steps==
compile: gcc EX1.c -o EX1
run: ./EX1

==Input:==
it's our first project in this course 

==Output:==
7 words
it's: 4
our: 3
first: 5
project: 7
in: 2
this: 4
course: 6