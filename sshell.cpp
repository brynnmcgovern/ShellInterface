/**
1. Brynn McGovern
   2370579
   bmcgovern@chapman.edu
   CPSC 380-01
   Assignment 2 - Shell Interface
2. A program to act as a shell interface. It takes in terminal commands one at a time and stores
  them in an array, then splits the commands into tokens, then forks a child process. The child
  process uses the exec() function to run the command.
*/


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <iostream>
using namespace std;
#define MAX_LINE 80

/**
  ReadCommands()
  Takes user inputed commands and stores them in a char array
  @return *char array
*/
char *ReadCommands(){
	int index = 0, bufsize = 41;
	char *args = new char[MAX_LINE/2 + 1];
	int c;

	while (1) {
		c = getchar();

		if(c == '\n' || c == EOF){
			args[index] = '\0';
			return args;
		}
		else{
			args[index] = c;
		}
		index++;
    }
}

/**
  SplitLine()
  @param *char
  Takes char item from array and splits it into tokens, then stores tokens in its own array
  @return char **array
*/
char **SplitLine(char *line){
	int i = 0, bufsize = 41;
	char *token;
	char **tokensArray = (char**) malloc(sizeof(char) * bufsize);

	token = strtok(line, " ");
	while(token != NULL){
		tokensArray[i] = token;
		i++;

		token = strtok(NULL, " \t\r\n\a");
	}
	tokensArray[i] = NULL;
	return tokensArray;
}

/**
  StartProcess()
  @param char **array
  Uses the token array from SplitLine to fork a child process and execute the command
  Handles any errors.
  @return int
*/
int StartProcess(char **args){
	pid_t pFork;

	if(strcmp(args[0], "cd") == 0){
		if(args[1] == NULL){
			perror("The following error occured: ");
		}
		else{
			if(chdir(args[1]) != 0){
				perror("The following error occured: ");
			}
		}
		return 1;
	}
	else if(strcmp(args[0], "exit") == 0){
		return EXIT_SUCCESS;
	}
  else if(strcmp(args[0], "quit") == 0){
		return EXIT_SUCCESS;
	}
  else if(strcmp(args[0], "&") == 0){
		waitpid(-1, NULL, 0);
    cout << "parent is waiting" << endl;
	}
    else{
		pFork = fork();
		if(pFork == 0){
			if(execvp(args[0], args) == -1){
				perror("The following error occured: ");
			}
			exit(EXIT_FAILURE);
		}
		else if(pFork < 0){
			perror("The following error occured: ");
		}
		else{

			waitpid(-1, NULL, 0);
		}
	    return 1;
	}
}
/**
  main()
  main method that runs the methods in a loop until user is done
*/
int main(void){

	char *line;
	char **arguments;
	int should_run;

	do{
		printf("osh> ");
		line = ReadCommands();
		arguments = SplitLine(line);
		should_run = StartProcess(arguments);
		free(line);
		free(arguments);
	} while(should_run);
}
