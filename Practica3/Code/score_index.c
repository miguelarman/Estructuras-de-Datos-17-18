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
#define STRMAX 128



int main (int argc, char **argv) {

	table_t *table = NULL;
	void **values = NULL;
    index_t *index = NULL;
    long position;
    int return_aux;

	SQLHENV env;
    SQLHDBC dbc;
    SQLHSTMT stmt;
    SQLRETURN ret; /* ODBC API return status */
    SQLCHAR isbn[64];
    
    char buf[500];



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

	/*ret = SQLPrepare(stmt, (SQLCHAR *) "SELECT isbn FROM edition WHERE title = '?'", SQL_NTS);
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
    }*/
    
    sprintf(buf, "SELECT isbn FROM edition WHERE title = '%s';", argv[1]);
    ret = SQLExecDirect(stmt, (SQLCHAR*) buf, SQL_NTS);
    if (!SQL_SUCCEEDED(ret)) {
        printf("\nError executing query");
        odbc_extract_error("SQLDriverConnect", dbc, SQL_HANDLE_DBC);
        return EXIT_FAILURE;
    }
    
    


    if (SQL_SUCCEEDED(ret = SQLFetch(stmt))) {
    	ret = SQLGetData(stmt, 1, SQL_C_CHAR, isbn, sizeof(isbn), NULL);
    }




    /*Open the table to store the values*/

	table = table_open(PATH_TABLE);
	if (table == NULL) {
		printf("\nError opening the table\n");
		return -1;
	}

    index = index_open(PATH_INDEX);
    if (index == NULL) {
        table_close(table);
        printf("\nError opening index\n");
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


    position = table_last_pos(table);
    table_insert_record(table, values);
    
    index_put(index, *((int *) values[2]), position);




    /*Free auxiliary array*/

    if (values != NULL) {
    	for (int i = 0; i < 4; i++) {
	    	if (values[i] != NULL) {
	    		free (values[i]);
	    	}
	    }
    	free(values);
    }
    
    /* free up statement handle */
    SQLFreeHandle(SQL_HANDLE_STMT, stmt);

    /* DISCONNECT */
    ret = odbc_disconnect(env, dbc);
    if (!SQL_SUCCEEDED(ret)) {
        return EXIT_FAILURE;
    }

    
    table_close(table);

    return_aux = index_save(index);
    if (return_aux == -1) {
        printf("\nError saving index\n");
        return -1;
    }

    index_close(index);

    
    return 0;
}