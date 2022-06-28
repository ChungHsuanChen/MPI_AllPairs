SHELL := bash
CC :=  mpicc 
SOURCE := mpi.c
INPUT_FILE_SOURCE:=input_file_generation.c
CFLAG := -lm -o 
TARGET := ./mpi
MPIRUN := mpirun -n


all: 
	$(CC) $(SOURCE) $(CFLAG) $(TARGET)
input_generation:
	$(CC) $(INPUT_FILE_SOURCE) $(CFLAG) ./input_file_generation
	./input_file_generation
run: all
	for CORE in 1 2 3 4 ; do \
		$(MPIRUN) $$CORE $(TARGET);\
	done
clean:
	rm -rf $(TARGET)
