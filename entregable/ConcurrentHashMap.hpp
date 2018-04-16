#ifndef CONCURRENT_HASH_MAP_H__
#define CONCURRENT_HASH_MAP_H__

#include <atomic>
#include <string>
#include "ListaAtomica.hpp"
#include <iostream>
#include <mutex>
#include <pthread.h>
#include <fstream>
#include <list>
using namespace std;

typedef pair<string, unsigned int> par;

class ConcurrentHashMap {
public:

	Lista<par>* tabla[26];
	mutex mutexes[26];
	int orden(string key) {
		return key.at(0) - 'a';
	}

	void print(){
		for(int i = 0; i < 26; i++) {
			auto it = tabla[i]->CrearIt();
			cout << i << ": ";
			while(it.HaySiguiente()) {
				par elemento = it.Siguiente();
				cout << "(" << elemento.first << "," << elemento.second << "), ";
				it.Avanzar();
			}
			cout << endl;
		}
	}

	// Constructor. Crea la tabla. La misma tendrá 26 en tradas (una por cada letra del abecedario 1 ).
	// Cada entrada consta de una lista de pares (string, entero). La función de hash será la primer letra del string.
	ConcurrentHashMap() {
		for (int i = 0; i < 26; i++) {
			tabla[i] = new Lista<par>();
		}
	}

	ConcurrentHashMap(const ConcurrentHashMap& mapa) {
		for (int i = 0; i < 26; i++) {
			tabla[i] = new Lista<par>(*mapa.tabla[i]);
		}
	}

	ConcurrentHashMap operator=(const ConcurrentHashMap& mapa) {
		for (int i = 0; i < 26; i++) {
			delete tabla[i];
			tabla[i] = mapa.tabla[i];
		}
		return *this;
	}

	// Si key existe, incrementa su valor, si no existe, crea el par (key, 1).
	// Se debe garantizar que sólo haya contención en caso de colisión de hash.
	// Esto es, deberá haber locking a nivel de cada elemento del array.
	void addAndInc(string key) {
		int i = orden(key);
		mutexes[i].lock();
		auto it = tabla[i]->CrearIt();
		while(it.HaySiguiente() && it.Siguiente().first != key) it.Avanzar();
		if (it.HaySiguiente()) // key ya esta definido
			it.Siguiente().second++;
		else // key no existe, insertamos (key, 1)
			tabla[i]->push_front(make_pair(key, 1));
		mutexes[i].unlock();
	}

	// true si y solo si el par (key, x) pertenece al hash map para algún x.
	// Esta operación deberá ser wait-free.
	bool member(string key) {
		int i = orden(key);
		auto it = tabla[i]->CrearIt();
		while(it.HaySiguiente() && it.Siguiente().first != key) it.Avanzar();
		return it.HaySiguiente();
	}

