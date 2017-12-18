#include <stdio.h>
#include "table.h"

#define MAX 16
#define ROWS 5
#define STRMAX 128
#define PATH "book_table.txt"

int main () {
    
    /*the program will create a simple table, insert a few records,
close the table, open it again, read the records and print them. */
    table_t *tabla = NULL;
    long position;
    int rowaux = 0;
    void **values = NULL;

    tabla = table_open(PATH);
    
    position = table_first_pos(tabla);
    
    values = (void **)malloc(table_ncols(tabla) * sizeof(void *));

    while (1) {
        rowaux++;

        position = table_read_record(tabla, position);
        
        if (position == -1) {
            break;
        }
        
        for (int j = 0; j < table_ncols(tabla); j++) {
            values[j] = table_column_get(tabla, j);
        }
        
        printf("\nRow: %d\n\tValue 1: %s\tValue 2: %s\tValue 3: %d\tValue 4: %d\t", rowaux, (char *) values[0], (char *) values[1], *((int *) values[2]), *((int *) values[3]));
    }

    table_close(tabla);

    
    
    free(values);
    
    return 0;
}