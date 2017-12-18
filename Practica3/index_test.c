#include <stdio.h>
#include <stdlib.h>
#include "index.h"
#include "type.h"

void comprobar (int i) {
    if (i == -1) {
        printf("Error");
        exit(-1);
    }
    
    return;
}

int main () {
    
    index_t *index = NULL;
    char *path = "index_test.txt";
    int return_aux, key_aux, nposs = 0;
    long pos_aux, *long_array = NULL;
    
    return_aux = index_create(INT, path);
    comprobar(return_aux);
    
    
    
    index = index_open(path);
    if (index == NULL) {
        printf("Error opening index");
        return -1;
    }
    
    for (int i = 0; i < 10; i++) {
        key_aux = rand()%3 + 1;
        pos_aux = 0L + (long) rand()%100;
        
        
        return_aux = index_put(index, key_aux, pos_aux);
        comprobar(return_aux);
        
        printf("\nKey: %d\tPosition: %ld", key_aux, pos_aux);
    }
    
    return_aux = index_save(index);
    comprobar(return_aux);
    
    index_close(index);
    
    
    
    printf("\n\n\n");
    
    
    index = index_open(path);
    
    for (int k = 1; k <= 3; k++) {
        long_array = index_get(index, k, &nposs);
        
        
        printf("\nKey: %d (nposs: %d)", k, nposs);
        for (int l = 0; l < nposs; l++) {
            printf("\t%ld", long_array[l]);
        }
        
        free(long_array);
    }
    
    
    index_close(index);
    
    return 0;
}