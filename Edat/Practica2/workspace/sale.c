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
    /*SQLDATE start, end;
    SQLCHAR isbn[256];*/
    char isbn[500];
    
    int i;
    char buf[500];

    /* CONNECT */
    ret = odbc_connect(&env, &dbc);
    if (!SQL_SUCCEEDED(ret)) {
        printf("Error conecting");
        return EXIT_FAILURE;
    }

    /* Allocate a statement handle */
    ret = SQLAllocHandle(SQL_HANDLE_STMT, dbc, &stmt);
    if (!SQL_SUCCEEDED(ret)) {
        printf("Error allocating stmt");
                odbc_extract_error("SQLDriverConnect", dbc, SQL_HANDLE_DBC);

        return EXIT_FAILURE;
    }
   
   
   
   
    /*First query: add sale to sale table*/

    /*SQLPrepare(stmt, (SQLCHAR*) "INSERT INTO sale (startdate, enddate, discount) VALUES (?, ?, ?)", SQL_NTS);
    
    start = TO_DATE(argv[2], 'YYYY-MM-DD');
    end = TO_DATE(argv[3], 'YYYY-MM-DD');
    discount = atoi(argv[1]);
    
    SQLBindParameter(stmt, 1, SQL_PARAM_INPUT, SQL_C_TYPE_DATE, SQL_DATE, 0, 0, &start, 0, NULL);
    SQLBindParameter(stmt, 2, SQL_PARAM_INPUT, SQL_C_TYPE_DATE, SQL_DATE, 0, 0, &end, 0, NULL);
    SQLBindParameter(stmt, 3, SQL_PARAM_INPUT, SQL_C_SLONG, SQL_INTEGER, 0, 0, &discount, 0, NULL);
        
    SQLExecute(stmt);*/
    

    sprintf(buf, "INSERT INTO sale (start_date, end_date, discount) VALUES ('%s', '%s', %s)", argv[2], argv[3], argv[1]);
    
    ret = SQLExecDirect(stmt, (SQLCHAR*) buf, SQL_NTS);
    if (!SQL_SUCCEEDED(ret)) {
        printf("Error executing stmt");
        odbc_extract_error("SQLDriverConnect", dbc, SQL_HANDLE_DBC);
        return EXIT_FAILURE;
    }
    

    
    
    
    
    
    
    
    
    
    
    
    
    
    SQLCloseCursor(stmt);
    
    
    
    /*Second query: get saleid*/
    
    
    /*ret = SQLPrepare(stmt, (SQLCHAR*) "SELECT saleid FROM sale ORDER BY saleid DESC", SQL_NTS);
    if (!SQL_SUCCEEDED(ret)) {
        printf("Error preparing stmt");
        return EXIT_FAILURE;
    }
    
    ret = SQLExecute(stmt);
    if (!SQL_SUCCEEDED(ret)) {
        printf("Error executing stmt");
        return EXIT_FAILURE;
    }*/


    /*----------*/
    sprintf(buf, "SELECT MAX(saleid) FROM sale");
    
    ret = SQLExecDirect(stmt, (SQLCHAR*) buf, SQL_NTS);
    if (!SQL_SUCCEEDED(ret)) {
        printf("Error executing stmt");
        odbc_extract_error("SQLDriverConnect", dbc, SQL_HANDLE_DBC);
        return EXIT_FAILURE;
    }
    /*-------------*/

    
    /*if (!SQL_SUCCEEDED(ret)) {
        printf("Error getting data stmt 2");
        printf("%d", (int)saleid);
        odbc_extract_error("SQLDriverConnect", dbc, SQL_HANDLE_DBC);
        return EXIT_FAILURE;
    }*/
    if (SQL_SUCCEEDED(ret = SQLFetch(stmt))) {
        ret = SQLGetData(stmt, 1, SQL_C_SLONG, &saleid, sizeof(SQLINTEGER), NULL);
    }

    printf("\nsaleid: %d\n", saleid);

    
    
    SQLCloseCursor(stmt);
    
    
    
    /*Third query: add isbns to sale*/
    
    
    /*ret = SQLPrepare(stmt, (SQLCHAR*) "INSERT INTO sale_isbn (sale, isbn) VALUES (?, ?)", SQL_NTS);
    if (!SQL_SUCCEEDED(ret)) {
        printf("Error preparing stmt");
        return EXIT_FAILURE;
    }
    
    for (i = 4; i < argc; i++) {
        strcpy(isbn, argv[i]);
        
        ret = SQLBindParameter(stmt, 1, SQL_PARAM_INPUT, SQL_C_SLONG, SQL_INTEGER, 0, 0, &saleid, 0, NULL);
        if (!SQL_SUCCEEDED(ret)) {
            printf("Error with first parameter");
            return EXIT_FAILURE;
        }
        ret = SQLBindParameter(stmt, 2, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_CHAR, 0, 0, (SQLCHAR *)isbn, 0, NULL);
        if (!SQL_SUCCEEDED(ret)) {
            printf("Error with second parameter");
            return EXIT_FAILURE;
        }
        
        ret = SQLExecute(stmt);
        if (!SQL_SUCCEEDED(ret)) {
            printf("Error executing stmt 3");
            odbc_extract_error("SQLDriverConnect", dbc, SQL_HANDLE_DBC);
            return EXIT_FAILURE;
        }
    }*/
    
    for (i = 4; i < argc; i++) {
        sprintf(buf, "INSERT INTO sale_isbn (saleid, isbn) VALUES (%d, '%s')", saleid, argv[i]);
    
        ret = SQLExecDirect(stmt, (SQLCHAR*) buf, SQL_NTS);
        if (!SQL_SUCCEEDED(ret)) {
            printf("Error executing stmt\n");
            odbc_extract_error("SQLDriverConnect", dbc, SQL_HANDLE_DBC);
            return EXIT_FAILURE;
        }

        SQLCloseCursor(stmt);
    }
    
    



    /* free up statement handle */
    ret = SQLFreeHandle(SQL_HANDLE_STMT, stmt);
    if (!SQL_SUCCEEDED(ret)) {
        printf("Error freeing stmt");
        return EXIT_FAILURE;
    }

    /* DISCONNECT */
    ret = odbc_disconnect(env, dbc);
    if (!SQL_SUCCEEDED(ret)) {
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
