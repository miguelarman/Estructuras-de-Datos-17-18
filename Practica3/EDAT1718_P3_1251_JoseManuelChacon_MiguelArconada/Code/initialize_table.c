#include <stdio.h>
#include "table.h"

#define PATH "book_table.txt"

int main () {
    
    
    
    type_t tipos_array[4] = {STR, STR, INT, INT};

    
    table_create(PATH, 4, tipos_array);

    return 0;
}