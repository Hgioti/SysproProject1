objects = main.o SearchSites.o queue.o

sniffer : $(objects)
	cc -o sniffer $(objects)

main.o: main.c myLib.h
	gcc -c -g main.c

SearchSites.o: SearchSites.c myLib.h
	gcc -c -g SearchSites.c

queue.o: queue.c myLib.h
	gcc -c -g queue.c

clean:
	rm -rf *.o sniffer
