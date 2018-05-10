#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#define READ 0
#define WRITE 1


pid_t reader(int* pipe)
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

		execl("imageReader", "-i", NULL);
		printf("Fallo de execl().\n");
		exit(-1);
	}
}



int main(int argc, char *argv[])
{
	int pipe1[2];
	int pipe2[2];
	int pipe3[2];
	int pipe4[2];
	int pipe5[2];

	pipe(pipe1);
	pipe(pipe2);
	pipe(pipe3);
	pipe(pipe4);
	pipe(pipe5);

	char buffer[100];

	pid_t* processes = (pid_t*)calloc(5, sizeof(int));

	processes[0] = reader(pipe1);

	read(pipe1[READ], buffer, 100);


	for (int i = 0; i < 5; ++i)
	{
		waitpid(processes[i], NULL, 0);
	}

	printf("%s\n", buffer);
	printf("kasjdkjasd\n");
	return 0;
}