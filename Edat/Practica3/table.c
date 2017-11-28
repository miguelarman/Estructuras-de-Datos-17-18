#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "table.h"
#include "type.h"

/*USAR -std=c99*/

struct table_ {
  FILE *fp;
  int ncols;
  type_t *types;
  long first;
  void **values;
};

/* 
   Creates a file that stores an empty table. This function doesn't
   keep any information in memory: it simply creates the file, stores
   the header information, and closes it.
*/
void table_create(char* path, int ncols, type_t* types) {
    
    if (ncols < 1 || path == NULL || types == NULL) {
        return;
    }

    FILE *fp = NULL;
    int *values = NULL;
    
    fp = fopen(path, "w");
    if (fp == NULL) {
        return;
    }
    
    fwrite(&ncols, sizeof(int), 1, fp);
    fwrite(types, sizeof(int), ncols, fp);
    
    fclose(fp);
    
    return;
}

/* 
   Opens a table given its file name. Returns a pointer to a structure
   with all the information necessary to manage the table. Returns
   NULL is the file doesn't exist or if there is any error.
*/
table_t* table_open(char* path) {
    
    FILE *fp = NULL;
    table_t *table = NULL;
    int ncols;
    char *buf = NULL;
    
    if (path == NULL) {
        return NULL;
    }
    
    fp = fopen(path, "r+");
    if (fp == NULL)  {
        return NULL;
    }
    
    table = (table_t *)malloc(sizeof(table_t));
    if (table == NULL) {
        fclose (fp);
        return NULL;
    }
    
    table->fp = fp;
    
    fread(&ncols, sizeof(int), 1, fp);   
    
    table->ncols = ncols;
    
    table->types = (type_t *)malloc(ncols * sizeof(type_t));
    
    fread(table->types, sizeof(int), ncols, fp);
    
    t->values=(void **)malloc(t->ncols * sizeof(void*));
    if (t->values == NULL) {
        fclose(fp);
        free(table->types);
        free(table);
        return NULL;
    }
    
    t->values[0]=malloc(1);
    
    table->first = ftell(fp);
  
    return table;
}

/* 
   Closes a table freeing the alloc'ed resources and closing the file
   in which the table is stored.
*/
void table_close(table_t* table) {
    
    if (table == NULL) {
        return;
    }
    
    if (table->fp != NULL) {
        fclose(table->fp);
    }
    
    if (table->types != NULL) {
        free(table->types);
    }
    
    if (table->values[0] != NULL) {
        free(table->values[0]);
    }
    
    if (table->values != NULL) {
        free(table->values);
    }
    
    
    if (table != NULL) {
        free(table);
    }
    
    return;
}

/* 
   Returns the number of columns of the table 
*/
int table_ncols(table_t* table) {
    if (table == NULL) {
        return -1;
    }
    
    return (table->ncols);
}

/* 
   Returns the array with the data types of the columns of the
   table. Note that typically this kind of function doesn't make a
   copy of the array, rather, it returns a pointer to the actual array
   contained in the table structure. This means that the calling
   program should not, under any circumstance, modify the array that
   this function returns.
 */
type_t* table_types(table_t* table) {
    if (table == NULL) {
        return NULL;
    }
    
    return (table->types);
}

/* 
   Returns the position in the file of the first record of the table 
*/
long table_first_pos(table_t* table) {
    if (table == NULL) {
        return NULL;
    }
    
    return (table->first);
}

/* 
   Returns the position in the file in which the table is currently
   positioned. 
*/
long table_cur_pos(table_t* table) {
    if (table == NULL) {
        return NULL;
    }
    
    return (ftell(table->fp));
}

/* 
   Returns the position just past the last byte in the file, where a
   new record should be inserted.
*/
long table_last_pos(table_t* table) {
    
    FILE *fpaux = NULL;
    
    if (table == NULL) {
        return NULL;
    }
    
    fpaux = table->pf;
    
    fseek(fpaux, 0, SEEK_END);
    
    
    return (ftell(fpaux));
}

/* 
   Reads the record starting in the specified position. The record is
   read and stored in memory, but no value is returned. The value
   returned is the position of the following record in the file or -1
   if the position requested is past the end of the file.
*/
long table_read_record(table_t* table, long pos) {
    
    int len;

    /*fseek(table->fp, OL, SEEK_END);
    if (ftell(table->fp) <= pos) {
        return -1;
    }*/
    
    if (table_last_pos(table) <= pos) {
        return -1;
    }
    
    free(table->values[0]);
    
    fseek(table->fp, pos, SEEK_SET);

    fread(&len,sizeof(int), 1, table->fp);
    
    char *buf = (char *)malloc(len*sizeof(char));
    
    fread(buf, sizeof(char), len, table->fp);
    
    for (int i=0; i<table->ncols;i++) {
        table->values[i] = buf;
        buf +=value_length(table->types[i], table->values[i]);
    }
    
    return ftell(table->fp);
}

/*
  Returns a pointer to the value of the given column of the record
  currently in memory. The value is cast to a void * (it is always a
  pointer: if the column is an INT, the function will return a pointer
  to it).. Returns NULL if there is no record in memory or if the
  column doesn't exist.
*/
void *table_column_get(table_t* table, int col) {
  /* To be implemented */
  return NULL;
}


/* 
   Inserts a record in the last available position of the table. Note
   that all the values are cast to void *, and that there is no
   indication of their actual type or even of how many values we ask
   to store... why?
  */
void table_insert_record(table_t* table, void** values) {
  /* To be implemented */
  return;
}
