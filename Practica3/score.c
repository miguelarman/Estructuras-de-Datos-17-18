#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sql.h>
#include <sqlext.h>
#include "odbc.h"
#include "table.h"


#define PATH "book_table.txt"



int main (int argc, char **argv) {

	table_t *table = NULL;



	table = table_open(PATH);

    
    
    table_close(table);

    
    return 0;
}