#define l192279
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<sys/stat.h>
#include <fcntl.h>    
#include<sys/stat.h>

/* Defining constants for shell */
#define LINE_LEN 80                          // Total length of each command line.
#define MAX_ARGS 64                          // maximum number of arguments of each command.
#define MAX_ARG_LEN 16                       // length of each argument
#define MAX_PATHS 64                         // maximum number of paths        
#define MAX_PATH_LEN 96                      // he maximum length of each path.
#define WHITESPACE " .,\t\n"                 // Delimeters for command line.
#define TRUE 1                               // 1 for TRUE 
#ifndef NULL                                 // if NULL is not defined, then define NULL as 0
#define NULL 0
#endif



void printPrompt();
char * getCommand(char *, char *);
int mergeArrays(char *, char *);
int parsePath(char **);
void readCommand(char *);
int parseCommand(char *, char**);
char * lookupPath(char **, char **);




char * getCommand(char * command, char * result) {

	int len = strlen(command); // calculating length of command
	char path[len+6];
	strcpy(path, "/bin/");

	for(int i=5, j=0; command[j] != '\0'; i++, j++){
		path[i] = command[j];	// getting complete path of current command 
	}
	path[len+5] = '\0';
	char * commandArg[2];
	commandArg[0] = command;
	commandArg[1] = NULL;
	
	int p[2], pid; 

	pipe(p); // Creating Pipe
	int read_p = p[0];
	int write_p= p[1];

	pid = fork();
	if(pid == 0) { // Child Process
		
		close(p[0]); //Close read end
		dup2(p[1],1); // Bind Standard output
		
        	execv(path, commandArg);
		close(p[1]);
		return 0;
	} 
	else {
		wait(NULL);
		close(p[1]); // close write end
		FILE  * fd = fdopen(p[0],"r");
		char buffer[1024];
		fgets(buffer,1024,fd);
		fclose(fd);
		close(p[0]);
		strcpy(result, buffer);
		return result;

	}	
	
}	

// merge arrays and return current length of destination array
int mergeArrays(char * des, char * src){
	int len1 = strlen(des);
	for(int j=0; src[j] != '\0'; j++){
		des[len1++] = src[j];	// getting complete path of current command 
	}
	return len1;
}







/* printing machine name, 
current directory, or other desired information. */
void printPrompt() {
	char final[1024];
	char temp[1024];
	getCommand("whoami", temp);	// getting name of machine
	int len = strlen(temp);
	temp[len-1] = '\0';
	strcpy(final, temp);
	strcat(final, "@");
	strcat(final, temp);
	
	temp[0] = '\0';
	getCommand("pwd", temp);	// getting directories
	len = strlen(temp);
	temp[len-1] = '\0';
	strcat(final, temp);
	strcat(final, ":");
	printf("%s", final); // printing prompt
}

int parsePath(char * allPaths[]){
	char * pathEnv = getenv("PATH");	// get all environemtns
	int len = strlen(pathEnv) +1;
    	char * temp=(char*)malloc(len);
    	strcpy(temp, pathEnv);

	for(int i=0; i<MAX_PATHS; i++){
		allPaths[i] = strsep(&temp, ":");
			if(allPaths[i] == NULL)
			    break;
	}
	return 0;
}

void readCommand(char * userCommands){
	fgets(userCommands, 80, stdin); //store command entered by user
}

int parseCommand(char * userCommand, char * parsedCommand[]){
	// get all arguments in form of tokens
	int len = strlen(userCommand) +1;
    	char * temp=(char*)malloc(len);
    	strcpy(temp, userCommand);

	for(int i=0; i <MAX_ARGS; i++){
		parsedCommand[i] = strsep(&temp, " ");
			if(parsedCommand[i] == NULL)
			    break;
	}
}

char * lookupPath(char * allPaths[], char * parsedCommand[]){
	// if path entered is absolute
	if(*parsedCommand[0] == '/'){
		if(access(parsedCommand[0], 0) == 0){
		    printf("lookupPath: Found %s\n", parsedCommand[0]);
		    return parsedCommand[0];
		}
		fprintf(stderr,"%s : command not found\n", parsedCommand[0]);
		return NULL;
    	}
    	
    	// if path entered is relative
    	char final[MAX_PATH_LEN];
    	for(int i = 0; allPaths[i]!=NULL && i < MAX_PATHS; i++){	// binding complete directory by joining
    		char temp[MAX_PATH_LEN];
    		strcpy(temp, allPaths[i]);	// path with the user command placed at first index 
    		strcat(temp, "/");
    		strcat(temp, parsedCommand[0]);
    		int len = strlen(temp);
    		if(parsedCommand[1] == NULL)
    			temp[len-1] = '\0';
    		strcpy(final, temp);
    		if(access(final, 0) == 0){ // after binding, check if commnd is valid 
		    strcpy(parsedCommand[0], final); // for valid command, make it ready fpr execution
		    return parsedCommand[0];
        	}
        	
    	}
    	
    	//invalid path(neither absolute nor relative)
	fprintf(stderr,"%s: no such Directory exists\n", parsedCommand[0]);
	return NULL;
    	
}


