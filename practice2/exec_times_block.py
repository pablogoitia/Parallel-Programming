#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Practica 2 - Programacion Paralela
(updated 16-dic)
Coments:    Obtiene los tiempos de ejecucion secuencial y paralelo, y la ganancia
            conseguida para distintos tamanhos de bloque.
@author:    Pablo Goitia <pgg566@alumnos.unican.es>


NOTES:      Requiere que el programa .c imprima los resultados a traves del canal
            estandar con el formato "[tiempo_secuencial]" "[tiempo_paralelo]".
"""
import subprocess
import matplotlib.pyplot as plt
from datetime import datetime

BLOCK_SIZES = [1, 10, 20, 30, 40, 50, 60, 70, 80, 90, 100, 110, 120, 130]
NUM_MUESTRAS = 10
MATRIX_SIZE = 1224

def main():
    # Obtiene los tiempos medios de ejecucion
    output = exec_mat_mul()
    fichero_resultados(output) # los exporta a un fichero

def exec_mat_mul():
    """ Obtiene tiempos de ejecucion para la los distintos tamanhos de bloque.
    Returns
    -------
    output
        String con todos los datos.
    """
    output = "\nEXECUTION TIMES:\n\n"

    for i in BLOCK_SIZES:
        sec = par = 0
        output += "Tamaño de bloque: " + str(i) + "\n"
        muestras_sec = []
        muestras_par = []
        print("Tamaño de bloque: " + str(i))
        for j in range(NUM_MUESTRAS):
            print("Muestra: " + str(j))
            call = './MatMul ' + str(MATRIX_SIZE) + " " + str(i)
            out = subprocess.run(call, capture_output=True, text=True, shell=True)
            if (out == None):
                print("Algo ha fallado")
                exit(-1)
            result = out.stdout.split(' ')
            muestras_sec.append(float(result[0]))
            muestras_par.append(float(result[1]))
        
        for j in range(NUM_MUESTRAS):
            sec += muestras_sec[j]
            par += muestras_par[j]
        media_sec = sec / NUM_MUESTRAS
        media_par = par / NUM_MUESTRAS
        desviacion_superior_sec = media_sec + 0.05 * media_sec
        desviacion_inferior_sec = media_sec - 0.05 * media_sec
        desviacion_superior_par = media_par + 0.05 * media_par
        desviacion_inferior_par = media_par - 0.05 * media_par

        sec = par = datos_validos_sec = datos_validos_par = 0
        for j in range(len(muestras_sec)):
            aux = muestras_sec[j]
            if (aux <= desviacion_superior_sec or aux >= desviacion_inferior_sec):
                sec += aux
                datos_validos_sec += 1
        for j in range(len(muestras_par)):
            aux = muestras_par[j]
            if (aux <= desviacion_superior_par or aux >= desviacion_inferior_par):
                par += aux
                datos_validos_par += 1

        sec /= datos_validos_sec
        par /= datos_validos_par

        print("Tiempo secuencial: " + str(round(sec, 2)))
        print("Tiempo paralelo: " + str(round(par, 2)))
        print("SpeedUp: " + str(round(sec / par, 2)) + "\n")
        output += "Tiempo secuencial: " + str(round(sec, 2)) + "\n"
        output += "Tiempo paralelo: " + str(round(par, 2)) + "\n"
        output += "SpeedUp: " + str(round(sec / par, 2)) + "\n\n"
    return(output)

def fichero_resultados(tiempos):
    """ Actualiza el fichero con el registro de tiempos de ejecucion calculados.
    Parameters
    ----------
    tiempos : list
        Tiempos de ejecucion por cada tamaño de bloque a almacenar en el fichero.
    """
    now = datetime.now()
    dt = now.strftime("%d-%m-%Y_%H-%M-%S")
    f = open("time_" + dt + ".txt", "a")
    f.write(str(tiempos))
    f.close()
    
main()