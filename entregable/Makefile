.POSIX:
.SUFFIXES:
.SUFFIXES: .o .cpp .hpp

CXX = g++
CXXFLAGS = -std=c++11
LDLIBS = -lpthread -lrt

.cpp.o:
	$(CXX) $(CXXFLAGS) -c $<

BIN = test-2 test-3 test-5 exp/exp mitest

all: $(BIN)

$(BIN): ListaAtomica.hpp ConcurrentHashMap.hpp

test-2: test-2.cpp
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ test-2.cpp $(LDLIBS)

test-2-run: test-2
	awk -f corpus.awk corpus | sort >corpus-post
	./test-2 | sort | diff -u - corpus-post
	rm -f corpus-post

test-3:  test-3.cpp
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ test-3.cpp $(LDLIBS)

test-3-run: test-3
	awk -f corpus.awk corpus | sort >corpus-post
	for i in 0 1 2 3 4; do sed -n "$$((i * 500 + 1)),$$(((i + 1) * 500))p" corpus >corpus-"$$i"; done
	for i in 0 1 2 3 4; do ./test-3 $$((i + 1)) | sort | diff -u - corpus-post; done
	rm -f corpus-post corpus-[0-4]

test-5: test-5.cpp
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ test-5.cpp $(LDLIBS)

test-5-run: test-5
	awk -f corpus.awk corpus | sort -nk 2 | tail -n 1 >corpus-max
	cat corpus-max
	for i in 0 1 2 3 4; do sed -n "$$((i * 500 + 1)),$$(((i + 1) * 500))p" corpus >corpus-"$$i"; done
	for i in 0 1 2 3 4; do for j in 0 1 2 3 4; do \
		./test-5 $$((i + 1)) $$((j + 1)) | diff -u - corpus-max; \
	done; done
	rm -f corpus-max corpus-[0-4]

exp/exp: exp/exp.cpp
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ exp/exp.cpp $(LDLIBS)

mitest: mitest.cpp
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ mitest.cpp  $(LDLIBS)

clean:
	rm -f $(BIN)
	rm -f corpus-*
