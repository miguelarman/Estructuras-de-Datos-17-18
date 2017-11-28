#include <stdio.h>
#include "table.h"

#define MAX 16

int main () {
    
    table_t *tabla = NULL;
    type_t tipos_array[4] = {INT, INT, STR, INT};
    char path[MAX] = "fichero.txt";
    
    table_create(path, 4, tipos_array);

    tabla = table_open(path);
    
    printf("number of columns: %d", table_ncols(tabla));
    
    
    
    
    
    
    
    
    
    table_close(tabla);
    
    return 0;
}