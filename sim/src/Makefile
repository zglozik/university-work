
#
#	Makefile for "sim"
#

default: linux

OPTIONS = -DDEBUG -D_X_VIEWER

SIMSOURCES = string.cc \
	simulator.cc wristmotion.cc bodybuilder.cc x_viewer.cc \
	body.cc expression.cc numanal.cc god.cc land.cc lex.yy.l y.tab.y

SIMOBJECTS = string.o \
	simulator.o wristmotion.o bodybuilder.o x_viewer.o \
	body.o expression.o numanal.o god.o land.o lex.yy.o y.tab.o

VSOURCES = xviewer.c
VOBJECTS = $(VSOURCES:.c=.o)

.SUFFIXES: .cc

.y.o:
	yacc -d $<
	mv -f y.tab.c y.tab.cc
	mv -f y.tab.h ../include
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c y.tab.cc
	mv -f y.tab.o $@

.y.cc:
	yacc -d $<
	mv -f y.tab.c $@
	mv -f y.tab.h ../include

.l.o:
	lex $<
	mv -f lex.yy.c lex.yy.cc
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c lex.yy.cc
	-mv -f lex.yy.o $@
	
.l.cc:
	lex $<
	mv -f lex.yy.c $@

.cc.o:
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $<

sim: $(SIMOBJECTS)
	$(CXX) -o $@ $(SIMOBJECTS) $(LDFLAGS)

xviewer: $(VOBJECTS)
	$(CC) -o $@ $(VOBJECTS) $(XLIBS) $(LDFLAGS)

#
#	platform specific
#

linux:
	make CXX="c++" \
		CFLAGS="-Wall" \
		CXXFLAGS="-g -Wall $(OPTIONS)" \
		CPPFLAGS="-I../include -I/usr/include/X11"\
		LDFLAGS="-lfl -lm"\
		XLIBS="-L/usr/X11/lib -lXm -lXpm -lXext -lXt -lX11"\
		sim xviewer

sgi:
	make CXX=CC CXXFLAGS="-ptused -no_prelink -O -DNEED_BOOL $(OPTIONS)" \
	CFLAGS="-I../include -I$(SDK)/h" \
	CPPFLAGS="-I../include -I$(SDK)/h" \
	LDFLAGS="-L$(SDK)/lib  -ldkit -ldbdk -ll -lm" \
	XLIBS="-L$(SDK)/lib  -ldkit -lXm -lXt -lX11" \
	sim xviewer

depend:
	makedepend -Y $(OPTIONS) -I../include $(SIMSOURCES) $(VSOURCES) 

backup:
	cd ../..; find sim \( -name "*.o" -or -name "*~" -or -name "sim" \) \
	-print > /tmp/$$$$; \
	tar -cvf $(HOME)/backup/sim.tar -X /tmp/$$$$ sim; \
	rm /tmp/$$$$
	gzip -f $(HOME)/backup/sim.tar

clean:
	-rm *.o sim core xviewer sim_si y.tab.cc ../include/y.tab.h lex.yy.cc

copy:
	rcp $(HOME)/backup/sim.tar.gz digo:
	rsh digo rcp sim.tar.gz chaos.hupe.hu:

#
#	dependencies
#

../include/y.tab.h: y.tab.y
	yacc -d y.tab.y
	mv -f y.tab.c y.tab.cc
	mv y.tab.h ../include
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c y.tab.cc

y.tab.cc: y.tab.y
lex.yy.cc: lex.yy.l ../include/y.tab.h

# DO NOT DELETE

