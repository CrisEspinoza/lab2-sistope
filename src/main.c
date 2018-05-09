# include <stdio.h>
# include <stdlib.h>
# include <ctype.h>
# include <unistd.h>

# include "menu.c"
# include "structs.h"

int main(int argc, char *argv[])
{
	/* code */
	  int quantityImages;
  	int umbralImages;
  	int umbralClassification;
  	int flag;
  	int c;

  	opterr = 0;

  	if (argc < 9)
  	{
   		printf("La cantidad de parametros en menor a la solicitada \n");
    	return 0;
  	}


  	if (argc > 9)
  	{
    	printf("La cantidad de parametros en mayor a la solicitada \n");
    	return 0;
  	}

  	while ((c = getopt (argc, argv, "c:u:n:b:")) != -1)
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
        	sscanf(optarg, "%d", &flag);
        	break;
      	case '?':
        	if (optopt == 'c' || optopt == 'u' || optopt == 'n' || optopt == 'b')
          		fprintf (stderr, "Opcion -%c requiere un argumento.\n", optopt);
        	else if (isprint (optopt))
          		fprintf (stderr, "Opcion desconocida `-%c'.\n", optopt);
        	else
          		fprintf (stderr,
                   "Opcion con caracter desconocido `\\x%x'.\n",
                   optopt);
        	return 1;
      	default:
        	abort ();
      	}

    // Falta agregar las restricciones de los valores que nos tiene que ingresar por consola ! 
    //printf("%d\n", isdigit(quantityImages) );    
    if ( umbralImages <= 0 && umbralImages >= 255  )
    {
      printf("EL umbralImages (-u ) tiene que ser mayor a 0 y menor a 255\n");
      return 1;
    }
    if ( flag != 0 && flag != 1 )
    {
      printf("EL flag (-d ) tiene que ser seteado en 0 o 1 \n");
      return 1;
    }
    if ( umbralClassification >= 100 && umbralClassification <= 0)
    {
      printf("La varible umbralClassification (-n ) tiene que ser mayor a 0 y menor a 100 \n");
      return 1;
    }

    menu(quantityImages,umbralImages,umbralClassification,flag);


  return 0;
	
}