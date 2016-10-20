INCDIR=./include
_DEPS=rs232.h
DEPS=$(patsubst %,$(INCDIR)/%,$(_DEPS))

CC=g++
CFLAGS=-I$(INCDIR) -std=c++11 -Wall

ODIR=./obj
_OBJ=rs232.o signalHandler.o dataLogger.o
OBJ=$(patsubst %,$(ODIR)/%,$(_OBJ))

EXE=dataLogger.exe
INSTALLPATH=../out

LLIBS=-lpthread
LIBS=-pthread

$(ODIR)/rs232.o: rs232.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

$(ODIR)/%.o: %.cpp $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS) $(LLIBS)

all: $(OBJ)
	$(CC) -o $(EXE) $^ $(CFLAGS) $(LIBS)
	cp $(EXE) $(INSTALLPATH)

clean:
	rm -f $(ODIR)/*.o *~  core $(EXE)

