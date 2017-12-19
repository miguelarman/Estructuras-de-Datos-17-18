#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sql.h>
#include <sqlext.h>
#include "odbc.h"
#include "table.h"
#include "index.h"

#define PATH_TABLE "book_table.txt"
#define PATH_INDEX "book_index.txt"
#define STRMAX 256

int main(int argc, char **argv){
    int score = 0, nposs;
    long position = 0;
    SQLHENV env;
    SQLHDBC dbc;
    SQLHSTMT stmt;
    SQLRETURN ret;
    SQLCHAR title[STRMAX], author[STRMAX];
    table_t *table = NULL;
    void **values = NULL;
    char buf[500];
    long *long_array = NULL;
    index_t *index = NULL;
    
    
    if (argc<2){
        printf("Not enought parameters: %d", argc);
        return -1;
    }
    
    score = atoi (argv[1]);
    
    
    /* CONNECT */
    ret = odbc_connect(&env, &dbc);
    if (!SQL_SUCCEEDED(ret)) {
        printf("\nError connecting\n");
        odbc_extract_error("SQLDriverConnect", dbc, SQL_HANDLE_DBC);
        return EXIT_FAILURE;
    }
    
    
    table = table_open(PATH_TABLE);
    if (table == NULL) {
        printf("\nError opening the table\n");
        return -1;
    }

    index = index_open(PATH_INDEX);
    if (index == NULL) {
        printf("\nError opening the index\n");
        table_close(table);
        return -1;
    }

    
    


    values = (void **)malloc(4 * sizeof(void *));
    if (values == NULL) {
        printf("\nError allocating values\n");
    }



    
    long_array = index_get(index, score, &nposs);
    
    for (int j = 0; j < nposs; j++) {
        position = table_read_record(table, long_array[j]);

        if (position == -1) {
            break;
        }
        
        for (int k = 0; k < table_ncols(table); k++) {
            values[k] = table_column_get(table, k);
        }




         /* Allocate a statement handle */
        ret = SQLAllocHandle(SQL_HANDLE_STMT, dbc, &stmt);
        if (!SQL_SUCCEEDED(ret)) {
            printf("\nError allocating stmt\n");
            odbc_extract_error("SQLDriverConnect", dbc, SQL_HANDLE_DBC);
            return EXIT_FAILURE;
        }
        
        sprintf(buf, "SELECT title, author FROM edition WHERE isbn = '%s';", (char*)values[0]);
        ret = SQLExecDirect(stmt, (SQLCHAR*) buf, SQL_NTS);
        if (!SQL_SUCCEEDED(ret)) {
            printf("\nError executing query");
            odbc_extract_error("SQLDriverConnect", dbc, SQL_HANDLE_DBC);
            return EXIT_FAILURE;
        }


        if (SQL_SUCCEEDED(ret = SQLFetch(stmt))) {
           ret = SQLGetData(stmt, 1, SQL_C_CHAR, title, sizeof(title), NULL);
           ret = SQLGetData(stmt, 2, SQL_C_CHAR, author, sizeof(author), NULL);
        } else {
            printf("\nError fetching data");
        }
        
        
        printf("\n%s\t%s", (char *) author, (char*) title);   
                
        /* free up statement handle */
        SQLFreeHandle(SQL_HANDLE_STMT, stmt);

    }


	
	
    
    printf("\n\n");
    
    /* DISCONNECT */
    ret = odbc_disconnect(env, dbc);
    if (!SQL_SUCCEEDED(ret)) {
        return EXIT_FAILURE;
    }
    
    free(values);
    
    table_close(table);
    
    return 0;
}