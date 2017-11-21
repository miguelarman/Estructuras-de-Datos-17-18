#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sql.h>
#include <sqlext.h>
#include "odbc.h"


int main (int argc, char **argv) {
    SQLHENV env;
    SQLHDBC dbc;
    SQLHSTMT stmt;
    SQLRETURN ret; /* ODBC API return status */
    SQLINTEGER saleid;
    char isbn[500];
    
    int i;
    char buf[500];

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
   
   
   
   
    /*First query: add sale to sale table*/

    sprintf(buf, "INSERT INTO sale (start_date, end_date, discount) VALUES ('%s', '%s', %s)", argv[2], argv[3], argv[1]);
    
    ret = SQLExecDirect(stmt, (SQLCHAR*) buf, SQL_NTS);
    if (!SQL_SUCCEEDED(ret)) {
        printf("\nError executing first query\n");
        odbc_extract_error("SQLDriverConnect", dbc, SQL_HANDLE_DBC);
        return EXIT_FAILURE;
    }
    
    ret = SQLCloseCursor(stmt);
    if (!SQL_SUCCEEDED(ret)) {
        printf("\nError closing first cursor\n");
        odbc_extract_error("SQLDriverConnect", dbc, SQL_HANDLE_DBC);
        return EXIT_FAILURE;
    }
    
    
    /*Second query get saleid*/
    sprintf(buf, "SELECT MAX(saleid) FROM sale");
    
    ret = SQLExecDirect(stmt, (SQLCHAR*) buf, SQL_NTS);
    if (!SQL_SUCCEEDED(ret)) {
        printf("\n Error executing second query\n");
        odbc_extract_error("SQLDriverConnect", dbc, SQL_HANDLE_DBC);
        return EXIT_FAILURE;
    }
   
    if (SQL_SUCCEEDED(ret = SQLFetch(stmt))) {
        ret = SQLGetData(stmt, 1, SQL_C_SLONG, &saleid, sizeof(SQLINTEGER), NULL);
    }

    
    
    ret = SQLCloseCursor(stmt);
    if (!SQL_SUCCEEDED(ret)) {
        printf("\nError closing second cursor\n");
        odbc_extract_error("SQLDriverConnect", dbc, SQL_HANDLE_DBC);
        return EXIT_FAILURE;
    }
    
    
    
    /*Third query: add isbns to sale*/
    
    for (i = 4; i < argc; i++) {
        sprintf(buf, "INSERT INTO sale_isbn (saleid, isbn) VALUES (%d, '%s')", saleid, argv[i]);
    
        ret = SQLExecDirect(stmt, (SQLCHAR*) buf, SQL_NTS);
        if (!SQL_SUCCEEDED(ret)) {
            printf("\nError executing third query for parameter argv[%d]\n", i);
            odbc_extract_error("SQLDriverConnect", dbc, SQL_HANDLE_DBC);
            return EXIT_FAILURE;
        }

        ret = SQLCloseCursor(stmt);
        if (!SQL_SUCCEEDED(ret)) {
            printf("|n Error closing third cursor for parameter argv[%d]\n", i);
            odbc_extract_error("SQLDriverConnect", dbc, SQL_HANDLE_DBC);
            return EXIT_FAILURE;
        }
    }
    
    



    /* Free up statement handle */
    ret = SQLFreeHandle(SQL_HANDLE_STMT, stmt);
    if (!SQL_SUCCEEDED(ret)) {
        printf("\nError freeing stmt\n");
        odbc_extract_error("SQLDriverConnect", dbc, SQL_HANDLE_DBC);
        return EXIT_FAILURE;
    }

    /* DISCONNECT */
    ret = odbc_disconnect(env, dbc);
    if (!SQL_SUCCEEDED(ret)) {
        printf("\nError disconnecting\n");
        odbc_extract_error("SQLDriverConnect", dbc, SQL_HANDLE_DBC);
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
