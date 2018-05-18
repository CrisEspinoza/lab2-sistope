# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/wait.h>
# include "../utils/structs.h"

# define READ 0
# define WRITE 1


pid_t reader(int* pipe, int images)
{
	pid_t pid = fork();
	if(pid < 0)
	{
		perror("Fallo de fork() en el main.");
		exit(-1);
	}
	else if(pid > 0)
	{
		close(pipe[WRITE]);
		return pid;
	}
	else
	{
		close(pipe[READ]); // se cierra canal de lectura
		dup2(pipe[WRITE], STDOUT_FILENO);
		execlp("./imageReader", "-n", "2", NULL);
		printf("Fallo de execl().\n");
		exit(-1);
	}
}



int main(int argc, char *argv[])
{

	// Falta todo lo del getopt

	int myPipe[2];
	pipe(myPipe);
	char string[40];

	//Image* img = (Image*)malloc(sizeof(Image));

	pid_t processReader = reader(myPipe, 5);
	
	wait(&processReader);

	read(myPipe[READ], string, 40);

	printf("Mensaje desde imageReader: %s\n", string);

	printf("Terminaron los procesos.\n");


	return 0;
}