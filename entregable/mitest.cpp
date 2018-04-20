#include <iostream>
#include "ListaAtomica.hpp"
#include "ConcurrentHashMap.hpp"
#include <pthread.h>
#include <stdio.h>
#include <atomic>
#include <list>

using namespace std;

void testLista(){
	Lista<int> lista;
	lista.push_front(5);
	cout << lista.front() << endl;
	lista.push_front(4);
	cout << lista.front() << endl;
	lista.push_front(3);
	cout << lista.front() << endl;
	lista.push_front(2);
	cout << lista.front() << endl;
	lista.push_front(1);
	cout << lista.front() << endl;
	lista.push_front(0);
	cout << lista.front() << endl;
	for(int i = 0; i < 6; i++){
		cout << lista.iesimo(i) << endl;
	}
}

void testAddInc(){
	ConcurrentHashMap mapa;
	mapa.addAndInc("hola");
	mapa.addAndInc("hola");
	mapa.addAndInc("hola");
	mapa.addAndInc("hola");
	mapa.addAndInc("hola");
	mapa.addAndInc("chau");
	mapa.addAndInc("hilde");
	mapa.addAndInc("hola");
	mapa.addAndInc("hilde");
	mapa.addAndInc("chau");
	mapa.print();
}

void * agregarHola(void* data){
	cout << "empecé" << endl;
	ConcurrentHashMap * mapa = (ConcurrentHashMap *)data;
	mapa->addAndInc("hola");
	cout << "terminé" << endl;
	return NULL;
}
void testAddIncMultithread(){
	ConcurrentHashMap* mapa = new ConcurrentHashMap();
	long long unsigned int CANT_THREADS = 1000;
	pthread_t thread[CANT_THREADS];
	long long unsigned int tid;
	for (tid = 0; tid < CANT_THREADS; ++tid){
		pthread_create(&thread[tid], NULL, agregarHola,  mapa);
	}
	for (tid = 0; tid < CANT_THREADS; ++tid){
		pthread_join(thread[tid], NULL);
	}
	mapa->print();
}

void testMaximum(){
	ConcurrentHashMap mapa;
	mapa.addAndInc("hola");
	mapa.addAndInc("hola");
	mapa.addAndInc("chau");
	mapa.addAndInc("chau");
	mapa.addAndInc("hola");
	mapa.print();
	auto res = mapa.maximum(32);
	cout << res.first << " " << res.second << endl;
}

void testCountWords() {
	ConcurrentHashMap h(ConcurrentHashMap::count_words("corpus"));
	h.print();
}


int main(void) {
	int opcion = 0;
	while (opcion != 5) {
		cout << endl;
		cout << "OPCIONES" << endl;
		cout << "0 - testLista" << endl;
		cout << "1 - testAddInc" << endl;
		cout << "2 - testAddIncMultithread" << endl;
		cout << "3 - testMaximum" << endl;
		cout << "4 - testCountWords" << endl;
		cout << "5 - salir" << endl;
		cout << "Opción: ";
		cin  >> opcion;
		cout << endl << endl;
		switch (opcion) {
			case 0:
				testLista();
				break;
			case 1:
				testAddInc();
				break;
			case 2:
				testAddIncMultithread();
				break;
			case 3:
				testMaximum();
				break;
			case 4:
				testCountWords();
				break;
			case 5:
				break;
			case 42:
				system("telnet towel.blinkenlights.nl");
				break;
			default:
				cout << "No che, no tengo nada puesto ahi, nada puesto." << endl;
		}
	}
	return 0;
}
