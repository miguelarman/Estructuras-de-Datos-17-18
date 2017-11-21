#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sql.h>
#include <sqlext.h>
#include "odbc.h"


int main (int argc, char ** argv) {

    SQLHENV env;
    SQLHDBC dbc;
    SQLHSTMT stmt;
    SQLRETURN ret; /* ODBC API return status */
    SQLREAL amount;
    


    char purchaseid[256], isbn[256], userid[256], time[256], buf[500];
    FILE *pf = NULL;

    /* CONNECT */
    ret = odbc_connect(&env, &dbc);
    if (!SQL_SUCCEEDED(ret)) {
        return EXIT_FAILURE;
    }

    /* Allocate a statement handle */
    SQLAllocHandle(SQL_HANDLE_STMT, dbc, &stmt);
    if (!SQL_SUCCEEDED(ret)) {
        printf("Error allocating stmt");
        return EXIT_FAILURE;
    }

    if (argc < 2) {
        printf("Must specify input_file");
        return -1;
    }

    pf = fopen (argv[1], "r");
    if (pf == NULL) {
        printf("Error opening file");
        return -1;
    }

    sprintf(buf, "CREATE TABLE ventas_temp (purchaseid varchar(256), userid varchar(256), isbn varchar(256), time varchar(256), amount NUMERIC(10,2))");
    
    ret = SQLExecDirect(stmt, (SQLCHAR*) buf, SQL_NTS);
    if (!SQL_SUCCEEDED(ret)) {
        printf("Error executing stmt 4");
        odbc_extract_error("SQLDriverConnect", dbc, SQL_HANDLE_DBC);
        return EXIT_FAILURE;
    }

    SQLCloseCursor(stmt);

    while (!feof(pf)) {
        fscanf(pf, "%s\t%s\t%s\t%s", purchaseid, userid, isbn, time);

        /*ret = SQLPrepare(stmt, (SQLCHAR*) "SELECT prize FROM edition WHERE isbn = ?", SQL_NTS);
        if (!SQL_SUCCEEDED(ret)) {
            printf("Error preparing stmt");
            return EXIT_FAILURE;
        }
        
        ret = SQLBindParameter(stmt, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_CHAR, 0, 0, (SQLCHAR *) isbn, 0, NULL);
        if (!SQL_SUCCEEDED(ret)) {
            printf("Error with parameter isbn");
            return EXIT_FAILURE;
        }

        ret = SQLExecute(stmt);
        if (!SQL_SUCCEEDED(ret)) {
            printf("Error eecuting stmt");
            return EXIT_FAILURE;
        }*/

        sprintf(buf, "SELECT prize FROM edition WHERE isbn = '%s'", isbn);
    
        ret = SQLExecDirect(stmt, (SQLCHAR*) buf, SQL_NTS);
        if (!SQL_SUCCEEDED(ret)) {
            printf("Error executing stmt 3");
            odbc_extract_error("SQLDriverConnect", dbc, SQL_HANDLE_DBC);
            return EXIT_FAILURE;
        }

        if (SQL_SUCCEEDED(ret = SQLFetch(stmt))) {
            ret = SQLGetData(stmt, 1, SQL_C_FLOAT, &amount, sizeof(SQLREAL), NULL);
        }
        /*if (!SQL_SUCCEEDED(ret)) {
            printf("Error getting data from stmt");
            return EXIT_FAILURE;
        }*/

        SQLCloseCursor(stmt);

        sprintf(buf, "INSERT INTO ventas_temp (purchaseid, isbn, amount, userid, time) VALUES (%s, '%s', %d, %s, '%s')", purchaseid, isbn, (int) amount, userid, time);
        ret = SQLExecDirect(stmt, (SQLCHAR*) buf, SQL_NTS);
        if (!SQL_SUCCEEDED(ret)) {
            printf("Error executing stmt 1\n");
            odbc_extract_error("SQLDriverConnect", dbc, SQL_HANDLE_DBC);
            return EXIT_FAILURE;
        }

        SQLCloseCursor(stmt);
    }


    sprintf(buf, "INSERT INTO purchase (purchaseid, isbn, amount, userid, time) SELECT CAST(purchaseid AS INTEGER), isbn, CAST(amount AS NUMERIC(10,2)), CAST(userid AS INTEGER), CAST(time AS DATE) FROM ventas_temp WHERE isbn IN (SELECT isbn FROM edition)");
    ret = SQLExecDirect(stmt, (SQLCHAR*) buf, SQL_NTS);
    if (!SQL_SUCCEEDED(ret)) {
        printf("Error executing stmt 2\n");
        odbc_extract_error("SQLDriverConnect", dbc, SQL_HANDLE_DBC);
        return EXIT_FAILURE;
    }



    SQLCloseCursor(stmt);



    sprintf(buf, "DROP TABLE ventas_temp");
    
    ret = SQLExecDirect(stmt, (SQLCHAR*) buf, SQL_NTS);
    if (!SQL_SUCCEEDED(ret)) {
        printf("Error executing stmt 7");
        odbc_extract_error("SQLDriverConnect", dbc, SQL_HANDLE_DBC);
        return EXIT_FAILURE;
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
        printf("Error disconnecting");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}