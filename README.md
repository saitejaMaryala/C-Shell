[![Review Assignment Due Date](https://classroom.github.com/assets/deadline-readme-button-22041afd0340ce965d47ae6ef1cefeee28c7c493a6346c4f15d667ab976d596c.svg)](https://classroom.github.com/a/Qiz9msrr)
# mini-project-1-template

### spec 1 display
  
  I have read about the pwd.h ,getuid,getpwuid and gethost name to get the username and sytemname,I asked the chatgpt to retrieve a relative path for my home path

### spec 2 input requirements
  I have tokenized the input based on ; and then I ran each command while running I checked if there is & in it ,if it is then I ran the command before the & as background,else foreground


### spec 3 hop
  I will be in the home directory which is the directory of the main file.from here I use chdir to jump into a specific directory,if no such directory or path is found then program gives an error that no such directory found


### spec 4 reveal
  I maintained two int variables which has information about flags(-a and -l) . If no path is given then it will consider the present directory to print the files.
  When a item name is given it will check for if it is directory or it is a file so it will print some more errors
  and last if program not able to find the directory or file it prints no such directory or file.


### spec 5 log
  I maintained a history1.txt file which stores the past events for last session as well.I created functions to retrieve ,delete and show past commands


### spec 6 system commands
  #### foreground
    I executed the foreground using the execvp for system commands and if a process takes control of cpu for more than 2 seconds it will be shown.
  #### background
    The program will show the pid and exited with a satus when the program ends,if there an interactive process like 
vim it won't terminate so I redirected the stdout,stdin,stderr to dev/null because of which it will terminate with the exit status 1. where as sleep and other non interactive processes exit normally

 I implemented the backgound only for the system commands which are executed in the execvp!!

### spec 7 prolore
  I asked Chat gpt where to find the process information and how to retrieve it. I used the group id of foreground and the group id of given pid to check whether it is foreground or background

### spec 8 seek
  I have int variables of each flag.When user enter -e without -f and -d it has no effect on the output it executes as the -e never existed. When user enters -e with -f or -d then checks for number of fies with that name if more than one is found then it prints multiple files and exits.
  If no match of the given file is found then it prints no match found

### General
   I assumed the length of any path to be 1000 - 4096

   For any flags entered like -l-a is considered invalid the valid one is -a -l (same like bash). The flag should start with '-' and shouldn't conatain '-' any where in the flag
    When you run the makefile it will create an executable file named shell.

