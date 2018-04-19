#!/usr/bin/python3
# -*- coding: utf-8 -*-
import sys
import os
from random import shuffle

# Experimento 1: Genero 100 archivos distntos
# Experimento 2: Genero solo 1 archivo, pero armo una lista de 100 veces ese

def archivosRandom(filesCount, wordcount):
	f = open("words.txt")
	lista = [a for a in f]
	for filee in ["words/words-"+str(i)+".txt" for i in range(filesCount)]:
		shuffle(lista)
		listita = lista[:wordcount]
		fa = open(filee, "w")
		for word in listita:
			fa.write(word)

experimento = int(sys.argv[1])
repeticiones = int(sys.argv[2])
sys.stdout.write("Experimento " + str(experimento) + "\n")
if experimento == 1:
	filesCount = 100
else:
	filesCount = 1
filename = "tiempos" + str(experimento) + ".csv"
with open(filename, 'w') as f:
	f.write("p_archivos,ej5-load,ej5-merge,ej6\n")
for repeticion in range(repeticiones):
	sys.stdout.flush()
	archivosRandom(filesCount, 500)
	sys.stdout.write("\rRepetición " + str(repeticion+1) + "/" + str(repeticiones))
	os.system("./exp " + str(experimento) + " 2>>" + filename)

sys.stdout.write("\n")
quit()
