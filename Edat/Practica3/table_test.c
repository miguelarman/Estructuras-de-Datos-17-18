#include <stdio.h>
#include "table.h"

#define MAX 16
#define ROWS 5
#define STRMAX 128

int main () {
    
    /*the program will create a simple table, insert a few records,
close the table, open it again, read the records and print them. */
    table_t *tabla = NULL;
    type_t tipos_array[4] = {INT, INT, STR, INT};
    char path[MAX] = "fichero.txt";
    long position;
    int rowaux = 0;

    
    void **values = NULL;
    
    values = (void **)malloc(4 * sizeof(void *));
    
    table_create(path, 4, tipos_array);

    tabla = table_open(path);
    
    printf("number of columns: %d\n", table_ncols(tabla));
    
    
    for (int i = 0; i < ROWS; i++) {
        values[0] = (int *)malloc(sizeof(int));
        values[1] = (int *)malloc(sizeof(int));
        values[2] = (char *)malloc(STRMAX * sizeof(char));
        values[3] = (int *)malloc(sizeof(int));
        
        *((int *) values[0]) = rand()%10;
        *((int *) values[1]) = rand()%10;
        strcpy((char *) values[2], "Hola mundo.");
        *((int *) values[3]) = rand()%10;
        
        table_insert_record(tabla, values);
        
        free(values[0]);
        free(values[1]);
        free(values[2]);
        free(values[3]);
        
    }

    table_close(tabla);
    

    
    tabla = table_open(path);
    
    position = table_first_pos(tabla);
    
    while (1) {
        rowaux++;

        position = table_read_record(tabla, position);
        
        if (position == -1) {
            break;
        }
        
        // values[0] = (int *)malloc(sizeof(int));
        // values[1] = (int *)malloc(sizeof(int));
        // values[2] = (char *)malloc(STRMAX*sizeof(char));
        // values[3] = (int *)malloc(sizeof(int));
        
        for (int j = 0; j < table_ncols(tabla); j++) {
            values[j] = table_column_get(tabla, j);
        }
        
        // free(values[0]);
        // free(values[1]);
        // free(values[2]);
        // free(values[3]);
        
        printf("\nRow: %d\n\tValue 1 :%d\tValue 2 :%d\tValue 3 :%s\tValue 4 :%d\t", rowaux, *((int *)values[0]), *((int *)values[1]), (char *)values[2], *((int *)values[3]));
    }

    table_close(tabla);

    
    
    free(values);
    
    return 0;
}