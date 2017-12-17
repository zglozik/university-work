
CXX = g++
CXXFLAGS = -Wall -g
LDFLAGS =

SOURCES = main.cc MessageQueue.cc Object.cc Owner.cc Animals.cc Equerry.cc \
	Buyer.cc random.cc

OBJECTS = $(SOURCES:.cc=.o)

af: $(OBJECTS)
	$(CXX) -o $@ $^ $(LDFLAGS)

depend:
	makedepend -Y $(SOURCES)

clean:
	rm *~ *.o af core

backup:
	(cd ..; tar czf af.tar.gz af/*.cc af/*.h af/Makefile)

 # DO NOT DELETE

main.o: af.h MessageQueue.h generic.h Object.h Equerry.h Animals.h Owner.h
main.o: Buyer.h
MessageQueue.o: MessageQueue.h generic.h Object.h
Object.o: Object.h
Animals.o: Animals.h Object.h Equerry.h random.h
Equerry.o: Equerry.h Object.h Animals.h
Buyer.o: Buyer.h Object.h Animals.h Equerry.h af.h MessageQueue.h generic.h
Buyer.o: Owner.h
# DO NOT DELETE

main.o: af.h MessageQueue.h generic.h Object.h Equerry.h Animals.h random.h
main.o: Owner.h Buyer.h
MessageQueue.o: MessageQueue.h generic.h Object.h
Object.o: Object.h
Owner.o: Owner.h Object.h Animals.h random.h Buyer.h af.h MessageQueue.h
Owner.o: generic.h Equerry.h
Animals.o: Animals.h Object.h random.h Equerry.h generic.h af.h
Animals.o: MessageQueue.h Owner.h Buyer.h
Equerry.o: Equerry.h Object.h Animals.h random.h generic.h af.h
Equerry.o: MessageQueue.h Owner.h Buyer.h
Buyer.o: Buyer.h Object.h Animals.h random.h Owner.h af.h MessageQueue.h
Buyer.o: generic.h Equerry.h
random.o: random.h
