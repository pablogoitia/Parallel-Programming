#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Practica 1 - Programacion Paralela
(updated 13-dic)
Coments:    Obtiene los tiempos de ejecucion del programa "reduccion.cpp"
            y los representa en una grafica.
@author:    Pablo Goitia <pgg566@alumnos.unican.es>


NOTES:      Requiere que el programa reduccion exista en el mismo directorio.
            Es necesario que el programa .c imprima los tiempos a traves del 
            canal estandar con el formato "[exec_time]".
"""
import subprocess
import matplotlib.pyplot as plt
from datetime import datetime

NUM_THREADS = 16
NUM_ELEMENTS = 100000000
NUM_MUESTRAS = 10

def main():
    # Obtiene los tiempos medios de ejecucion
    tiempos = obtiene_medidas()
    fichero_resultados(tiempos) # los exporta a un fichero
    
    # Genera la grafica
    num_hilos = list(range(1, NUM_THREADS + 1))
    fig, ax = plt.subplots()
    plt.title("Tiempo - Nº de hilos")
    plt.xlabel("Nº de hilos")
    plt.ylabel("Tiempo (ms)")
    ax.plot(num_hilos, tiempos, color='blue')
    plt.grid()

    # Guarda grafica en fichero con nombre "results" mas la fecha y hora
    now = datetime.now()
    dt = now.strftime("%d-%m-%Y_%H-%M-%S")
    plt.savefig(f'results_{dt}.png', dpi = 150)
    plt.show()

def obtiene_medidas():
    """ Obtiene el tiempo de ejecucion medio de una muestra de por cada
    cantidad de hilos.

    Returns
    -------
    list
        Tiempos de ejecucion del programa para distintas cantidades de hilos.
    """
    tiempos = []
    for i in range(NUM_THREADS):
        muestras = exec_suma(i)
        suma = 0
        for j in range(NUM_MUESTRAS):
            suma += muestras[j]
        media = suma / NUM_MUESTRAS
        desviacion_superior = media + 0.05 * media
        desviacion_inferior = media - 0.05 * media

        suma = datos_validos = 0
        for j in range(len(muestras)):
            if (muestras[j] <= desviacion_superior or muestras[j] >= desviacion_inferior):
                suma += muestras[j]
                datos_validos += 1
        tiempos.append(suma / datos_validos)
        
    return tiempos

def exec_suma(threads):
    """ Genera una muestra de tiempos de ejecucion para la cantidad de hilos
    pasada como parametro.
    Parameters
    ----------
    num_threads : int
        Numero de hilos de los que se obtiene la muestra.
    Returns
    -------
    list
        Muestra de tiempos de ejecucion para el numero de hilos recibido.
    """
    muestras = []
    for i in range(NUM_MUESTRAS):
        print("Numero hilos: " + str(threads + 1) + " llamada: " + str(i + 1))
        call = './reduccion ' + str(NUM_ELEMENTS) + " " + str(threads + 1)
        output = subprocess.run(call, capture_output=True, text=True, shell=True)
        print(output)
        if (output == None):
            print("Algo ha fallado")
            exit(-1)
        result = output.stdout.split(' ')
        time = float(result[0])
        print("Tiempo empleado: " + str(time))
        print("¿Resultado correcto?: " + str(float(output.stdout.split(' ')[1]) == NUM_ELEMENTS))
        muestras.append(time)

    return(muestras)

def fichero_resultados(tiempos):
    """ Actualiza el fichero con el registro de tiempos de ejecucion calculados.
    Parameters
    ----------
    tiempos : list
        Tiempos medios de ejecucion por cada hilo a almacenar en el fichero.
    """
    now = datetime.now()
    dt = now.strftime("%d-%m-%Y_%H-%M-%S")
    f = open("time.txt", "a")
    f.write(f'[{dt}] ' + str(tiempos) + "\n")
    f.close()
    
main()