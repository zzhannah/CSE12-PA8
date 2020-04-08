# To use, type:
#
#       make Calc               # This will make executable calc
# or
#       make Driver             # This will make executable driver
# or
#       make clean              # This will safely remove old stuff

Driver:	Driver.o 
	g++ -o Driver -Wall -pedantic -g Driver.o 
Driver.o:	Driver.c Tree.c Tree.h
	g++ -Wall -pedantic -g -c Driver.c
valgrind_Driver: Driver
	valgrind --leak-check=full --read-var-info=yes \
	    --show-reachable=yes ./Driver $(DEBUG)
clean:
	rm -f *.o Driver
new:
	make clean
	make
