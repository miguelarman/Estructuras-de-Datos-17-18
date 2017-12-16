########################################################
CC=gcc
CFLAGS = -g -Wall -pedantic -ansi -std=c99
FILES = table_test initialize_table score suggest
########################################################
# OBJECTSCOMPILEALL = plane.o cube.o game.o print_speed.o windows.o player.o mtype.o stype.o gametime.o readchar.o menu.o compileAll.o 
# OBJECTSWINDOWS_TEST = windows.o windows_test.o
# OBJECTSGAME_TEST = game.o cube.o plane.o mtype.o stype.o player.o gametime.o readchar.o game_test.o
# OBJECTSEPI = gametime.o epyleptic.o

OBJECTSTABLE_TEST = table.o table_test.o type.o
OBJECTSSCORE = table.o type.o score.o
OBJECTSSUGGEST = table.o type.o suggest.o
OBJECTSINITIALIZE = table.o type.o initialize_table.o
########################################################

all: $(FILES) clear

# compileAll: $(OBJECTSCOMPILEALL)
# 	$(CC) $(CFLAGS) -o compileAll $(OBJECTSCOMPILEALL)

# compileAll.o: windows.h cube.h plane.h print_speed.h player.h mtype.h stype.h general.h compileAll.c
# 	$(CC) $(CFLAGS) -c compileAll.c 

# windows_test: $(OBJECTSWINDOWS_TEST)
# 	$(CC) $(CFLAGS) -o windows_test $(OBJECTSWINDOWS_TEST)

# windows_test.o: windows.h windows_test.c
# 	$(CC) $(CFLAGS) -c windows_test.c

# game_test: $(OBJECTSGAME_TEST)
# 	$(CC) $(CFLAGS) -o game_test $(OBJECTSGAME_TEST)

# game_test.o: game.h cube.h plane.h mtype.h stype.h player.h gametime.h readchar.h general.h game_test.c
# 	$(CC) $(CFLAGS) -c game_test.c

# epyleptic: $(OBJECTSEPI)
# 	$(CC) $(CFLAGS) -o epyleptic $(OBJECTSEPI)

# epyleptic.o: gametime.h epyleptic.c
# 	$(CC) $(CFLAGS) -c epyleptic.c

#p3_e1: $(OBJECTSP3_E1)
#	$(CC) $(CFLAGS) -o p3_e1 $(OBJECTSP3_E1)

#p3_e1.o: functions.h queue.h node.h p3_e1.c
#	$(CC) $(CFLAGS) -c p3_e1.c

table_test: $(OBJECTSTABLE_TEST)
	$(CC) $(CFLAGS) -o table_test $(OBJECTSTABLE_TEST)

table_test.o: type.h table.h table_test.c
	$(CC) $(CFLAGS) -c table_test.c

score: $(OBJECTSSCORE)
	$(CC) $(CFLAGS) -o score $(OBJECTSSCORE)

score.o: type.h table.h score.c
	$(CC) $(CFLAGS) -c score.c

suggest: $(OBJECTSSUGGEST)
	$(CC) $(CFLAGS) -o suggest $(OBJECTSSUGGEST)

suggest.o: type.h table.h suggest.c
	$(CC) $(CFLAGS) -c suggest.c

initialize_table: $(OBJECTSINITIALIZE)
	$(CC) $(CFLAGS) -o initialize_table $(OBJECTSSUGGEST)

initialize_table.o: type.h table.h initialize_table.c
	$(CC) $(CFLAGS) -c initialize_table.c


# windows.o: windows.c windows.h
# 	$(CC) $(CFLAGS) -c windows.c

# cube.o: cube.c cube.h
# 	$(CC) $(CFLAGS) -c cube.c

# player.o: player.c player.h 
# 	$(CC) $(CFLAGS) -c player.c

# mtype.o: mtype.c mtype.h 
# 	$(CC) $(CFLAGS) -c mtype.c

# stype.o: stype.c stype.h 
# 	$(CC) $(CFLAGS) -c stype.c

# gametime.o: gametime.c gametime.h 
# 	$(CC) $(CFLAGS) -c gametime.c

# readchar.o: readchar.c readchar.h 
# 	$(CC) $(CFLAGS) -c readchar.c

# plane.o: plane.c plane.h player.o cube.o
# 	$(CC) $(CFLAGS) -c plane.c

# print_speed.o: print_speed.c print_speed.h windows.o
# 	$(CC) $(CFLAGS) -c print_speed.c

# menu.o: menu.c menu.h
# 	$(CC) $(CFLAGS) -c menu.c

# game.o: cube.o plane.o mtype.o player.o gametime.o readchar.o game.c game.h
# 	$(CC) $(CFLAGS) -c game.c

table.o: type.o table.c table.h
	$(CC) $(CFLAGS) -c table.c

type.o: type.c type.h
	$(CC) $(CFLAGS) -c type.c






# windowstest:
# 	@echo Ejecutando windows_test
# 	@./windows_test

# windowstestval:
# 	@echo Ejecutando windows_test con valgrind
# 	@valgrind --leak-check=full ./windows_test

# gametest:
# 	@echo Ejecutando game_test
# 	@./game_test

# gametestval:
# 	@echo Ejecutando game_test con valgrind
# 	@valgrind --leak-check=full ./game_test






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