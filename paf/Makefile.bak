
CXXFLAGS = -Wall -g
LDFLAGS = -L$(PVM_ROOT)/lib/$(PVM_ARCH) -lpvm3
CPPFLAGS = -I$(PVM_ROOT)/include

SOURCES = main.cc Pig.cc Chicken.cc Buyer.cc Equerry.cc Owner.cc

OBJECTS = $(SOURCES:.cc=.o)

%: %.o
	$(CXX) -o $@ $^ $(LDFLAGS)
	ln -f $@ $(HOME)/pvm3/bin/$(PVM_ARCH)

all: main Pig Chicken Buyer Equerry Owner

main: main.o MessageQueue.o Object.o global.o

Pig: Pig.o Animals.o MessageQueue.o Object.o global.o random.o

Chicken: Chicken.o Animals.o MessageQueue.o Object.o global.o random.o

Buyer: Buyer.o MessageQueue.o Object.o global.o random.o

Equerry: Equerry.o MessageQueue.o Object.o global.o

Owner: Owner.o MessageQueue.o Object.o global.o random.o

backup:
	cd ..; tar cf sb.tar sb/*.cc sb/*.h sb/Makefile

clean:
	rm *.o sb *~ \#* core

depend:
	makedepend -Y $(SOURCES)

# DO NOT DELETE

main.o: generic.h MessageQueue.h Object.h global.h
Pig.o: Pig.h Object.h global.h MessageQueue.h Animals.h random.h
Chicken.o: Chicken.h Object.h global.h MessageQueue.h Animals.h random.h
Buyer.o: Buyer.h Object.h global.h MessageQueue.h random.h
Equerry.o: Equerry.h Object.h global.h MessageQueue.h Animals.h random.h
Equerry.o: generic.h
Owner.o: Owner.h Object.h global.h MessageQueue.h Animals.h random.h Buyer.h
