 #####################################################################
 # Example MPI C Makefile.
 #####################################################################
 
SRC = serial
CC = g++
ARGS = 10

# HEADERS = ${SRC}.h
OBJECTS = ${SRC}.o

default: ${SRC}
# %.o: %.c $(HEADERS)
%.o: %.c
	${CC} -c $< -o $@

${SRC}: $(OBJECTS)
	${CC} $(OBJECTS) -o $@

clean:
	-rm -f $(OBJECTS)
	-rm -f ${SRC}

run: ${SRC}
	./${SRC} ${ARGS}

test: ${SRC}
	./${SRC} 1000
	./${SRC} 2000

