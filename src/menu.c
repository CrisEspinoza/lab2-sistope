#include <stdio.h>

# include "imagelib.c"

// EL menu de la aplicación que se le muestra al usuario para que realice las funcionalidades del laboratorio

void menu(int quantityImages,int umbralImages,int umbralClassification,int flag)
{

    int opcion;

    do
    {
        printf( "\n   1. Comenzar pipeline");
        printf( "\n   2. Creditos" );
        printf( "\n   3. Salir" );
        printf( "\n\n   Introduzca opcion (1-3): ");

        scanf( "%d", &opcion );

        /* Inicio del anidamiento */

        switch ( opcion )
        {
            case 1: pipeline(quantityImages,flag,umbralClassification,umbralImages);
                    break;

            case 2: printf(" * Autor: Javier Arrendondo && Cristian Espinoza \n ");
                    printf(" * Universidad santiago de chile \n");
                    break;
            default:
                    if(opcion != 3)
                        printf("Esta opcion no esta permitida.\n");
                    break;
        }

    }while(opcion!=3);
        
}