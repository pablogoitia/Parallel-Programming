/************************************************************/
/* Nombre:		Pablo Goitia Gonzalez                        */
/* Práctica:	3                                            */
/* Fecha:		14/11/2022           	                     */
/************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <omp.h>

void fgauss (int *, int *, int, int);

int main(int argc, char *argv[]) {

   FILE *in;
   FILE *out;
   int i, j, size, seq = 8;
   int **pixels, **filtered;
   double inicio, final;

   if (argc == 2) seq = atoi (argv[1]);
  
   in = fopen("movie.in", "rb");
   if (in == NULL) {
      perror("movie.in");
      exit(EXIT_FAILURE);
   }

   out = fopen("movie.out", "wb");
   if (out == NULL) {
      perror("movie.out");
      exit(EXIT_FAILURE);
   }

   int width, height;

   fread(&width, sizeof(width), 1, in);
   fread(&height, sizeof(height), 1, in);

   fwrite(&width, sizeof(width), 1, out);
   fwrite(&height, sizeof(height), 1, out);

   pixels = (int **) malloc (seq * sizeof (int *));
   filtered = (int **) malloc (seq * sizeof (int *));

   for (i=0; i<seq; i++)
   {
      pixels[i] = (int *) malloc((height + 2) * (width + 2) * sizeof(int));
      filtered[i] = (int *) malloc((height + 2) * (width + 2) * sizeof(int));
   }

   #ifdef _OPENMP
	  inicio = omp_get_wtime();
   #endif

   i = 0;
   #pragma omp parallel shared(pixels, filtered, width, height)
   {
      #pragma omp master
      {
         do 
         {
            // lee una imagen
            size = fread(pixels[i], (height + 2) * (width + 2) * sizeof(int), 1, in);
            if (size)
            {
               // delega la tarea de aplicar el filtro a un thread hijo
               #pragma omp task firstprivate(i)
               {
                  fgauss (pixels[i], filtered[i], height, width);
               }
               i = (i + 1) % seq; // actualiza i
            }
            else
            {
               /**
                * Si ha entrado aqui es porque se esta aplicando el filtro al ultimo 
                * conjunto de imagenes, y la cantidad de imagenes en este es menor a "seq".
                * Es necesario indicar que el nuevo seq es dicho tamanho, y forzar la entrada
                * a la parte del codigo que escribe el resultado en el fichero. No hace falta
                * reestablecer de nuevo los valores porque esta seria la ultima iteracion del
                * "do while" porque se ha alcanzado el end of file.
                */
               seq = i;
               i = 0;
            }

            if (i == 0)
            {
               #pragma omp taskwait
               // si se llega aqui, el bloque de "seq" imgs esta listo para ser escrito
               for (int s = 0; s < seq; s++)
                  fwrite(filtered[s], (height + 2) * (width + 2) * sizeof(int), 1, out);
            }
         } while (!feof(in)); // no quedan imgs por leer
      }
   }

   #ifdef _OPENMP
	final = omp_get_wtime();
	printf("Tiempo: %f\n", final - inicio);
	//printf("%f", final - inicio);
   #endif

   for (i=0; i<seq; i++)
   {
      free (pixels[i]);
      free (filtered[i]);
   }
   free(pixels);
   free(filtered);

   fclose(out);
   fclose(in);

   return EXIT_SUCCESS;
}

void fgauss (int *pixels, int *filtered, int heigh, int width)
{
	int y, x, dx, dy;
	int filter[5][5] = {1, 4, 6, 4, 1, 4, 16, 26, 16, 4, 6, 26, 41, 26, 6, 4, 16, 26, 16, 4, 1, 4, 6, 4, 1};
	int sum;

	for (x = 0; x < width; x++)
   {
		for (y = 0; y < heigh; y++)
		{		
			sum = 0;
			for (dx = 0; dx < 5; dx++)
				for (dy = 0; dy < 5; dy++)
					if (((x+dx-2) >= 0) && ((x+dx-2) < width) && ((y+dy-2) >=0) && ((y+dy-2) < heigh))
						sum += pixels[(x+dx-2)*heigh+(y+dy-2)] * filter[dx][dy];
			filtered[x*heigh+y] = (int) sum/273;
		}
	}
}

