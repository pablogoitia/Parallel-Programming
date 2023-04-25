/************************************************************/
/* Nombre:		Pablo Goitia Gonzalez                       */
/* Práctica:	2                                           */
/* Fecha:		07/11/2022           	                    */
/************************************************************/
#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <sys/times.h>
#include <omp.h>

#define RAND rand() % 100
#define MAX_PRINTABLE_SIZE 10

// Global Variables
int block_size = 1;


/************************************************************/
void Init_Mat_Sup ();
void Init_Mat_Inf ();
void Multiplicar_Matrices_Secuencial ();
void Multiplicar_Matrices ();
void Multiplicar_Matrices_Sup_Secuencial ();
void Multiplicar_Matrices_Sup ();
void Multiplicar_Matrices_Inf_Secuencial ();
void Multiplicar_Matrices_Inf ();
void Escribir_Matriz ();
/************************************************************/

void main (argc, argv)
int argc;
char **argv;
{
	int dim=5;
	float *A, *B, *C;

	/* Lectura de parámetros de entrada */
	/* Usage: ./MatMul <dim> [block_size]*/
	if (argc == 2)
	{
		dim = atoi (argv[1]);
	} 
	if (argc == 3)
	{
		dim = atoi (argv[1]);
		block_size = atoi (argv[2]); 
	} 

	A = (float *) malloc (dim * dim * sizeof (float));
	B = (float *) malloc (dim * dim * sizeof (float));
	C = (float *) malloc (dim * dim * sizeof (float));

	Init_Mat_Sup (dim, A);
	Init_Mat_Inf (dim, B);
	if (dim <= MAX_PRINTABLE_SIZE)
	{
		printf ("Matriz A\n");
		Escribir_Matriz (A, dim);
		printf ("Matriz B\n");
		Escribir_Matriz (B, dim);
	}

	printf ("Multiplicar_Matrices_Secuencial()\n");
	Multiplicar_Matrices_Secuencial (A, B, C, dim);
	if (dim <= MAX_PRINTABLE_SIZE)
	{
		printf ("Matriz Resultado correcto (secuencial)\n");
		Escribir_Matriz (C, dim);
	}

	printf ("Multiplicar_Matrices()\n");
	Multiplicar_Matrices (A, B, C, dim);
	if (dim <= MAX_PRINTABLE_SIZE)
	{
		printf ("Matriz Resultado correcto (paralelo)\n");
		Escribir_Matriz (C, dim);
	}

	printf ("Multiplicar_Matrices_Sup_Secuencial()\n");
	Multiplicar_Matrices_Sup_Secuencial (A, B, C, dim);
	if (dim <= MAX_PRINTABLE_SIZE)
	{
		printf ("Matriz Resultado Superior\n");
		Escribir_Matriz (C, dim);
	}

	printf ("Multiplicar_Matrices_Sup()\n");
	Multiplicar_Matrices_Sup (A, B, C, dim);
	if (dim <= MAX_PRINTABLE_SIZE)
	{
		printf ("Matriz Resultado Superior\n");
		Escribir_Matriz (C, dim);
	}

	printf ("-------------------------------------------------\n");

	Init_Mat_Inf (dim, A);
	Init_Mat_Sup (dim, B);

	if (dim <= MAX_PRINTABLE_SIZE)
	{
		printf ("Matriz A\n");
		Escribir_Matriz (A, dim);
		printf ("Matriz B\n");
		Escribir_Matriz (B, dim);
	}

	printf ("Multiplicar_Matrices_Secuencial()\n");
	Multiplicar_Matrices_Secuencial (A, B, C, dim);
	if (dim <= MAX_PRINTABLE_SIZE)
	{
		printf ("Matriz Resultado correcto (secuencial)\n");
		Escribir_Matriz (C, dim);
	}

	printf ("Multiplicar_Matrices()\n");
	Multiplicar_Matrices (A, B, C, dim);
	if (dim <= MAX_PRINTABLE_SIZE)
	{
		printf ("Matriz Resultado correcto (paralelo)\n");
		Escribir_Matriz (C, dim);
	}

	printf ("Multiplicar_Matrices_Inf_Secuencial()\n");
	Multiplicar_Matrices_Inf_Secuencial (A, B, C, dim);
	if (dim <= MAX_PRINTABLE_SIZE)
	{
		printf ("Matriz Resultado Inferior\n");
		Escribir_Matriz (C, dim);
	}

	printf ("Multiplicar_Matrices_Inf()\n");
	Multiplicar_Matrices_Inf (A, B, C, dim);
	if (dim <= MAX_PRINTABLE_SIZE)
	{
		printf ("Matriz Resultado Inferior\n");
		Escribir_Matriz (C, dim);
	}

	exit (0);
}

void Init_Mat_Sup (int dim, float *M)
{
	int i,j,m,n,k;

	for (i = 0; i < dim; i++) {
		for (j = 0; j < dim; j++) {
			if (j <= i)
				M[i*dim+j] = 0.0;
			else
//				M[i*dim+j] = j+1;
				M[i*dim+j] = RAND;
		}
	}
}

