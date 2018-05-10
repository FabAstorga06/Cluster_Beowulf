IDIR = ./include
SDIR = ./src
ODIR = ./obj

CC = gcc
MPICC = mpicc
CXX = g++
MPIXX = mpic++
CPPFLAGS = -std=c++11 -O3 -I$(IDIR) `libpng-config --cflags`
PNG = `libpng-config --ldflags`

EXEC = cluster

_DEPS = constants.h image_utils.hpp gaussian_blur.hpp
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = $(EXEC).o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))


$(ODIR)/%.o: $(SDIR)/%.cpp $(DEPS)
	$(MPIXX) -c -o $@ $< $(CPPFLAGS) 

$(EXEC): $(OBJ)
	$(MPIXX) -o $@ $^ $(CPPFLAGS) $(PNG)

.PHONY: clean
clean:
	rm -f $(ODIR)/*.o $(EXEC)
