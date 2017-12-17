#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sql.h>
#include <sqlext.h>
#include "odbc.h"
#include "table.h"

#define PATH "book_table.txt"
#define STRMAX 256

int main(int argc, char **argv){
    
    int score = 0;
    long position = 0;
    SQLHENV env;
    SQLHDBC dbc;
    SQLHSTMT stmt;
    SQLRETURN ret;
    SQLCHAR title[STRMAX], author[STRMAX];
    table_t *table = NULL;
    void **values = NULL;
    char buf[500];
    
    
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
    
    
    table = table_open(PATH);
	if (table == NULL) {
		printf("\nError opening the table\n");
		return -1;
	}
	
	
	values = (void **)malloc(table_ncols(table) * sizeof(void *));
	if (values == NULL) {
	    printf("\nError allocating values\n");
	    table_close(table);

	    /* DISCONNECT */
	    ret = odbc_disconnect(env, dbc);
	    
	    return -1;
	}
	
	
	
	
	
	while (1) {
        position = table_read_record(table, position);
        
        if (position == -1) {
            break;
        }
        
        for (int j = 0; j < table_ncols(table); j++) {
            values[j] = table_column_get(table, j);
        }
        printf("%d\n", *((int*)values[2]));
        
        if (*((int *) values[2]) == score){
            
            
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
            
            
            /*ret = SQLCloseCursor(stmt);
            if (!SQL_SUCCEEDED(ret)) {
                printf("\nError closing cursor for argv[%d]\n", i);
                odbc_extract_error("SQLDriverConnect", dbc, SQL_HANDLE_DBC);
                return EXIT_FAILURE;
            }
            

	        sprintf(buf, "SELECT author FROM edition WHERE isbn = '%s';", (char*)values[2]);
            ret = SQLExecDirect(stmt, (SQLCHAR*) buf, SQL_NTS);
            if (!SQL_SUCCEEDED(ret)) {
                printf("\nError executing query for inserting into purchases for argv[%d]");
                odbc_extract_error("SQLDriverConnect", dbc, SQL_HANDLE_DBC);
                return EXIT_FAILURE;
            }


            if (SQL_SUCCEEDED(ret = SQLFetch(stmt))) {
    	       ret = SQLGetData(stmt, 1, SQL_C_CHAR, author, sizeof(author), NULL);
            }*/
            
            
            
            
            
            printf("\n%s\t%s", (char *) author, (char*) title);   
                    
            /* free up statement handle */
            SQLFreeHandle(SQL_HANDLE_STMT, stmt);
        }
    }
    
    /* DISCONNECT */
    ret = odbc_disconnect(env, dbc);
    if (!SQL_SUCCEEDED(ret)) {
        return EXIT_FAILURE;
    }
    
    free(values);
    
    table_close(table);
    
    return 0;
}