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
    SQLHENV env;
    SQLHDBC dbc;
    SQLHSTMT stmt;
    SQLRETURN ret;
    SQLCHAR isbn[STRMAX], title[STRMAX], author[STRMAX];
    table_t *table = NULL;
    void **values = NULL;
    
    
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
	
	while (1) {
        position = table_read_record(tabla, position);
        
        if (position == -1) {
            break;
        }
        
        for (int j = 0; j < table_ncols(tabla); j++) {
            values[j] = table_column_get(tabla, j);
        }
        
        if ((int *)values[2]==score){
             /* Allocate a statement handle */
            ret = SQLAllocHandle(SQL_HANDLE_STMT, dbc, &stmt);
            if (!SQL_SUCCEEDED(ret)) {
                printf("\nError allocating stmt\n");
                odbc_extract_error("SQLDriverConnect", dbc, SQL_HANDLE_DBC);
                return EXIT_FAILURE;
            }
            isbn = (char*) malloc(STRMAX * sizeof(char)); 
            strcpy (isbn, (char*)values[0]);
            
            /*Get the title*/

	        ret = SQLPrepare(stmt, (SQLCHAR *) "SELECT title FROM edition WHERE isbn = '?';", SQL_NTS);
            if (!SQL_SUCCEEDED(ret)) {
                printf("\nError preparing query for purchaseid in \"add\"\n");
                odbc_extract_error("SQLDriverConnect", dbc, SQL_HANDLE_DBC);
                return EXIT_FAILURE;
            }

            ret = SQLBindParameter(stmt, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_CHAR, 0, 0, isbn, 0, NULL);
            if (!SQL_SUCCEEDED(ret)) {
                printf("\nError binding parameter in query for isbn\n");
                odbc_extract_error("SQLDriverConnect", dbc, SQL_HANDLE_DBC);
                return EXIT_FAILURE;
            }

            ret = SQLExecute(stmt);
            if (!SQL_SUCCEEDED(ret)) {
                printf("\nError executing query for purchaseid in \"add\"\n");
                odbc_extract_error("SQLDriverConnect", dbc, SQL_HANDLE_DBC);
                return EXIT_FAILURE;
            }


            if (SQL_SUCCEEDED(ret = SQLFetch(stmt))) {
    	       ret = SQLGetData(stmt, 1, SQL_C_CHAR, title, sizeof(title), NULL);
            }
            
            
            ret = SQLCloseCursor(stmt);
            if (!SQL_SUCCEEDED(ret)) {
                printf("\nError closing cursor for argv[%d]\n", i);
                odbc_extract_error("SQLDriverConnect", dbc, SQL_HANDLE_DBC);
                return EXIT_FAILURE;
            }
            
            /*Get the author*/

	        ret = SQLPrepare(stmt, (SQLCHAR *) "SELECT author FROM edition WHERE isbn = '?';", SQL_NTS);
            if (!SQL_SUCCEEDED(ret)) {
                printf("\nError preparing query for purchaseid in \"add\"\n");
                odbc_extract_error("SQLDriverConnect", dbc, SQL_HANDLE_DBC);
                return EXIT_FAILURE;
            }

            ret = SQLBindParameter(stmt, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_CHAR, 0, 0, isbn, 0, NULL);
            if (!SQL_SUCCEEDED(ret)) {
                printf("\nError binding parameter in query for isbn\n");
                odbc_extract_error("SQLDriverConnect", dbc, SQL_HANDLE_DBC);
                return EXIT_FAILURE;
            }

            ret = SQLExecute(stmt);
            if (!SQL_SUCCEEDED(ret)) {
                printf("\nError executing query for purchaseid in \"add\"\n");
                odbc_extract_error("SQLDriverConnect", dbc, SQL_HANDLE_DBC);
                return EXIT_FAILURE;
            }


            if (SQL_SUCCEEDED(ret = SQLFetch(stmt))) {
    	       ret = SQLGetData(stmt, 1, SQL_C_CHAR, author, sizeof(author), NULL);
            }
            
            
            ret = SQLCloseCursor(stmt);
            if (!SQL_SUCCEEDED(ret)) {
                printf("\nError closing cursor for argv[%d]\n", i);
                odbc_extract_error("SQLDriverConnect", dbc, SQL_HANDLE_DBC);
                return EXIT_FAILURE;
            }
            
            printf("%s\t%s", (char *)author ,(char*)title);   
            
            free(isbn);
        
        }
    }
    
    
    return 0;
}