#include <stdio.h>
#include "table.h"
#include "index.h"

#define PATH_TABLE "book_table.txt"
#define PATH_INDEX "book_index.txt"

int main () {
    
    int return_aux;

    return_aux = index_create(INT, PATH_INDEX);
    if (return_aux == -1) {
    	printf("\nError initializing index\n");
    }



    return 0;
}