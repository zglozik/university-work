
CXX = c++
CXXFLAGS = -I/usr/include/X11 -Wall -g
LDLIBS = -L/usr/X11/lib -lXm -lXext -lXpm -lXt -lSM -lICE -lX11 -lfl
LDFLAGS = 

CXXSRCS = logo.cc gc.cc Expression.cc SymbolTable.cc BuildUp.cc Turtle.cc \
	string.cc y.tab.cc lex.yy.cc

CSRCS = 
OBJS = $(CXXSRCS:.cc=.o) $(CSRCS:.c=.o)

logo: $(OBJS)
	$(LINK.cc) $^ $(LDLIBS) -o $@

depend:
	makedepend -Y $(CXXSRCS)

clean:
	rm *.o *~ test core y.tab.cc lex.yy.cc

y.tab.h y.tab.cc:	logo.y
	yacc -d logo.y
	mv y.tab.c y.tab.cc

lex.yy.cc:	logo.l
	flex logo.l
	mv lex.yy.c lex.yy.cc

# DO NOT DELETE

logo.o: logo.h SymbolTable.h cstring.h stdtypes.h gc.h BuildUp.h Expression.h
logo.o: Turtle.h
gc.o: gc.h
Expression.o: Expression.h cstring.h stdtypes.h gc.h Turtle.h SymbolTable.h
Expression.o: BuildUp.h
SymbolTable.o: SymbolTable.h cstring.h stdtypes.h gc.h BuildUp.h Expression.h
SymbolTable.o: Turtle.h
BuildUp.o: BuildUp.h stdtypes.h gc.h Expression.h cstring.h Turtle.h
BuildUp.o: SymbolTable.h
Turtle.o: Turtle.h gc.h
string.o: cstring.h stdtypes.h gc.h
y.tab.o: logo.h SymbolTable.h cstring.h stdtypes.h gc.h BuildUp.h
y.tab.o: Expression.h Turtle.h
lex.yy.o: stdtypes.h gc.h y.tab.h
