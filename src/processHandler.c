# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/wait.h>
# include "../utils/structs.h"

#define READ 0
#define WRITE 1


pid_t reader(int* pipe, int images)
{
	pid_t pid = fork();
	if(pid < 0)
	{
		printf("Fallo de fork() en apertura de imagen.\n");
		exit(-1);
	}
	else if(pid > 0)
	{
		return pid;
	}
	else
	{

		dup2(pipe[1], STDOUT_FILENO);
		close(pipe[0]);

		execl("imageReader", "-n", "1", NULL);
		printf("Fallo de execl().\n");
		exit(-1);
	}
}



int main(int argc, char *argv[])
{

	// Falta todo lo del getopt
	int myPipe[2];
	pipe(myPipe);


	Image* img = (Image*)malloc(sizeof(Image));

	pid_t processReader = reader(myPipe, 5);

	read(myPipe[READ], img, sizeof(Image*));

	printf("Realizando lectura de imagen %i\n", img->height);

	waitpid(processReader, NULL, 0);

	return 0;
}