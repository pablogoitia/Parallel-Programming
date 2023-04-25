/*
 * File: rap.cpp
 *
 * Description: Implementacion paralela de una aproximacion 
 * de un Resource Allocation Problem.
 *
 * Author:  Pablo Goitia <pgg566@alumnos.unican.es>
 * Version: 2022-10-26
 *
*/

#include <stdio.h>		// IO operations
#include <stdlib.h>		// exit() function
#include <time.h>		// seed for random
#include <iostream>
#include <thread>
#include <mutex>
#include <ctime>
#include <ratio>
#include <chrono>

/**
 * @brief Suma los elementos en un rango de un vector.
 * @param V, puntero al vector
 * @param ini, limite inferior del rango
 * @param fin, limite superior del rango
 */
void opera(int V[], int R[], int ini, int fin);

// Variables globales
std::mutex mtx_print;

int main(int argc, char *argv[])
{
    int N, T;           // num de elementos del vector, num de threads para la suma
    int* V;             // vector problema de N elementos. Usa memory allocation para crearse
    int* R;             // vector resultado de N elementos. Usa memory allocation para crearse
    int elem_thread;    // numero de elementos que suma cada hilo

	// Gestion de errores
	if (argc != 3)
	{
		perror("Se requieren 2 argumentos.");
		exit(-1);
	}
    N = atoi(argv[1]);  // atoi = ascii to int
    T = atoi(argv[2]);
	
	// Variables
	V = (int*) calloc(N, sizeof(int));  // reserva memoria e inicializa a cero
	R = (int*) calloc(N, sizeof(int));
    std::thread threads[T - 1];
    elem_thread = (N + (T - 1)) / T;

    // Asigna valores al vector
    for (int i = 0; i < N; i++) V[i] = 1;

    using namespace std::chrono;
    steady_clock::time_point time_1 = steady_clock::now();

    // Calcula el vector Resultado. Cada hilo opera un rango proporcional de elementos
    for (int i = 0; i < T - 1; i++) // T - 1 porque tambien usamos el principal
    {
        threads[i] = std::thread(opera, V, R, i * elem_thread, (i + 1) * elem_thread);
    }

    // Ultimo hilo (principal): opera su rango proporcional mas los elementos restantes
    opera(V, R, (T - 1) * elem_thread, N);

    // Espera la finalizacion de todos los hilos
    for (int i = 0; i < T - 1; i++)
    {
        threads[i].join();
    }

    steady_clock::time_point time_2 = steady_clock::now();
    duration<double> total_duration = duration_cast<duration<double>> (time_2 - time_1);
    auto total_time = total_duration.count() * 1000;

    // Muestra resultado (solo en casos pequenhos para depurar)
    if (N < 20)
    {
        std::cout << "\nRESULTADO:\n";
        for (int i = 0; i < N; i++)
        {
            if (i < N - 1)
                std::cout << R[i] << ", ";
            else
                std::cout << R[i];
            
        }
        std::cout << "\n";
    }
    std::cout << "TIEMPO TOTAL:\n" << total_time << " ms\n\n";
    //std::cout << total_time;

    return(0);
}

void opera(int V[], int R[], int ini, int fin)
{
    using namespace std::chrono;
    steady_clock::time_point time_1 = steady_clock::now();

    R[ini] = V[ini];
    for (int i = 0; i < ini; i++)
    {
        R[ini] += V[i];
    }

    for (int i = ini + 1; i < fin; i++)
    {
        R[i] = R[i - 1] + V[i];
    }

    steady_clock::time_point time_2 = steady_clock::now();
    duration<double> total_duration = duration_cast<duration<double>> (time_2 - time_1);
    auto total_time = total_duration.count() * 1000;

    std::lock_guard<std::mutex> guard (mtx_print);
    std::cout << "El hilo que opera los elementos del " << ini << " al " << fin << " ha necesitado " << total_time << " ms\n";
}