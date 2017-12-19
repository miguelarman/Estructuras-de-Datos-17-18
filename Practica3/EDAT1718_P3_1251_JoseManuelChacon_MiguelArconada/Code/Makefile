########################################################
CC=gcc
CFLAGS = -g -Wall -pedantic -ansi -std=c99
LDLIBS = -lodbc
FILES = table_test index_test score score_test score_index suggest suggest_index initialize_table initialize_index
########################################################
OBJECTSTABLE_TEST = table.o table_test.o type.o
OBJECTSINDEX_TEST = index.o index_test.o type.o

OBJECTSSCORE_TEST = table.o score_test.o type.o
OBJECTSSCORE = odbc.o table.o type.o score.o
OBJECTSSUGGEST = odbc.o table.o type.o suggest.o

OBJECTSSCORE_INDEX = odbc.o table.o type.o index.o score_index.o
OBJECTSSUGGEST_INDEX = odbc.o table.o type.o index.o suggest_index.o

OBJECTSINITIALIZE_TABLE= odbc.o table.o type.o initialize_table.o
OBJECTSINITIALIZE_INDEX = odbc.o table.o type.o index.o initialize_index.o
########################################################

all: $(FILES) clear

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





score_index_test: $(OBJECTSSCORE_INDEX_TEST)
	$(CC) $(CFLAGS) -o score_index_test $(OBJECTSSCORE_INDEX_TEST)

score_index_test.o: type.h table.h index.h score_index_test.c
	$(CC) $(CFLAGS) -c score_index_test.c

score_index: $(OBJECTSSCORE_INDEX)
	$(CC) $(CFLAGS) $(OBJECTSSCORE_INDEX) $(LDLIBS) -o score_index

score_index.o: type.h table.h odbc.h index.h score_index.c
	$(CC) $(CFLAGS) -c score_index.c

suggest_index: $(OBJECTSSUGGEST_INDEX)
	$(CC) $(CFLAGS) $(OBJECTSSUGGEST_INDEX) $(LDLIBS) -o suggest_index

suggest_index.o: type.h table.h index.h suggest_index.c
	$(CC) $(CFLAGS) -c suggest_index.c





initialize_table: $(OBJECTSINITIALIZE_TABLE)
	$(CC) $(CFLAGS) $(OBJECTSINITIALIZE_TABLE) $(LDLIBS) -o initialize_table

initialize_table.o: type.h table.h initialize_table.c
	$(CC) $(CFLAGS) -c initialize_table.c



initialize_index: $(OBJECTSINITIALIZE_INDEX)
	$(CC) $(CFLAGS) $(OBJECTSINITIALIZE_INDEX) $(LDLIBS) -o initialize_index

initialize_index.o: type.h table.h index.h initialize_index.c
	$(CC) $(CFLAGS) -c initialize_index.c




table.o: type.o table.c table.h
	$(CC) $(CFLAGS) -c table.c

type.o: type.c type.h
	$(CC) $(CFLAGS) -c type.c

index.o: type.o index.c index.h
	$(CC) $(CFLAGS) -c index.c



clear:
	rm -rf *.o 

clean:
	rm -rf *.o $(FILES)
