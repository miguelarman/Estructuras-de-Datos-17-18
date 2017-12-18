########################################################
CC=gcc
CFLAGS = -g -Wall -pedantic -ansi -std=c99
LDLIBS = -lodbc
FILES = table_test score suggest initialize_table
########################################################
OBJECTSTABLE_TEST = table.o table_test.o type.o
OBJECTSINDEX_TEST = index.o index_test.o type.o
OBJECTSSCORE_TEST = table.o score_test.o type.o
OBJECTSSCORE = odbc.o table.o type.o score.o
OBJECTSSUGGEST = odbc.o table.o type.o suggest.o
OBJECTSINITIALIZE = odbc.o table.o type.o initialize_table.o
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

index_test: $(OBJECTSINDEX_TEST)
	$(CC) $(CFLAGS) -o index_test $(OBJECTSINDEX_TEST)

index_test.o: type.h index.h index_test.c
	$(CC) $(CFLAGS) -c index_test.c

score_test: $(OBJECTSSCORE_TEST)
	$(CC) $(CFLAGS) -o score_test $(OBJECTSSCORE_TEST)

score_test.o: type.h table.h score_test.c
	$(CC) $(CFLAGS) -c score_test.c

score: $(OBJECTSSCORE)
	$(CC) $(CFLAGS) $(OBJECTSSCORE) $(LDLIBS) -o score

score.o: type.h table.h odbc.h score.c
	$(CC) $(CFLAGS) -c score.c

suggest: $(OBJECTSSUGGEST)
	$(CC) $(CFLAGS) $(OBJECTSSUGGEST) $(LDLIBS) -o suggest

suggest.o: type.h table.h suggest.c
	$(CC) $(CFLAGS) -c suggest.c

initialize_table: $(OBJECTSINITIALIZE)
	$(CC) $(CFLAGS) $(OBJECTSINITIALIZE) $(LDLIBS) -o initialize_table

initialize_table.o: type.h table.h initialize_table.c
	$(CC) $(CFLAGS) -c initialize_table.c




table.o: type.o table.c table.h
	$(CC) $(CFLAGS) -c table.c

type.o: type.c type.h
	$(CC) $(CFLAGS) -c type.c

index.o: type.o index.c index.h
	$(CC) $(CFLAGS) -c index.c

#odbc.o: odbc.c odbc.h
#	$(CC) $(CFLAGS) -c odbc.c




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