
#
#	Makefile for "entropy"
#

OPTIONS = -DDEBUG 
CXXFLAGS = -Wall -g -I../include $(OPTIONS)

SOURCES = main.cc

OBJECTS = main.o

entropy: $(OBJECTS)
	$(CXX) -o $@ $(OBJECTS) $(LDFLAGS)

depend:
	makedepend -Y $(OPTIONS) -I../include $(SOURCES)

backup:
	cd ../..; tar cvf $(HOME)/backup/entropy.tar \
	entropy/*/*.cc entropy/*/*.hh entropy/src/Makefile
	gzip -f $(HOME)/backup/entropy.tar

clean:
	rm *.o entropy core

copy:
	rcp $(HOME)/backup/entropy.tar.gz digo:
	rsh digo rcp entropy.tar.gz chaos.hupe.hu:

# DO NOT DELETE

main.o: ../include/array.hh ../include/debug.hh ../include/array.cc
main.o: ../include/funcs.hh ../include/list.hh ../include/list.cc
