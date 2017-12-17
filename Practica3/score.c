#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sql.h>
#include <sqlext.h>
#include "odbc.h"
#include "table.h"


#define PATH "book_table.txt"
#define STRMAX 128



int main (int argc, char **argv) {

	table_t *table = NULL;
	void **values = NULL;

	SQLHENV env;
    SQLHDBC dbc;
    SQLHSTMT stmt;
    SQLRETURN ret; /* ODBC API return status */
    SQLCHAR isbn[64];



    /*Parameter checks*/

	if (argc < 3) {
		printf("\nError with parameters\n");
		return -1;
	}




    /* CONNECT */
    ret = odbc_connect(&env, &dbc);
    if (!SQL_SUCCEEDED(ret)) {
        printf("\nError connecting\n");
        odbc_extract_error("SQLDriverConnect", dbc, SQL_HANDLE_DBC);
        return EXIT_FAILURE;
    }

    /* Allocate a statement handle */
    ret = SQLAllocHandle(SQL_HANDLE_STMT, dbc, &stmt);
    if (!SQL_SUCCEEDED(ret)) {
        printf("\nError allocating stmt\n");
        odbc_extract_error("SQLDriverConnect", dbc, SQL_HANDLE_DBC);
        return EXIT_FAILURE;
    }





	/*Get the ISBN*/

	ret = SQLPrepare(stmt, (SQLCHAR *) "SELECT isbn FROM edition WHERE title = '?';", SQL_NTS);
    if (!SQL_SUCCEEDED(ret)) {
        printf("\nError preparing query for purchaseid in \"add\"\n");
        odbc_extract_error("SQLDriverConnect", dbc, SQL_HANDLE_DBC);
        return EXIT_FAILURE;
    }

    ret = SQLBindParameter(stmt, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_CHAR, 0, 0, (SQLCHAR *)argv[1], 0, NULL);
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
    	ret = SQLGetData(stmt, 1, SQL_C_CHAR, isbn, sizeof(isbn), NULL);
    }
    
                    printf("%s", (char *)isbn);


    /* free up statement handle */
    SQLFreeHandle(SQL_HANDLE_STMT, stmt);

    /* DISCONNECT */
    ret = odbc_disconnect(env, dbc);
    if (!SQL_SUCCEEDED(ret)) {
        return EXIT_FAILURE;
    }



    /*Open the table to store the values*/

	table = table_open(PATH);
	if (table == NULL) {
		printf("\nError opening the table\n");
		return -1;
	}


	/*Store the values*/

    values = (void **)malloc(4 * sizeof(void *));
    if (values == NULL) {
    	printf("\nError allocating values\n");
    	table_close(table);
    	return -1;
    }

	values[0] = (char *)malloc(STRMAX * sizeof(char));
	if (values[0] == NULL) {
    	printf("\nError allocating values[0]\n");
    	free(values);
    	table_close(table);
    	return -1;
    }

	values[1] = (char *)malloc(STRMAX * sizeof(char));
	if (values[1] == NULL) {
    	printf("\nError allocating values[1]\n");
    	free(values[0]);
    	free(values);
    	table_close(table);
    	return -1;
    }

    values[2] = (int *)malloc(sizeof(int));
    if (values[2] == NULL) {
    	printf("\nError allocating values[2]\n");
    	free(values[0]);
    	free(values[1]);
    	free(values);
    	table_close(table);
    	return -1;
    }

    values[3] = (int *)malloc(sizeof(int));
    if (values[3] == NULL) {
    	printf("\nError allocating values[3]\n");
    	free(values[0]);
    	free(values[1]);
    	free(values[2]);
    	free(values);
    	table_close(table);
    	return -1;
    }
    

    strcpy((char *) values[0], (char *)isbn);
    strcpy((char *) values[1], argv[1]);
    *((int *) values[2]) = atoi(argv[2]);
    *((int *) values[3]) = 0;


    
    table_insert_record(table, values);
    





    /*Free auxiliary array*/

    if (values != NULL) {
    	for (int i = 0; i < 4; i++) {
	    	if (values[i] != NULL) {
	    		free (values[i]);
	    	}
	    }
    	free(values);
    }

    
    table_close(table);

    
    return 0;
}