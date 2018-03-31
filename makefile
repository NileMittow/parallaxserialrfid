testexecutable: test.cpp
	g++ -g -std=c++11 -o testexecutable test.cpp CoworkerLookup.h -ljsoncpp -lsqlite3

doorservice: doorservice.cpp DoorController.o color.o
	g++ -g -std=c++11 -o doorservice doorservice.cpp CoworkerLookup.h DoorController.o color.o -ljsoncpp -lsqlite3 -lmraa -lupm-i2clcd

DoorController.o: color.o DoorController.cpp DoorController.h
	g++ -g -std=c++11 -o DoorController.o DoorController.cpp color.o -lmraa -lupm-i2clcd -I/usr/include/upm -c

color.o: color.cpp color.h
	g++ -g -std=c++11 -o color.o color.cpp -c

test: clean testexecutable
	./testexecutable 2> /dev/null

clean:
	rm -f *.o *.~ doorservice testexecutable
