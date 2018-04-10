#ifndef CONCURRENT_HASH_MAP_H__
#define CONCURRENT_HASH_MAP_H__

#include <atomic>
#include <string>
#include "ListaAtomica.hpp"
#include <iostream>
#include <mutex>
#include<pthread.h>

using namespace std;

class ConcurrentHashMap {
private:

	struct Par {
		Par(string key) : _key(key), _value(1) {}
		string _key;
		long unsigned int _value;
	};
 	struct Busqueda {
		Busqueda():maximo(nullptr){
			contador.store(0);
		}
		Par* maximo;
		atomic<int> contador;
	};
	Lista<Par> mapa[26];
	mutex mutexes[26]; // No encontre mejor nombre para el array este
	int orden(string key) {
		return key.at(0) - 'a';
	}


public:

	void print(){
		for(int i = 0; i < 26; i++) {
			auto it = mapa[i].CrearIt();
			cout << i << ": ";
			while(it.HaySiguiente()) {
				Par elemento = it.Siguiente();
				cout << elemento._key << "," << elemento._value << ",";
				it.Avanzar();
			}
			cout << endl;
		}
	}

	// Constructor. Crea la tabla. La misma tendrá 26 en tradas (una por cada letra del abecedario 1 ).
	// Cada entrada consta de una lista de pares (string, entero). La función de hash será la primer letra del string.
	ConcurrentHashMap(){} //cambiar esto por ; si vamos a implementarlo a parte

	// Si key existe, incrementa su valor, si no existe, crea el par (key, 1).
	// Se debe garantizar que sólo haya contención en caso de colisión de hash.
	// Esto es, deberá haber locking a nivel de cada elemento del array.
	void addAndInc(string key){
		int i = orden(key);
		mutexes[i].lock();
		auto it = mapa[i].CrearIt();
		while(it.HaySiguiente() && it.Siguiente()._key != key) it.Avanzar();
		if (it.HaySiguiente()) // key ya esta definido
			it.Siguiente()._value++;
		else // key no existe, insertamos (key, 1)
			mapa[i].push_front(Par(key));
		mutexes[i].unlock();
	}

	// true si y solo si el par (key, x) pertenece al hash map para algún x.
	// Esta operación deberá ser wait-free.
	bool member(string key){
		int i = orden(key);
		auto it = mapa[i].CrearIt();
		while(it.HaySiguiente() && it.Siguiente()._key != key) it.Avanzar();
		return it.HaySiguiente();
	}

	void * buscador(void* data){
		Busqueda* busqueda = (Busqueda*) data;
		int i = busqueda->contador++;
		cout<<i<<","<<busqueda->contador.load()<<endl;
		while(i<26){
			cout<<i<<endl;
			i = busqueda->contador++;
		}

		return NULL;
	}
typedef void * (*THREADFUNCPTR)(void *);
	// Devuelve el par (k, m) tal que k es la clave con máxima cantidad de apariciones y m es ese valor.
	// No puede ser concurrente con addAndInc, sı́ con member, y tiene que ser implementada con concurrencia interna.
	// El parámetro nt indica la cantidad de threads a utilizar.
	// Los threads procesarán una fila del array. Si no tienen filas por procesar terminarán su ejecución.
	pair<string, unsigned int>maximum(unsigned int nt){
		Busqueda* busqueda = new Busqueda();
		busqueda->contador.store(0);
		cout<<"contador"<<busqueda->contador++<<endl;
		pthread_t thread[nt];
		long long unsigned int tid;
		for (tid = 0; tid < nt; ++tid){
			pthread_create(&thread[tid], NULL,(THREADFUNCPTR)&ConcurrentHashMap::buscador,  busqueda);
		}
		for (tid = 0; tid < nt; ++tid){
			pthread_join(thread[tid], NULL);
		}
		return make_pair ("hola",20);
	}

};

#endif /* CONCURRENT_HASH_MAP_H__ */
