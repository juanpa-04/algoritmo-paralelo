CC= g++
CPPFLAGS= -g -Wall -fopenmp

SRCDIR=src
BUILDDIR=build
TARGET=mergesort

bin/$(TARGET): $(BUILDDIR)/$(TARGET).o
	$(CC) $(CPPFLAGS) -o bin/$(TARGET) $(BUILDDIR)/$(TARGET).o

$(BUILDDIR)/$(TARGET).o: $(SRCDIR)/$(TARGET).cpp | object
	$(CC) $(CPPFLAGS) -c $(SRCDIR)/$(TARGET).cpp -o $(BUILDDIR)/$(TARGET).o

object:
	mkdir -p bin

run: bin/$(TARGET)
	./bin/$(TARGET)

.PHONY: clean
clean: 
	rm -rf $(BUILDDIR)/*.o
	rm -rf bin/$(TARGET)