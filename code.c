#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<sys/stat.h>
#include <fcntl.h>    
#include<sys/stat.h>
#include"code.h"


int main(int argc,char* argv[])
{	
	
	char * allPaths[MAX_PATHS];
	parsePath(allPaths); 	// Get directory paths from PATH 
	
	while(1) {
		char * parsedCommand[MAX_ARGS];
		char * userCommands = (char*)malloc(LINE_LEN);
	  
		printPrompt();	// prints machine name and directories
		
		readCommand(userCommands);	//read command of user
		
		int totalCommands = parseCommand(userCommands, parsedCommand); // tokenize commmand line arguments
		char * execute = lookupPath(allPaths, parsedCommand); // find matching path of user command
		
		if(execute == NULL){
            		continue;
        	}
        	
        	else{
            		int pid = fork();//creating child process
            		
            		if(pid == 0) {
               		execv(parsedCommand[0], parsedCommand);	//execute the command.
            		}
            		wait(NULL);//wait for the child process to terminate 
        	}
        
        free(userCommands);
	}
	
	
	return 0;
	
}



