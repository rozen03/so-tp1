#!/usr/bin/python3
# -*- coding: utf-8 -*-
from random import shuffle
wordcount=500
portentaje=20
filesCount=100
f = open("words.txt")
lista = [a for a in f]
shuffle(lista)
cantidadRepetidos=(wordcount*portentaje)//100
listaRepetidos=lista[:cantidadRepetidos]
print(cantidadRepetidos)
for filee in ["words-"+str(i)+".txt" for i in range(filesCount)]:
	shuffle(lista)
	listita=listaRepetidos+lista[:wordcount-cantidadRepetidos]
	fa=open(filee, "w")
	for word in listita:
		fa.write(word)
