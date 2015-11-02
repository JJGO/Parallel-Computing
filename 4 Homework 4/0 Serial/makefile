 #####################################################################
 # Example MPI C Makefile.
 #####################################################################
 
SRC = serial
CC = g++
# ARGS = 10

# HEADERS = ${SRC}.h
OBJECTS = ${SRC}.o

default: ${SRC}
# %.o: %.c $(HEADERS)
%.o: %.cpp
	${CC} -c $< -o $@

${SRC}: $(OBJECTS)
	${CC} $(OBJECTS) -o $@

clean:
	-rm -f $(OBJECTS)
	-rm -f ${SRC}

run: ${SRC}
	./${SRC}
	# ./${SRC} ${ARGS}


