CC = g++
CCFLAGS = -g -std=c++11 -Wall
OBJ = main.o
INC = -I "./"

cppRay: $(OBJ)
	$(CC) $(OBJ) -o cppRay
	rm -f $(OBJ)

main.o:
	$(CC) -c main.cpp $(INC)

ppmutil.o:
	$(CC) -c ppmutil.cpp

clean:
	rm -f $(OBJ) cppRay scene.ppm