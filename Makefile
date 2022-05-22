SHELL := bash
CC :=  mpicc 
SOURCE := mpi.c
CFLAG := -lm -o 
TARGET := ./mpi
MPIRUN := mpirun -n

CORE:=4
all: 
	$(CC) $(SOURCE) $(CFLAG) $(TARGET)

run: all
	for CORE in 1 2 3 4 ; do \
		$(MPIRUN) $(CORE) $(TARGET);\
	done
