CC = gcc
CFLAGS = -Wall -Wextra -pedantic -ansi
LDLIBS = -lodbc

EXE = sale buy fill_sales

all : $(EXE)

clean :
	rm -f *.o core $(EXE)

$(EXE) : % : %.o odbc.o
