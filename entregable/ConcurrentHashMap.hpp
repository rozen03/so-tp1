#ifndef CONCURRENT_HASH_MAP_H__
#define CONCURRENT_HASH_MAP_H__

#include <atomic>
#include <string>
#include "ListaAtomica.hpp"
#include <iostream>
#include <mutex>
using namespace std;
class ConcurrentHashMap {
private:
	struct Nodo {
		Nodo(string key) : _key(key), _value(0) {}
		string _key;
		long unsigned int _value;
	};
 Lista<Nodo*> mapa [26];
 mutex mutexes[26]; // No encontre mejor nombre para el array este
int orden(string key ){
	return key.at(0) - 'a';
}
public:
	void print(){
		for(int i=0; i<26;i++){
			auto it =mapa[i].CrearIt();
			cout<<i<<": ";
			while(it.HaySiguiente()){
				auto cosa=it.Siguiente();
				cout<<cosa->_key<<","<<cosa->_value<<",";
				it.Avanzar();
			}
			cout<<endl;
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
		auto it =mapa[i].CrearIt();
		bool existe=false;
		while(it.HaySiguiente() && !existe){
			auto nodo = it.Siguiente();
			if (nodo->_key==key){
				nodo->_value++;
				existe=true;
			}else{
				it.Avanzar();
			}
		}
		if (!existe){
			mapa[i].push_front(new Nodo(key));
		}
		mutexes[i].unlock();
	}

	// true si y solo si el par (key, x) pertenece al hash map para algún x.
	// Esta operación deberá ser wait-free.
	bool member(string key){
		int i = orden(key);
		auto it =mapa[i].CrearIt();
		bool existe=false;
		while(it.HaySiguiente() && !existe){
			auto nodo = it.Siguiente();
			if (nodo->_key==key){
				existe=true;
			}else{
				it.Avanzar();
			}
		}
		return existe;
	}

	// Devuelve el par (k, m) tal que k es la clave con máxima cantidad de apariciones y m es ese valor.
	// No puede ser concurrente con addAndInc, sı́ con member, y tiene que ser implementada con concurrencia interna.
	// El parámetro nt indica la cantidad de threads a utilizar.
	// Los threads procesarán una fila del array. Si no tienen filas por procesar terminarán su ejecución.
	pair<string, unsigned int>maximum(unsigned int nt);

};

#endif /* CONCURRENT_HASH_MAP_H__ */
