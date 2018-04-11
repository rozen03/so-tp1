#ifndef CONCURRENT_HASH_MAP_H__
#define CONCURRENT_HASH_MAP_H__

#include <atomic>
#include <string>
#include "ListaAtomica.hpp"
#include <iostream>
#include <mutex>
#include <pthread.h>
#include <fstream>

using namespace std;

typedef pair<string, unsigned int> par;

struct Busqueda {
	Busqueda(Lista<par>* mapa) {
		contador.store(0);
		_mapa = mapa;
	}
	Lista<par>* _mapa;
	atomic<int> contador;
	par max;
	mutex mtx_max;
};

void * buscador(void* data){
	Busqueda* busqueda = (Busqueda*) data;
	par max("", 0);
	while (true) {
		int i = busqueda->contador++;
		// cout << i << endl;
		if (i >= 26) break;
		auto it = busqueda->_mapa[i].CrearIt();
		while (it.HaySiguiente()) {
			if (it.Siguiente().second > max.second) {
				// cout << "ejejejeojojojo" << endl;
				max = it.Siguiente();
				// cout<<max.first<<","<<max.second<<endl;
			}
			it.Avanzar();
		}
	}

	busqueda->mtx_max.lock();
	if (max.second > busqueda->max.second)
		busqueda->max = max;
	busqueda->mtx_max.unlock();

	return NULL;
}

class ConcurrentHashMap {
public:

	Lista<par> tabla[26];
	mutex mutexes[26]; // No encontre mejor nombre para el array este
	int orden(string key) {
		return key.at(0) - 'a';
	}

	void print(){
		for(int i = 0; i < 26; i++) {
			auto it = tabla[i].CrearIt();
			cout << i << ": ";
			while(it.HaySiguiente()) {
				par elemento = it.Siguiente();
				cout << "(" << elemento.first << "," << elemento.second << "), ";
				it.Avanzar();
			}
			cout << endl << endl;
		}
	}

	// Constructor. Crea la tabla. La misma tendrá 26 en tradas (una por cada letra del abecedario 1 ).
	// Cada entrada consta de una lista de pares (string, entero). La función de hash será la primer letra del string.
	ConcurrentHashMap(){} //cambiar esto por ; si vamos a implementarlo a parte

	ConcurrentHashMap(const ConcurrentHashMap& mapa) {

	}

	// Si key existe, incrementa su valor, si no existe, crea el par (key, 1).
	// Se debe garantizar que sólo haya contención en caso de colisión de hash.
	// Esto es, deberá haber locking a nivel de cada elemento del array.
	void addAndInc(string key){
		int i = orden(key);
		mutexes[i].lock();
		auto it = tabla[i].CrearIt();
		while(it.HaySiguiente() && it.Siguiente().first != key) it.Avanzar();
		if (it.HaySiguiente()) // key ya esta definido
			it.Siguiente().second++;
		else // key no existe, insertamos (key, 1)
			tabla[i].push_front(make_pair(key, 1));
		mutexes[i].unlock();
	}

	// true si y solo si el par (key, x) pertenece al hash map para algún x.
	// Esta operación deberá ser wait-free.
	bool member(string key){
			int i = orden(key);
			auto it = tabla[i].CrearIt();
		while(it.HaySiguiente() && it.Siguiente().first != key) it.Avanzar();
		return it.HaySiguiente();
	}

	// Devuelve el par (k, m) tal que k es la clave con máxima cantidad de apariciones y m es ese valor.
	// No puede ser concurrente con addAndInc, sı́ con member, y tiene que ser implementada con concurrencia interna.
	// El parámetro nt indica la cantidad de threads a utilizar.
	// Los threads procesarán una fila del array. Si no tienen filas por procesar terminarán su ejecución.
	pair<string, unsigned int>maximum(unsigned int nt){
		for (size_t i = 0; i < 26; i++) {
			/* code */
			mutexes[i].lock();
		}
		Busqueda* busqueda = new Busqueda(this->tabla);
		pthread_t thread[nt];
		long long unsigned int tid;
		for (tid = 0; tid < nt; ++tid){
			pthread_create(&thread[tid], NULL, buscador,  busqueda);
		}
		void* max_thread;
		for (tid = 0; tid < nt; ++tid){
			pthread_join(thread[tid], NULL);
		}
		for (size_t i = 0; i < 26; i++) {
			/* code */
			mutexes[i].unlock();
		}
		return busqueda->max;
	}
};

ConcurrentHashMap count_words(string arch) {
	ifstream ifs(arch);
	ConcurrentHashMap mapa;
	string key;
	while (!ifs.eof()) {
		ifs >> key;
		mapa.addAndInc(key);
	}
	return mapa;
}
// ConcurrentHashMap count_words(list<string>archs){
	// int nt =archs.size();
	// pthread_t thread[nt];
// }
#endif /* CONCURRENT_HASH_MAP_H__ */
