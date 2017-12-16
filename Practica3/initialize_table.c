#include <stdio.h>
#include "table.h"

int main () {
    
    char *path = "book_table.txt";
    
    type_t tipos_array[4] = {STR, STR, INT, INT};

    
    table_create(path, 4, tipos_array);

    return 0;
}