/*
 * File: reduccion.cpp
 *
 * Description: Implementacion paralela de un sumatorio.
 *
 * Author:  Pablo Goitia <pgg566@alumnos.unican.es>
 * Version: 2022-10-19
 *
 */

#include <stdio.h>  // IO operations
#include <stdlib.h> // exit() function
#include <time.h>   // seed for random
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
void sumar(int V[], int ini, int fin);

// Variables globales
std::mutex mtx_suma;
std::mutex mtx_print;
static int suma = 0;

int main(int argc, char *argv[])
{
    int N, T;        // num de elementos del vector, num de threads para la suma
    int *V;          // vector de N elementos. Usa memory allocation para crearse
    int elem_thread; // numero de elementos que suma cada hilo

    // Gestion de errores
    if (argc != 3)
    {
        perror("Se requieren 2 argumentos.");
        exit(-1);
    }
    N = atoi(argv[1]); // atoi = char to int
    T = atoi(argv[2]);
    if (N <= 1 || T <= 0)
    {
        perror("Parametros no admitidos.");
        exit(-1);
    }

    // Variables
    V = (int *)calloc(N, sizeof(int)); // reserva memoria e inicializa a cero
    std::thread threads[T - 1];
    elem_thread = (N + (T - 1)) / T;

    // Asigna valores al vector
    for (int i = 0; i < N; i++)
        V[i] = 1;

    using namespace std::chrono;
    steady_clock::time_point time_1 = steady_clock::now();

    // Suma los valores del vector
    for (int i = 0; i < T - 1; i++) // T - 1 porque tambien usamos el principal
    {
        threads[i] = std::thread(sumar, V, i * elem_thread, (i + 1) * elem_thread);
    }

    sumar(V, (T - 1) * elem_thread, N);

    // Espera la finalizacion de todos los hilos
    for (int i = 0; i < T - 1; i++)
    {
        threads[i].join();
    }

    steady_clock::time_point time_2 = steady_clock::now();
    duration<double> total_duration = duration_cast<duration<double>>(time_2 - time_1);
    auto total_time = total_duration.count() * 1000;

    // Muestra resultado
    std::cout << "\nRESULTADO:\n" << suma << "\n\n";
    std::cout << "TIEMPO TOTAL:\n" << total_time << " ms\n\n";
    //std::cout << total_time << " " << suma;

    return (0);
}

void sumar(int V[], int ini, int fin)
{
    using namespace std::chrono;
    steady_clock::time_point time_1 = steady_clock::now();
    int suma_parcial = 0;

    for (int i = ini; i < fin; i++)
    {
        suma_parcial += V[i];
    }

    // Proteger con mutex
    std::lock_guard<std::mutex> guard_suma(mtx_suma);
    suma += suma_parcial;

    steady_clock::time_point time_2 = steady_clock::now();
    duration<double> total_duration = duration_cast<duration<double>>(time_2 - time_1);
    auto total_time = total_duration.count() * 1000;

    std::lock_guard<std::mutex> guard_print(mtx_print);
    std::cout << "La suma del elemento " << ini << " a " << fin << " lleva " << total_time << " ms\n";
}