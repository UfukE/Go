CXX = g++
CXXFLAGS = -Wall -std=c++11
EXEC_FILES = test

%.o: %.cpp
	${CXX} -o $@ -c $< ${CXXFLAGS}

test: test.o Go.o
	$(CXX) -o $@ $^ $(LDFLAGS)

clean:
	rm -f *.o $(EXEC_FILES)

run:
	./test.exe