void Init_Mat_Inf (int dim, float *M)
{
	int i,j,m,n,k;

	for (i = 0; i < dim; i++) {
		for (j = 0; j < dim; j++) {
			if (j >= i)
				M[i*dim+j] = 0.0;
			else
//				M[i*dim+j] = j+1;
				M[i*dim+j] = RAND;
		}
	}
}

void Multiplicar_Matrices_Secuencial (float *A, float *B, float *C, int dim)
{	
	int i, j, k;

	clock_t start = clock();

	for (i=0; i < dim; i++)
		for (j=0; j < dim; j++)
			C[i*dim+j] = 0.0;

	for (i=0; i < dim; i++)
		for (j=0; j < dim; j++)
			for (k=0; k < dim; k++)
				C[i*dim+j] += A[i*dim+k] * B[j+k*dim];

	printf("T_exec = %f segundos\n\n", (double)(clock()-start)/CLOCKS_PER_SEC);
	//printf("%f ", (double)(clock()-start)/CLOCKS_PER_SEC);
} 

void Multiplicar_Matrices (float *A, float *B, float *C, int dim)
{
	int i, j, k;

	double time_start = omp_get_wtime(); 

	#pragma omp parallel private(i, j, k) shared(dim, A, B, C) 
	{
		#pragma omp for
		for (i=0; i < dim; i++)
			for (j=0; j < dim; j++)
				C[i*dim+j] = 0.0;

		#pragma omp for
		for (i=0; i < dim; i++)
			for (j=0; j < dim; j++)
				for (k=0; k < dim; k++)
					C[i*dim+j] += A[i*dim+k] * B[j+k*dim];
	}

	printf("T_exec = %f segundos\n\n", omp_get_wtime() - time_start);
	//printf("%f", omp_get_wtime() - time_start);
} 

void Multiplicar_Matrices_Sup_Secuencial (float *A, float *B, float *C, int dim)
{
	int i, j, k;
	clock_t start = clock();

	for (i=0; i < dim; i++)
		for (j=0; j < dim; j++)
			C[i*dim+j] = 0.0;

	for (i=0; i < (dim-1); i++)
		for (j=0; j < (dim-1); j++)
			for (k=(i+1); k < dim; k++)
				C[i*dim+j] += A[i*dim+k] * B[j+k*dim];

	printf("T_exec = %f segundos\n\n", (double)(clock()-start)/CLOCKS_PER_SEC);
	//printf("%f ", (double)(clock()-start)/CLOCKS_PER_SEC);
} 

void Multiplicar_Matrices_Sup (float *A, float *B, float *C, int dim)
{
	int i, j, k;

	double time_start = omp_get_wtime();

	#pragma omp parallel private(i, j, k) shared(dim, A, B, C)
	{
		#pragma omp for schedule(guided)
		for (i=0; i < dim; i++)
			for (j=0; j < dim; j++)
				C[i*dim+j] = 0.0;

		#pragma omp for schedule(guided)
		for (i=0; i < (dim-1); i++)
			for (j=0; j < (dim-1); j++)
				for (k=(i+1); k < dim; k++)
					C[i*dim+j] += A[i*dim+k] * B[j+k*dim];
	}

	printf("T_exec = %f segundos\n\n", omp_get_wtime() - time_start);
	//printf("%f", omp_get_wtime() - time_start);
} 

void Multiplicar_Matrices_Inf_Secuencial (float *A, float *B, float *C, int dim)
{
	int i, j, k;
	clock_t start = clock();

	for (i=0; i < dim; i++)
		for (j=0; j < dim; j++)
			C[i*dim+j] = 0.0;

	for (i=1; i < dim; i++)
		for (j=1; j < dim; j++)
			for (k=0; k < i; k++)
				C[i*dim+j] += A[i*dim+k] * B[j+k*dim];

	printf("T_exec = %f segundos\n\n", (double)(clock()-start)/CLOCKS_PER_SEC);
	//printf("%f ", (double)(clock()-start)/CLOCKS_PER_SEC);
} 

void Multiplicar_Matrices_Inf (float *A, float *B, float *C, int dim)
{
	int i, j, k;
	double time_start = omp_get_wtime();

	#pragma omp parallel private(i, j, k) shared(dim, A, B, C)
	{
		#pragma omp for
		for (i=0; i < dim; i++)
			for (j=0; j < dim; j++)
				C[i*dim+j] = 0.0;

		#pragma omp for
		for (i=1; i < dim; i++)
			for (j=1; j < dim; j++)
				for (k=0; k < i; k++)
					C[i*dim+j] += A[i*dim+k] * B[j+k*dim];
	}

	printf("T_exec = %f segundos\n\n", omp_get_wtime() - time_start);
	//printf("%f", omp_get_wtime() - time_start);
} 


void Escribir_Matriz (float *M, int dim)
{
	int i, j;

	for (i=0; i < dim; i++)
	{
		for (j=0; j < dim; j++)
			fprintf (stdout, "%.1f ", M[i*dim+j]);
		fprintf (stdout,"\n");
	}

   printf ("\n");
}
