 #####################################################################
 # Example MPI C Makefile.
 #####################################################################
 
SRC = fsp
CC = g++-5.2.0
# ARGS = 10

# HEADERS = ${SRC}.h
OBJECTS = ${SRC}.o

default: ${SRC}
# %.o: %.c $(HEADERS)
%.o: %.cpp
	${CC} -c $< -o $@ -fopenmp

${SRC}: $(OBJECTS)
	${CC} $(OBJECTS) -o3 -o $@ -fopenmp

clean:
	-rm -f $(OBJECTS)
	-rm -f ${SRC}

run: ${SRC}
	export OMP_NUM_THREADS=2
	./${SRC}
	# ./${SRC} ${ARGS}