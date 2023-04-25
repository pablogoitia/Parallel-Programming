#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Practica 3 - Programacion Paralela
(updated 17-dic)
Coments:    Obtiene los tiempos de ejecucion secuencial y paralelo, y la ganancia
            conseguida.
@author:    Pablo Goitia <pgg566@alumnos.unican.es>


NOTES:      Requiere que existan en el mismo directorio los programas video_task y 
            video_task_secuencial. Es necesario que el programa .c imprima los 
            tiempos a traves del canal estandar con el formato "[exec_time]".
"""
import subprocess
import matplotlib.pyplot as plt
from datetime import datetime

NUM_MUESTRAS = 10

def main():
    # Obtiene los tiempos medios de ejecucion
    output = exec_mat_mul()
    fichero_resultados(output) # los exporta a un fichero

def exec_mat_mul():
    """ Obtiene tiempos de ejecucion.
    Returns
    -------
    output
        String con todos los datos.
    """
    output = "\nEXECUTION TIMES:\n\n"
    sec = par = 0
    muestras_sec = []
    muestras_par = []
    for j in range(NUM_MUESTRAS):
        print("Muestra: " + str(j))
        call = './video_task_secuencial'
        result = call_program(call)
        muestras_sec.append(float(result[0]))
        call = './video_task'
        result = call_program(call)
        muestras_par.append(float(result[0]))
        
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

def call_program(call):
    out = subprocess.run(call, capture_output=True, text=True, shell=True)
    if (out == None):
        print("Algo ha fallado")
        exit(-1)
    result = out.stdout.split(' ')
    return result

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