parallaxserialrfid-example: parallaxSerialRFID-example.cpp parallaxSerialRFID.o
  g++ -g -std=c++11 -o parallaxSerialRFID-example.cpp parallaxSerialRFID.o

parallaxSerialRFID.o: parallaxSerialRFID.h parallaxSerialRFID.cpp
  g++ -g -std=c++11 -o parallaxSerialRFID.o parallaxSerialRFID.cpp -lmraa -c

remake: clean parallaxSerialRFID-example
  ./parallaxSerialRFID-example

clean:
  rm -f *.o *.~ parallaxSerialRFID-example
