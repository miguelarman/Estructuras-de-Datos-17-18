#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "index.h"


typedef struct {
    int key;
    long *positions;
    int amount;
} irecord;

struct index_ {
    char *path;
    irecord **keys;
    int n_keys;
};


int mycmp(const void *kptr, const void *e) {
  /*
  irecord *rec = *((irecord **) e);
  if (key < rec->key)
    return -1;
  else if (key > rec->key)
    return 1;
  else 
    return 0;
  */
}


/* 
   Creates a file for saving an empty index. The index is initialized
   to be of the specific tpe (in the basic version this is always INT)
   and to contain 0 entries.
 */
int index_create(int type, char *filename) {
    
    FILE *fp = NULL;
    int zero = 0;
    
    if (filename == NULL) {
        return -1;
    }
    
    fp = fopen(filename, "w");
    if (fp == NULL) {
        return -1;
    }
    
    fwrite(&type, sizeof(int), 1, fp);
    fwrite(&zero, sizeof(int), 1, fp);
    
    fclose(fp);
    
    return 0;
    
}

/* 
   Opens a previously created index: reads the contents of the index
   in an index_t structure that it allocates, and returns a pointer to
   it (or NULL if the files doesn't exist or there is an error). 

   NOTE: the index is stored in memory, so you can open and close the
   file in this function. However, when you are asked to save the
   index, you will not be given the path name again, so you must store
   in the structure either the FILE * (and in this case you must keep
   the file open) or the path (and in this case you will open the file
   again).
 */
index_t* index_open(char* path) {
    
    index_t *pi = NULL;
    int type, num_pos;
    FILE *pf = NULL;
    
    if (path == NULL) {
        return NULL;
    }
    
    pi = (index_t *)malloc(sizeof(index_t));
    if (pi == NULL) {
        return NULL;
    }
    
    
    
    
    pi->path = (char *)malloc((strlen(path) + 1) * sizeof(char));
    if (pi->path == NULL) {
        return NULL;
    }
    
    strcpy(pi->path, path);
  
    
    pf = fopen (path, "r");
    
    
    fread(&type, sizeof(int), 1, pf);
    fread(&(pi->n_keys), sizeof(int), 1, pf);
    
    pi->keys = (irecord **)malloc((pi->n_keys) * sizeof(irecord *));
    if (pi->keys == NULL) {
        free(pi->path);
        free(pi);
        return NULL;
    }
    
    for (int i = 0; i < pi->n_keys; i++) {
        pi->keys[i] = (irecord *)malloc(sizeof(irecord));
        if (pi->keys[i] == NULL) {
            for (int j = 0; j < i; j++) {
                free(pi->keys[j]);
            }
            free (pi->keys);
            free(pi->path);
            free(pi);
            return NULL;
        }
        
        fread(&(pi->keys[i]->key), sizeof(int), 1, pf);
        
        fread(&num_pos, sizeof(int), 1, pf);
        
        pi->keys[i]->positions = (long *)malloc(num_pos * sizeof(long));
        if (pi->keys[i]->positions == NULL) {
            for (int j = 0; j < i; j++) {
                free(pi->keys[j]);
            }
            free (pi->keys);
            free(pi->path);
            free(pi);
            return NULL;
        }
        
        fread(pi->keys[i]->positions, sizeof(long), num_pos, pf);
        
        
        pi->keys[i]->amount = num_pos;
    }
    
    return pi;
}

/* 
   Saves the current state of index in the file it came from. See the
   NOTE to index_open.
*/
int index_save(index_t* index) {
  
  return 0;
}