string.o: ../include/string.hh ../include/debug.hh
simulator.o: ../include/simulator.hh ../include/bodyspec.hh
simulator.o: ../include/expression.hh ../include/array.hh ../include/debug.hh
simulator.o: ../include/array.cc ../include/funcs.hh ../include/map.hh
simulator.o: ../include/btree.hh ../include/pair.hh ../include/btree.cc
simulator.o: ../include/map.cc ../include/string.hh ../include/bodybuilder.hh
simulator.o: ../include/body.hh ../include/vector.hh ../include/vector.cc
simulator.o: ../include/list.hh ../include/list.cc ../include/transform.hh
simulator.o: ../include/transform.cc ../include/angles.hh
simulator.o: ../include/viewer.hh ../include/wristmotion.hh ../include/god.hh
simulator.o: ../include/land.hh ../include/hermite.hh ../include/hermite.cc
wristmotion.o: ../include/wristmotion.hh ../include/god.hh ../include/land.hh
wristmotion.o: ../include/debug.hh ../include/vector.hh ../include/funcs.hh
wristmotion.o: ../include/vector.cc ../include/viewer.hh ../include/body.hh
wristmotion.o: ../include/array.hh ../include/array.cc ../include/list.hh
wristmotion.o: ../include/list.cc ../include/transform.hh
wristmotion.o: ../include/transform.cc ../include/angles.hh ../include/map.hh
wristmotion.o: ../include/btree.hh ../include/pair.hh ../include/btree.cc
wristmotion.o: ../include/map.cc ../include/string.hh
wristmotion.o: ../include/expression.hh ../include/hermite.hh
wristmotion.o: ../include/hermite.cc
bodybuilder.o: ../include/bodybuilder.hh ../include/body.hh
bodybuilder.o: ../include/debug.hh ../include/vector.hh ../include/funcs.hh
bodybuilder.o: ../include/vector.cc ../include/array.hh ../include/array.cc
bodybuilder.o: ../include/list.hh ../include/list.cc ../include/transform.hh
bodybuilder.o: ../include/transform.cc ../include/angles.hh ../include/map.hh
bodybuilder.o: ../include/btree.hh ../include/pair.hh ../include/btree.cc
bodybuilder.o: ../include/map.cc ../include/string.hh ../include/viewer.hh
x_viewer.o: ../include/viewer.hh ../include/debug.hh ../include/vector.hh
x_viewer.o: ../include/funcs.hh ../include/vector.cc
body.o: ../include/body.hh ../include/debug.hh ../include/vector.hh
body.o: ../include/funcs.hh ../include/vector.cc ../include/array.hh
body.o: ../include/array.cc ../include/list.hh ../include/list.cc
body.o: ../include/transform.hh ../include/transform.cc ../include/angles.hh
body.o: ../include/map.hh ../include/btree.hh ../include/pair.hh
body.o: ../include/btree.cc ../include/map.cc ../include/string.hh
body.o: ../include/viewer.hh
expression.o: ../include/expression.hh ../include/array.hh
expression.o: ../include/debug.hh ../include/array.cc ../include/funcs.hh
expression.o: ../include/map.hh ../include/btree.hh ../include/pair.hh
expression.o: ../include/btree.cc ../include/map.cc ../include/string.hh
numanal.o: ../include/numanal.hh ../include/vector.hh ../include/debug.hh
numanal.o: ../include/funcs.hh ../include/vector.cc ../include/transform.hh
numanal.o: ../include/transform.cc ../include/angles.hh
god.o: ../include/god.hh ../include/land.hh ../include/debug.hh
god.o: ../include/vector.hh ../include/funcs.hh ../include/vector.cc
god.o: ../include/viewer.hh ../include/body.hh ../include/array.hh
god.o: ../include/array.cc ../include/list.hh ../include/list.cc
god.o: ../include/transform.hh ../include/transform.cc ../include/angles.hh
god.o: ../include/map.hh ../include/btree.hh ../include/pair.hh
god.o: ../include/btree.cc ../include/map.cc ../include/string.hh
god.o: ../include/numanal.hh
land.o: ../include/land.hh ../include/debug.hh ../include/vector.hh
land.o: ../include/funcs.hh ../include/vector.cc ../include/viewer.hh
lex.yy.o: ../include/bodyspec.hh ../include/expression.hh ../include/array.hh
lex.yy.o: ../include/debug.hh ../include/array.cc ../include/funcs.hh
lex.yy.o: ../include/map.hh ../include/btree.hh ../include/pair.hh
lex.yy.o: ../include/btree.cc ../include/map.cc ../include/string.hh
lex.yy.o: ../include/bodybuilder.hh ../include/body.hh ../include/vector.hh
lex.yy.o: ../include/vector.cc ../include/list.hh ../include/list.cc
lex.yy.o: ../include/transform.hh ../include/transform.cc
lex.yy.o: ../include/angles.hh ../include/viewer.hh ../include/wristmotion.hh
lex.yy.o: ../include/god.hh ../include/land.hh ../include/hermite.hh
lex.yy.o: ../include/hermite.cc
y.tab.o: ../include/bodyspec.hh ../include/expression.hh ../include/array.hh
y.tab.o: ../include/debug.hh ../include/array.cc ../include/funcs.hh
y.tab.o: ../include/map.hh ../include/btree.hh ../include/pair.hh
y.tab.o: ../include/btree.cc ../include/map.cc ../include/string.hh
y.tab.o: ../include/bodybuilder.hh ../include/body.hh ../include/vector.hh
y.tab.o: ../include/vector.cc ../include/list.hh ../include/list.cc
y.tab.o: ../include/transform.hh ../include/transform.cc ../include/angles.hh
y.tab.o: ../include/viewer.hh ../include/wristmotion.hh ../include/god.hh
y.tab.o: ../include/land.hh ../include/hermite.hh ../include/hermite.cc
