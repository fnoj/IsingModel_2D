CXX = g++
CFLAGS = `root-config --cflags`
LFLAGS = `root-config --glibs`

TARGET = Ising2D.exe

all: $(TARGET)

Ising2D.exe : Ising.o Dict.o Application.o
	$(CXX) -o $@ $^ $(LFLAGS) 

%.o: %.cxx
	$(CXX) $(CFLAGS) -c $<

Dict.cxx : Ising.h
	@rootcint -f $@ -c $^

clean:
	@rm *.o Dict.h Dict.cxx
