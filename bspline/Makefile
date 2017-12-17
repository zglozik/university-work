
CXX = c++
LINK.o = $(CXX) $(LDFLAGS)
CXXFLAGS = -DDEBUG -g -I.
LDFLAGS =

SOURCES = bspline.cc approximation.cc approximation1.cc  householder.cc \
	mkcurve.cc

OBJECTS = $(SOURCES:.cc=.o)

all:	mkcurve

mkcurve: $(OBJECTS)
	$(CXX) -o $@ $^ $(LDFLAGS)

depend:
	makedepend -Y $(CXXFLAGS) $(SOURCES)

clean:
	rm *.o core *~ mkcurve

# DO NOT DELETE THIS LINE -- make depend depends on it.

bspline.o: bspline.hh ./stdtypes.h ./array.hh debug.hh array.cc funcs.hh
bspline.o: ./list.hh list.cc ./vector.hh vector.cc ./angles.hh ./map.hh
bspline.o: btree.hh pair.hh btree.cc map.cc ./matrix.hh matrix.cc
approximation.o: approximation.hh bspline.hh ./stdtypes.h ./array.hh debug.hh
approximation.o: array.cc funcs.hh ./list.hh list.cc ./vector.hh vector.cc
approximation.o: ./angles.hh ./map.hh btree.hh pair.hh btree.cc map.cc
approximation.o: ./matrix.hh matrix.cc householder.hh
approximation1.o: approximation.hh bspline.hh ./stdtypes.h ./array.hh
approximation1.o: debug.hh array.cc funcs.hh ./list.hh list.cc ./vector.hh
approximation1.o: vector.cc ./angles.hh ./map.hh btree.hh pair.hh btree.cc
approximation1.o: map.cc ./matrix.hh matrix.cc householder.hh
householder.o: householder.hh ./matrix.hh debug.hh ./stdtypes.h matrix.cc
mkcurve.o: approximation.hh bspline.hh ./stdtypes.h ./array.hh debug.hh
mkcurve.o: array.cc funcs.hh ./list.hh list.cc ./vector.hh vector.cc
mkcurve.o: ./angles.hh ./map.hh btree.hh pair.hh btree.cc map.cc ./matrix.hh
mkcurve.o: matrix.cc
