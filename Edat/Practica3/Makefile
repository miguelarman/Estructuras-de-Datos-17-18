########################################################
CC=gcc
CFLAGS = -g -Wall -pedantic -ansi
FILES = compileAll windows_test game_test epyleptic
########################################################
OBJECTSCOMPILEALL = plane.o cube.o game.o print_speed.o windows.o player.o mtype.o stype.o gametime.o readchar.o menu.o compileAll.o 
OBJECTSWINDOWS_TEST = windows.o windows_test.o
OBJECTSGAME_TEST = game.o cube.o plane.o mtype.o stype.o player.o gametime.o readchar.o game_test.o
OBJECTSEPI = gametime.o epyleptic.o
########################################################

all: $(FILES) clear

compileAll: $(OBJECTSCOMPILEALL)
	$(CC) $(CFLAGS) -o compileAll $(OBJECTSCOMPILEALL)

compileAll.o: windows.h cube.h plane.h print_speed.h player.h mtype.h stype.h general.h compileAll.c
	$(CC) $(CFLAGS) -c compileAll.c 

windows_test: $(OBJECTSWINDOWS_TEST)
	$(CC) $(CFLAGS) -o windows_test $(OBJECTSWINDOWS_TEST)

windows_test.o: windows.h windows_test.c
	$(CC) $(CFLAGS) -c windows_test.c

game_test: $(OBJECTSGAME_TEST)
	$(CC) $(CFLAGS) -o game_test $(OBJECTSGAME_TEST)

game_test.o: game.h cube.h plane.h mtype.h stype.h player.h gametime.h readchar.h general.h game_test.c
	$(CC) $(CFLAGS) -c game_test.c

epyleptic: $(OBJECTSEPI)
	$(CC) $(CFLAGS) -o epyleptic $(OBJECTSEPI)

epyleptic.o: gametime.h epyleptic.c
	$(CC) $(CFLAGS) -c epyleptic.c

#p3_e1: $(OBJECTSP3_E1)
#	$(CC) $(CFLAGS) -o p3_e1 $(OBJECTSP3_E1)

#p3_e1.o: functions.h queue.h node.h p3_e1.c
#	$(CC) $(CFLAGS) -c p3_e1.c


windows.o: windows.c windows.h
	$(CC) $(CFLAGS) -c windows.c

cube.o: cube.c cube.h
	$(CC) $(CFLAGS) -c cube.c

player.o: player.c player.h 
	$(CC) $(CFLAGS) -c player.c

mtype.o: mtype.c mtype.h 
	$(CC) $(CFLAGS) -c mtype.c

stype.o: stype.c stype.h 
	$(CC) $(CFLAGS) -c stype.c

gametime.o: gametime.c gametime.h 
	$(CC) $(CFLAGS) -c gametime.c

readchar.o: readchar.c readchar.h 
	$(CC) $(CFLAGS) -c readchar.c

plane.o: plane.c plane.h player.o cube.o
	$(CC) $(CFLAGS) -c plane.c

print_speed.o: print_speed.c print_speed.h windows.o
	$(CC) $(CFLAGS) -c print_speed.c

menu.o: menu.c menu.h
	$(CC) $(CFLAGS) -c menu.c

game.o: cube.o plane.o mtype.o player.o gametime.o readchar.o game.c game.h
	$(CC) $(CFLAGS) -c game.c









windowstest:
	@echo Ejecutando windows_test
	@./windows_test

windowstestval:
	@echo Ejecutando windows_test con valgrind
	@valgrind --leak-check=full ./windows_test

gametest:
	@echo Ejecutando game_test
	@./game_test

gametestval:
	@echo Ejecutando game_test con valgrind
	@valgrind --leak-check=full ./game_test





epylepsia:
	@./epyleptyc




clear:
	rm -rf *.o 

clean:
	rm -rf *.o $(FILES)

run:
#	@echo ">>>>>>Running p3_e1"
#	./p3_e1 "nodos.txt"

runv:
#	@echo ">>>>>>Running p3_e1 with valgrind"
#	valgrind --leak-check=full ./p3_e1 "nodos.txt"