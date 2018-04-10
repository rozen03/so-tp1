#include <iostream>
#include "ListaAtomica.hpp"
#include "ConcurrentHashMap.hpp"
#include<pthread.h>
#include<stdio.h>
#include <atomic>
using namespace std;
void testLista(){
  Lista<int> lista;
	lista.push_front(5);
	cout<<lista.front()<<endl;
	lista.push_front(4);
	cout<<lista.front()<<endl;
	lista.push_front(3);
	cout<<lista.front()<<endl;
	lista.push_front(2);
	cout<<lista.front()<<endl;
	lista.push_front(1);
	cout<<lista.front()<<endl;
	lista.push_front(0);
	cout<<lista.front()<<endl;
	for(int i =0; i<6; i++){
		cout<<lista.iesimo(i)<<endl;
	}
}
void testaddInc(){
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
  cout<<"empecé"<<endl;
  ConcurrentHashMap * mapa = (ConcurrentHashMap *)data;
	mapa->addAndInc("hola");
  cout<<"terminé"<<endl;
	return NULL;
}
void testaddincMultithread(){
  ConcurrentHashMap* mapa = new ConcurrentHashMap();
  long long unsigned int CANT_THREADS =9999;
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
void atomictest(){
atomic<int> a;
a.store(0);
int b = ++a;
cout<<b<<endl;
}
void testMaximum(){
	ConcurrentHashMap mapa;
  mapa.addAndInc("hola");
	mapa.maximum(1);
}
int main(void) {
	testMaximum();

}
