
#
#	Makefile for Euler-computation
#

CXXFLAGS = -Wall -g
LDFLAGS = -L/usr/X11/lib -lXm -lXpm -lXext -lXt -lX11

SOURCES = main.cc euler.cc image.cc

OBJECTS = $(SOURCES:.cc=.o)

euler: $(OBJECTS)
	$(LINK.cc) -o $@ $(OBJECTS)

depend:
	makedepend -Y $(SOURCES)

# DO NOT DELETE

main.o: image.hh euler.hh
euler.o: euler.hh image.hh
image.o: image.hh
