IDIR = ./include
SDIR = ./src
ODIR = ./obj

CC = gcc
MPI = mpicc
CFLAGS = -I$(IDIR)

EXEC = cluster

_DEPS = 
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = $(EXEC).o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

LIBS = 

$(ODIR)/%.o: $(SDIR)/%.c $(DEPS)
	$(MPI) -c -o $@ $< $(CFLAGS)

$(EXEC): $(OBJ)
	$(MPI) -o $@ $^ $(CFLAGS) $(LIBS)

.PHONY: clean
clean:
	rm -f $(ODIR)/*.o $(EXEC)
