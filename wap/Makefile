
# Makefile

CFLAGS = -Wall -g -pedantic
LDFLAGS = -lpthread

SOURCES = timer_manager.c queue.c statemachine.c

OBJECTS = $(SOURCES:.c=.o)

test: $(OBJECTS)
	$(CC) -o $@ $^ $(LDFLAGS)

clean:
	rm *.o sb *~ \#* core

depend:
	makedepend -Y $(SOURCES)

TAGS:
	etags $(SOURCES)

# DO NOT DELETE

timer_manager.o: timer_manager.h
queue.o: queue.h
statemachine.o: statemachine.h timer_manager.h debug.h queue.h
