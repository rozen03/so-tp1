#include <iostream>
#include "../ConcurrentHashMap.hpp"

#define ARCHIVOS 100

using namespace std;

// argv[1] = 1 -> archivos distintos
// argv[1] = 2 -> archivos iguales
int main(int argc, char **argv) {
	list<string> l;
	for (int i = 0; i < ARCHIVOS; i ++) {
		if (stoi(argv[1]) == 1) {
			l.push_back("words/words-"+to_string(i)+".txt");
		} else {
			l.push_back("words/words-0.txt");
		}
	}

	//p_archivos,ej5-load,ej5-merge,ej6
	for (int p_archivos = 1; p_archivos <= ARCHIVOS; p_archivos++) {
	    cerr << p_archivos << ",";
	    ConcurrentHashMap::maximum(p_archivos, p_archivos, l);
	    cerr << ",";
	    ConcurrentHashMap::maximum2(p_archivos, p_archivos, l);
	    cerr << endl;
	}

	return 0;
}
