#include <iostream>
#include "ListaAtomica.hpp"
#include "ConcurrentHashMap.hpp"
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
int main(void) {
testaddInc();

}
