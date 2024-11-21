CC= g++
CXXFLAGS= -Wall -fopenmp -02

SRCDIR=src
BUILDDIR=build
TARGET=analyzer

bin/$(TARGET): $(BUILDDIR)/$(TARGET).o
	$(CC) $(CXXFLAGS) -o bin/$(TARGET) $(BUILDDIR)/$(TARGET).o

$(BUILDDIR)/$(TARGET).o: $(SRCDIR)/$(TARGET).cpp | object
	$(CC) $(CXXFLAGS) -c $(SRCDIR)/$(TARGET).cpp -o $(BUILDDIR)/$(TARGET).o

object:
	mkdir -p bin
	mkdir -p build

run: bin/$(TARGET)
	./bin/$(TARGET)

.PHONY: clean
clean: 
	rm -rf $(BUILDDIR)/*.o
	rm -rf bin/*