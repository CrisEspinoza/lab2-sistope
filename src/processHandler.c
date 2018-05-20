# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <string.h>
# include <sys/wait.h>
# include "../utils/structs.h"

# define READ 0
# define WRITE 1


pid_t reader(int* pipe, int nblack, int umbral, int flag, int image)
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
		char nblackstr [5];
		char umbralstr [5];
		char flagstr [5];
		char imagestr [5];
		sprintf(nblackstr, "%d", nblack);
		sprintf(umbralstr, "%d", umbral);
		sprintf(flagstr, "%d", flag);
		sprintf(imagestr, "%d", image);
		close(pipe[READ]);
		dup2(pipe[WRITE], STDIN_FILENO);
		execlp("./imageReader", "-n", nblackstr, "-u", umbralstr, "-f", flagstr, "-i", imagestr, NULL);
		exit(-1);
	}
}



int main(int argc, char *argv[])
{

	int quantityImages;
  	int umbralImages;
  	int umbralClassification;
  	int flag = 0;
  	int c;

  	opterr = 0;

  	if (argc < 8)
  	{
   		printf("La cantidad de parametros en menor a la solicitada \n");
    	return 0;
  	}


  	if (argc > 8)
  	{
    	printf("La cantidad de parametros en mayor a la solicitada \n");
    	return 0;
  	}

  	while ((c = getopt (argc, argv, "c:u:n:b")) != -1)
    	switch (c)
      	{
      	case 'c':
        	sscanf(optarg, "%d", &quantityImages);
        	break;
      	case 'u':
        	sscanf(optarg, "%d", &umbralImages);
        	break;
      	case 'n':
	      	sscanf(optarg, "%d", &umbralClassification);
        	break;
      	case 'b':
        	flag = 1;
        	break;
      	case '?':
        	if (optopt == 'c' || optopt == 'u' || optopt == 'n' || optopt == 'b')
          		fprintf (stderr, "Opcion -%c requiere un argumento.\n", optopt);
      	default:
        	abort ();
      	}
  
    if ( umbralImages <= 0 && umbralImages >= 255  )
    {
      printf("EL umbralImages (-u ) tiene que ser mayor a 0 y menor a 255\n");
      return 1;
    }

    if ( umbralClassification >= 100 && umbralClassification <= 0)
    {
      printf("La varible umbralClassification (-n ) tiene que ser mayor a 0 y menor a 100 \n");
      return 1;
    }


	int myPipe[2];
	pipe(myPipe);
	char string[40];

	perror("HANDLER: Inicio");

	int i;
	if (flag == 1)
	{
		printf("\n._________________________________.\n");
		printf("|     image   |    nearly black   |\n");
		printf("|---------------------------------|\n");
	}
	for (i = 0; i < quantityImages; i++)
	{
		pid_t processReader = reader(myPipe, umbralClassification, umbralImages, flag, i + 1);
		read(myPipe[READ], string, 40);
		wait(&processReader);
	}
	if (flag == 1)
		printf("|_________________________________|\n");
	return 0;
}