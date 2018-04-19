#!/usr/bin/python3
# -*- coding: utf-8 -*-
# import matplotlib.pyplot as plt
# import subprocess
from random import shuffle
def archivosRandom(filesCount,wordcount):
	f = open("words.txt")
	lista = [a for a in f]
	for filee in ["words-"+str(i)+".txt" for i in range(filesCount)]:
		shuffle(lista)
		listita=lista[:wordcount]
		fa=open(filee, "w")
		for word in listita:
			fa.write(word)

archivosRandom(100, 500)
quit()
# plt.plot([1,2,3,4])
# plt.ylabel('some numbers')
# subprocess.call()
# filesCount=2
# rep=2000
# from subprocess import Popen, PIPE
# lista=[]
# suma=0
# for i in range(rep):
	# s = Popen("./mitest", stdin=PIPE, stdout=PIPE, stderr=PIPE)
	# input = bytes(filesCount) # notice the input data are actually bytes and not text
	# output, errs = s.communicate(input)
	# output=output.decode("utf-8").rstrip()
	# suma+=int(output)
	# lista.append(int(output))
	# if (i%100==0):
		# print(i)
# print(lista)
# print(suma/rep)
# plt.plot(list(range(rep)),lista)
# plt.plot(list(range(rep)),[suma/rep for i in range(rep)])
# plt.show()
#
