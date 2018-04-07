#include <iostream>
#include "ListaAtomica.hpp"
using namespace std;

int main(void) {
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