	struct Busqueda {
		ConcurrentHashMap* mapa;
		atomic<int> contador;
		par max;
		mutex mtx_max;
	};
	static void * buscador(void* data) {
		Busqueda* busqueda = (Busqueda*) data;
		par max("", 0);
		while (true) {
			int i = busqueda->contador++;
			if (i >= 26) break;
			auto it = busqueda->mapa->tabla[i]->CrearIt();
			while (it.HaySiguiente()) {
				if (it.Siguiente().second > max.second) {
					max = it.Siguiente();
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
	// Devuelve el par (k, m) tal que k es la clave con máxima cantidad de apariciones y m es ese valor.
	// No puede ser concurrente con addAndInc, sı́ con member, y tiene que ser implementada con concurrencia interna.
	// El parámetro nt indica la cantidad de threads a utilizar.
	// Los threads procesarán una fila del array. Si no tienen filas por procesar terminarán su ejecución.
	pair<string, unsigned int>maximum(unsigned int nt){
		for (size_t i = 0; i < 26; i++) {
			mutexes[i].lock();
		}
		Busqueda busqueda;
		busqueda.mapa = this;
		busqueda.contador.store(0);
		busqueda.max = make_pair("", 0);
		pthread_t thread[nt];
		long long unsigned int tid;
		for (tid = 0; tid < nt; ++tid) {
			pthread_create(&thread[tid], NULL, buscador,  &busqueda);
		}
		for (tid = 0; tid < nt; ++tid) {
			pthread_join(thread[tid], NULL);
		}
		for (size_t i = 0; i < 26; i++) {
			mutexes[i].unlock();
		}
		return busqueda.max;
	}

	static ConcurrentHashMap count_words(string arch);
	static ConcurrentHashMap count_words(list<string>archs);
	static ConcurrentHashMap count_words(unsigned int n,list<string>archs);
	static pair<string, unsigned int> maximum(unsigned int p_archivos, unsigned int p_maximos, list<string>archs);
	static pair<string, unsigned int> maximum2(unsigned int p_archivos, unsigned int p_maximos, list<string>archs);
};

// Inserta en el mapa las palabras que esten en el archivo
void meterEnMapa(ConcurrentHashMap* mapa,string arch) {
	ifstream ifs(arch);
	string key;
	while(ifs >> key){
		mapa->addAndInc(key);
	}
}

ConcurrentHashMap ConcurrentHashMap::count_words(string arch) {
	ConcurrentHashMap mapa;
	meterEnMapa(&mapa, arch);
	return mapa;
}


struct Wrapper_count_words1 {
	ConcurrentHashMap* mapa;
	string arch;
};
// Funcion thread para count_words(list<string>archs)
void * thread_count_words1(void * data) {
	Wrapper_count_words1* wrap = (Wrapper_count_words1*)data;
	meterEnMapa(wrap->mapa, wrap->arch);
	return NULL;
}
ConcurrentHashMap ConcurrentHashMap::count_words(list<string>archs) {
	int nt = archs.size();
	pthread_t thread[nt];
	ConcurrentHashMap mapa;
	int tid = 0;
	Wrapper_count_words1 wrap[nt];
	for (string arch : archs) {
		wrap[tid].arch = arch;
		wrap[tid].mapa = &mapa;
		pthread_create(&thread[tid], NULL, thread_count_words1,  &wrap[tid]);
		tid++;
	}
	for (tid = 0; tid < nt; ++tid){
		pthread_join(thread[tid], NULL);
	}
	return mapa;
}

struct Wrapper_count_words2 {
	ConcurrentHashMap* mapa;
	mutex *mutexLista;
	list<string>::iterator it;
	list<string>::iterator ends;
};
// Funcion thread para count_words(unsigned int n,list<string>archs)
void * thread_count_words2(void * data) {
	Wrapper_count_words2* wrap = (Wrapper_count_words2*)data;
	string arch;
	while(true){
		wrap->mutexLista->lock();
		if (wrap->it == wrap->ends) {
			wrap->mutexLista->unlock();
			return NULL;
		}
		arch = *wrap->it;
		wrap->it++;
		wrap->mutexLista->unlock();
		meterEnMapa(wrap->mapa, arch);
	}
	return NULL;
}
ConcurrentHashMap ConcurrentHashMap::count_words(unsigned int n,list<string>archs) {
	pthread_t thread[n];
	ConcurrentHashMap mapa;
	int tid = 0;
	Wrapper_count_words2 wrap;
	wrap.mapa = &mapa;
	wrap.mutexLista = new mutex();
	wrap.it = archs.begin();
	wrap.ends = archs.end();
	for (tid = 0; tid < n; ++tid){
		pthread_create(&thread[tid], NULL, thread_count_words2,  &wrap);
	}
	for (tid = 0; tid < n; ++tid){
		pthread_join(thread[tid], NULL);
	}
	return mapa;
}

struct Wrapper_maximum {
	Lista<ConcurrentHashMap>* mapas;
	mutex *mutexLista;
	list<string>::iterator it;
	list<string>::iterator ends;
};
// Funcion thread para maximum(unsigned int p_archivos, unsigned int p_maximos, list<string>archs)
void * thread_maximum(void * data) {
	Wrapper_maximum* wrap = (Wrapper_maximum*)data;
	ConcurrentHashMap mapa;
	string arch;
	while(true){
		wrap->mutexLista->lock();
		if (wrap->it == wrap->ends) {
			wrap->mutexLista->unlock();
			break;
		}
		arch = *wrap->it;
		wrap->it++;
		wrap->mutexLista->unlock();
		meterEnMapa(&mapa, arch);
	}
	wrap->mapas->push_front(mapa);
	return NULL;
}
pair<string, unsigned int> ConcurrentHashMap::maximum(unsigned int p_archivos, unsigned int p_maximos, list<string>archs) {
	pthread_t thread[p_archivos];
	Lista<ConcurrentHashMap>* mapas = new Lista<ConcurrentHashMap>;
	int tid = 0;
	Wrapper_maximum wrap;
	wrap.mapas = mapas;
	wrap.mutexLista = new mutex();
	wrap.it = archs.begin();
	wrap.ends = archs.end();
	for (tid = 0; tid < p_archivos; ++tid){
		pthread_create(&thread[tid], NULL, thread_maximum,  &wrap);
	}
	for (tid = 0; tid < p_archivos; ++tid){
		pthread_join(thread[tid], NULL);
	}

	// Merge de los mapas
	ConcurrentHashMap mapa;
	for (auto it_1 = mapas->CrearIt(); it_1.HaySiguiente(); it_1.Avanzar()) {
		for (int i = 0; i < 26; i++) {
			for (auto it_2 = it_1.Siguiente().tabla[i]->CrearIt(); it_2.HaySiguiente(); it_2.Avanzar()) {
				for (int j = 0; j < it_2.Siguiente().second; j++) {
					mapa.addAndInc(it_2.Siguiente().first);
				}
			}
		}
	}

	return mapa.maximum(p_maximos);
}

pair<string, unsigned int> ConcurrentHashMap::maximum2(unsigned int p_archivos, unsigned int p_maximos, list<string>archs) {
	return count_words(p_archivos, archs).maximum(p_maximos);
}


#endif /* CONCURRENT_HASH_MAP_H__ */
