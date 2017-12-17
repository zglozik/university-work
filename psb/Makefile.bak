
CXXFLAGS = -Wall
LDFLAGS = -L$(PVM_ROOT)/lib/$(PVM_ARCH) -lpvm3
CPPFLAGS = -I$(PVM_ROOT)/include

SOURCES = main.cc Person.cc Manager.cc Object.cc Assistant.cc\
	WaitingQueue.cc Store.cc random.cc

OBJECTS = $(SOURCES:.cc=.o)

%: %.o
	$(CXX) -o $@ $^ $(LDFLAGS)
	ln -f $@ $(HOME)/pvm3/bin/$(PVM_ARCH)

all: main Person Manager Assistant WaitingQueue Store

main: main.o MessageQueue.o Object.o global.o

Person: Person.o MessageQueue.o Object.o global.o random.o

Manager: Manager.o MessageQueue.o Object.o global.o

Assistant: Assistant.o MessageQueue.o Object.o global.o

WaitingQueue: WaitingQueue.o MessageQueue.o Object.o global.o

Store: Store.o MessageQueue.o Object.o global.o

backup:
	cd ..; tar cf sb.tar sb/*.cc sb/*.h sb/Makefile

clean:
	rm *.o sb *~ \#* core

depend:
	makedepend -Y $(SOURCES)

# DO NOT DELETE

main.o: stdtypes.h debug.h MessageQueue.h Object.h global.h
Person.o: debug.h Person.h MessageQueue.h Object.h stdtypes.h global.h
Person.o: random.h
Manager.o: debug.h Manager.h global.h MessageQueue.h Object.h
Object.o: Object.h debug.h
Assistant.o: Assistant.h stdtypes.h debug.h MessageQueue.h Object.h global.h
WaitingQueue.o: WaitingQueue.h stdtypes.h debug.h MessageQueue.h Object.h
WaitingQueue.o: global.h
Store.o: Store.h stdtypes.h debug.h MessageQueue.h Object.h global.h
random.o: random.h