/* 
   Puts a pair key-position in the index. Note that the key may be
   present in the index or not... you must manage both situation. Also
   remember that the index must be kept ordered at all times.
*/
int index_put(index_t *index, int key, long pos) {
    
    int i = 0, intaux;
    long longaux;
    
    if (index == NULL) {
        return -1;
    }
    
    while (index->keys[i]->key < key || i < index->n_keys) {
        i++;
    }
    
    if (i == index->n_keys) {       /*Key is bigger than anyone in the index*/
        
        index->n_keys++;
        
        index->keys = (irecord **)realloc(index->keys, (index->n_keys) * sizeof(irecord *));
        if (index->keys == NULL) {
            return -1;
        }
        
        index->keys[i] = (irecord *)malloc(sizeof(irecord));                              /*Add new record*/
        if (index->keys[i] == NULL) {
            return -1;
        }
        
        index->keys[i]->key = key;
        index->keys[i]->positions = (long *)malloc(1 * sizeof(long));
        if (index->keys[i]->positions == NULL) {
            return -1;
        }
        
        index->keys[i]->positions[0] = pos;
        
        index->keys[i]->amount = 1;
        
        
        return 0;
        
    } else if (i == 0) {       /*Key is smaller than anyone in the index*/
        
        index->n_keys++;
        
        index->keys = (irecord **)realloc(index->keys, (index->n_keys) * sizeof(irecord *));
        if (index->keys == NULL) {
            return -1;
        }
        
        for (int j = index->n_keys - 1; j > 0; j--) {
            index->keys[j] = index->keys[j - 1];        /*Move every entry one position backwars*/
        }
        
        
        index->keys[0] = (irecord *)malloc(sizeof(irecord));
        if (index->keys[0] == NULL) {
            return -1;
        }
        
        
        index->keys[0]->key = key;                  /*Add new record*/
        index->keys[0]->positions = (long *)malloc(1 * sizeof(long));
        if (index->keys[0]->positions == NULL) {
            return -1;
        }
        
        index->keys[0]->positions[0] = pos;
        
        index->keys[0]->amount = 1;
        
        return 0;
        
    } else if (index->keys[i]->key == key) {
        
        index->keys[i]->amount++;
        
        index->keys[i]->positions = (long *)realloc((index->keys[i]->amount) * sizeof(long));
        if (index->keys[i]->positions == NULL) {
            return -1;
        }
        
        index->keys[i]->positions[amount - 1] = pos;
        
        intaux = amount - 1;
        
        while (j > 0 || index->keys[i]->positions[j] < index->keys[i]->positions[j - 1]) {
            
            longaux = index->keys[i]->positions[j];
            index->keys[i]->positions[j] = index->keys[i]->positions[j - 1];
            index->keys[i]->positions[j - 1] = longaux;
            
            j--;
        }
        
        
        return 0;
        
        
        
    } else if (index->keys[i]->key > key) {         /*No record with that key but key in between others*/
        
        index->n_keys++;
        
        index->keys = (irecord **)realloc(index->keys, (index->n_keys) * sizeof(irecord *));
        if (index->keys == NULL) {
            return -1;
        }
        
        
        for (int j = index->n_keys - 1; j > i; j--) {
            index->keys[j] = index->keys[j - 1];
        }
        
        index->keys[i] = (irecord *)malloc(sizeof(irecord));
        if (index->keys[i] == NULL) {
            return -1;
        }
        
        
        index->keys[i]->key = key;
        index->keys[i]->positions = (long *)malloc(1 * sizeof(long));
        if (index->keys[i]->positions == NULL) {
            return -1;
        }
        
        index->keys[i]->positions[0] = pos;
        index->keys[i]->amount = 1;
        
        
        return 0;
    } 
    
    
    return -1;
}

/* 
   Retrieves all the positions associated with the key in the index. 
   
   NOTE: the parameter nposs is not an array of integers: it is
   actually an integer variable that is passed by reference. In it you
   must store the number of elements in the array that you return,
   that is, the number of positions associated to the key. The call
   will be something like this:

   int n
   long *poss = index_get(index, key, &n);

   for (int i=0; i<n; i++) {
       Do something with poss[i]
   }

   ANOTHER NOTE: remember that the search for the key MUST BE DONE
   using binary search.

*/
long *index_get(index_t *index, int key, int* nposs) {
  if (index==NULL){
      return NULL;
  }
  long *poss;
  int p = 1;
  int u = index->n_keys;
  
    while (u >= p) {
        medio = (u+p)/2;
      
        if (index->keys[medio]->key==key){
            *nposs = 0;
            poss = (long *)malloc((index->keys[medio]->amount)*sizeof(long));
            for (int i=0; i < index->keys[medio]->amount ; i++){
                poss[i] = index->keys[medio]->positions[i];
                *nposs++;
            }
            return poss;
        }else if(index->keys[medio]>key){
            u = medio - 1;
        }else if (index->keys[medio]<key){
            p = medio + 1;
        }
    }
  
  
  return NULL;
}

/* 
   Closes the index by freeing the allocated resources 
*/
void index_close(index_t *index) {
    if (index==NULL){
        return;
    }
    for(int i=0; i<index->n_keys; i++){
        if (index->keys[i]!=NULL){
            if(index->keys[i]->positions != NULL){
                free(index->keys[i]->positions);
            }
            free(index->keys[i]);
        }
    }
    if (index->path != NULL){
      free(index->path);
    }
    free(index);
}

