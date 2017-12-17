
CFLAGS = -Wall -g

SOURCES = finder.c pattern.c

OBJECTS = $(SOURCES:.c=.o)

finder: $(OBJECTS)

backup:
	cd ../; tar cvzf finder.tar.gz finder/*.[ch] finder/Makefile \
	finder/README

depend:
	makedepend -Y $(SOURCES)

# DO NOT DELETE

finder.o: pattern.h finder.h stdtypes.h
pattern.o: pattern.h
