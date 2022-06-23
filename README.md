# C-Linux-Shell
C programmed dummy shell with implementation of pipes and execv

implementation of a simple, interactive shell program that prompts the user for a command, parses
the command, and then executes it with a child process. In provided solution there is a use of
execv(), which reads the PATH environment, then search each directory in
the PATH for the command file name that appears on the command line.
