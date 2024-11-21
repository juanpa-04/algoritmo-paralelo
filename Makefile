CC= g++
CXXFLAGS= -Wall -fopenmp

SRCDIR=src
BUILDDIR=build
TARGET=mergesort
THREADS=4

all: bin/$(TARGET_OMP) bin/$(TARGET)

bin/$(TARGET): $(BUILDDIR)/$(TARGET).o
	$(CC) $(CXXFLAGS) -o bin/$(TARGET) $(BUILDDIR)/$(TARGET).o

$(BUILDDIR)/$(TARGET).o: $(SRCDIR)/$(TARGET).cpp | object
	$(CC) $(CXXFLAGS) -c $(SRCDIR)/$(TARGET).cpp -o $(BUILDDIR)/$(TARGET).o

object:
	mkdir -p bin

run: bin/$(TARGET)
	@echo "******MergeSort sin OMP******"
	OMP_NUM_THREADS=1 ./bin/$(TARGET)
	@echo "******MergeSort con OMP******"
	OMP_NUM_THREADS=$(THREADS) ./bin/$(TARGET)

.PHONY: clean
clean: 
	rm -rf $(BUILDDIR)/*.o
	rm -rf bin/*