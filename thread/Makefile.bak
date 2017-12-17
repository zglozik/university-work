
CXXFLAGS = -Wall -D_REENTRANT
LDLIBS = -lpthread

SOURCES = thread1.cc worker.cc mutex.cc thread.cc

thread1: thread1.o worker.o mutex.o thread.o
	$(LINK.cc) $^ $(LDLIBS) -o $@

depend:
	makedepend -Y $(SOURCES)

# DO NOT DELETE

thread1.o: worker.hh thread.hh
worker.o: worker.hh thread.hh
mutex.o: mutex.hh
thread.o: thread.hh
