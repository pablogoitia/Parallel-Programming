#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Practica 4 - Programacion Paralela
(updated 18-dic)
Coments:    Representacion de SpeedUp's teoricos y reales.
@author:    Pablo Goitia <pgg566@alumnos.unican.es>


NOTES:      Requiere que el programa mandelbrot exista en el mismo directorio.
            Es necesario que el programa .c imprima los tiempos a traves del 
            canal estandar con el formato "[exec_time]".
"""
import subprocess
import matplotlib.pyplot as plt
from datetime import datetime

NUM_THREADS = 8
SECUENCIAL_TIME = 84.345105
PARALLEL_TIME = 73.022412

def main():
    # Obtiene los tiempos medios de ejecucion
    speedups = obtiene_medidas()

    speedup_teorico = []
    p = PARALLEL_TIME/SECUENCIAL_TIME
    s = 1 - p
    for i in range(NUM_THREADS):
        speedup_teorico.append((1)/(s+(p/(i+1))))
    
    # Genera la grafica
    fig, ax = plt.subplots()
    plt.title("SpeedUp - Nº de hilos")
    plt.xlabel("Nº de hilos")
    plt.ylabel("SpeedUp")
    ax.plot(range(1, NUM_THREADS + 1, 1), speedups, color='olive', label='SpeedUp Experimental')
    ax.plot(range(1, NUM_THREADS + 1, 1), speedup_teorico, color='purple', label='SpeedUp Teórico')
    plt.legend()
    plt.grid()

    # Muestra los resultados obtenidos
    print(speedup_teorico)
    print(speedups)

    # Guarda grafica en fichero con nombre "results" mas la fecha y hora
    now = datetime.now()
    dt = now.strftime("%d-%m-%Y_%H-%M-%S")
    plt.savefig(f'results_{dt}.png', dpi = 150)
    plt.show()

def obtiene_medidas():
    speedups = []
    for i in range(NUM_THREADS):
        time = exec_mandelbrot(i + 1)
        speedups.append(time)
        
    return speedups

def exec_mandelbrot(threads):
    print("Numero hilos: " + str(threads))
    call = './mandelbrot ' + str(threads)
    output = subprocess.run(call, capture_output=True, text=True, shell=True)
    print(output)
    if (output == None):
        print("Algo ha fallado")
        exit(-1)
    result = output.stdout.split(' ')

    return(SECUENCIAL_TIME/float(result[0]))
    
main()