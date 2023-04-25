#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Practica 4 - Programacion Paralela
(updated 18-dic)
Coments:    Representacion de SpeedUp's teoricos y reales.
@author:    Pablo Goitia <pgg566@alumnos.unican.es>


NOTES:      Requiere que el programa mandelbrot_secuencial exista en el mismo directorio.
            Requiere que el programa mandelbrot exista en el mismo directorio.
            Es necesario que el programa mandelbrot_secuencial imprima los tiempos a 
            traves del canal estandar con el formato "[exec_time_seq] [exec_time_par]".
            Es necesario que el programa mandelbrot imprima los tiempos a traves del canal 
            estandar con el formato "[exec_time]".
"""
import subprocess
import matplotlib.pyplot as plt
from datetime import datetime

NUM_THREADS = 8
PROBLEM_SIZE = [2001, 2830, 3466, 4002, 4475, 4902, 5295, 5660]

def main():
    # Obtiene los tiempos medios de ejecucion
    results = obtiene_medidas()
    speedups = results[0]
    fracciones_serie = results[1]

    speedup_escalado = []
    for i in range(NUM_THREADS):
        speedup_escalado.append((i+1) + (1 - (i+1)) * fracciones_serie[i])

    # Muestra los resultados obtenidos
    print("SpeedUp escalado: " + str(speedup_escalado))
    print("Fracciones Serie: " + str(fracciones_serie))
    print("SpeedUp experimental: " + str(speedups))
    
    # Genera la grafica
    fig, ax = plt.subplots()
    plt.title("SpeedUp - Nº de hilos")
    plt.xlabel("Nº de hilos")
    plt.ylabel("SpeedUp")
    ax.plot(range(1, NUM_THREADS + 1, 1), speedups, color='olive', label='SpeedUp Experimental')
    ax.plot(range(1, NUM_THREADS + 1, 1), speedup_escalado, color='purple', label='SpeedUp Teórico')
    plt.legend()
    plt.grid()

    # Guarda grafica en fichero con nombre "results" mas la fecha y hora
    now = datetime.now()
    dt = now.strftime("%d-%m-%Y_%H-%M-%S")
    plt.savefig(f'results_{dt}.png', dpi = 150)
    plt.show()

def obtiene_medidas():
    speedups = []
    fracciones_serie = []
    for i in range(NUM_THREADS):
        time = exec_mandelbrot(i + 1)
        speedups.append(time[0])
        fracciones_serie.append(time[1])
        
    return(speedups, fracciones_serie)

def exec_mandelbrot(threads):
    print("Numero hilos: " + str(threads))
    call = './mandelbrot_secuencial ' + str(PROBLEM_SIZE[threads - 1])
    output_sec = subprocess.run(call, capture_output=True, text=True, shell=True)
    call = './mandelbrot ' + str(threads) + " " + str(PROBLEM_SIZE[threads - 1])
    output_par = subprocess.run(call, capture_output=True, text=True, shell=True)
    print(output_sec)
    print(output_par)
    if (output_sec == None or output_par == None):
        print("Algo ha fallado")
        exit(-1)

    result_sec = output_sec.stdout.split(' ')
    result_par = output_par.stdout.split(' ')

    speedup_escalado = float(result_sec[1])/float(result_par[0])
    fraccion_serie = 1 - (float(result_sec[0])/float(result_sec[1]))
    print("Fracción serie: " + str(fraccion_serie))

    return(speedup_escalado, fraccion_serie)
    
main()