#include <iostream>
#include "ConcurrentHashMap.hpp"

#define ARCHIVOS 100

using namespace std;

int main(int argc, char **argv) {
	list<string> l;
	for (int i = 0; i < ARCHIVOS; i ++) {
		l.push_back("corpus");
	}

	//p_archivos,ej5,ej6
	for (int p_archivos = 1; p_archivos <= ARCHIVOS; p_archivos++) {
	    cerr << p_archivos << ",";
	    ConcurrentHashMap::maximum(p_archivos, p_archivos, l);
	    cerr << ",";
	    ConcurrentHashMap::maximum2(p_archivos, p_archivos, l);
	    cerr << endl;
	}

	return 0;
}
