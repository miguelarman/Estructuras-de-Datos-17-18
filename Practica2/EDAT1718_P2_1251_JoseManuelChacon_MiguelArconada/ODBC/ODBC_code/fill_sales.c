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
        printf("\nError connecting\n");
        odbc_extract_error("SQLDriverConnect", dbc, SQL_HANDLE_DBC);
        return EXIT_FAILURE;
    }

    /* Allocate a statement handle */
    SQLAllocHandle(SQL_HANDLE_STMT, dbc, &stmt);
    if (!SQL_SUCCEEDED(ret)) {
        printf("\nError allocating stmt\n");
        odbc_extract_error("SQLDriverConnect", dbc, SQL_HANDLE_DBC);
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
    
    
    /*Create temporary table */
    
    sprintf(buf, "CREATE TABLE ventas_temp (purchaseid varchar(256), userid varchar(256), isbn varchar(256), time varchar(256), amount NUMERIC(10,2))");
    
    ret = SQLExecDirect(stmt, (SQLCHAR*) buf, SQL_NTS);
    if (!SQL_SUCCEEDED(ret)) {
        printf("\nError creating temporary table\n");
        odbc_extract_error("SQLDriverConnect", dbc, SQL_HANDLE_DBC);
        return EXIT_FAILURE;
    }

    ret = SQLCloseCursor(stmt);
    if (!SQL_SUCCEEDED(ret)) {
        printf("\nError closing first cursor\n");
        odbc_extract_error("SQLDriverConnect", dbc, SQL_HANDLE_DBC);
        return EXIT_FAILURE;
    }
    
    
    

    while (!feof(pf)) {
        fscanf(pf, "%s\t%s\t%s\t%s", purchaseid, userid, isbn, time);


        /*Select amount for each book*/

        sprintf(buf, "SELECT prize FROM edition WHERE isbn = '%s'", isbn);
    
        ret = SQLExecDirect(stmt, (SQLCHAR*) buf, SQL_NTS);
        if (!SQL_SUCCEEDED(ret)) {
            printf("Error executing executing query for amount in while loop");
            odbc_extract_error("SQLDriverConnect", dbc, SQL_HANDLE_DBC);
            return EXIT_FAILURE;
        }

        if (SQL_SUCCEEDED(ret = SQLFetch(stmt))) {
            ret = SQLGetData(stmt, 1, SQL_C_FLOAT, &amount, sizeof(SQLREAL), NULL);
        }

        ret = SQLCloseCursor(stmt);
        if (!SQL_SUCCEEDED(ret)) {
            printf("\nError closing second cursor in while loop\n");
            odbc_extract_error("SQLDriverConnect", dbc, SQL_HANDLE_DBC);
            return EXIT_FAILURE;
        }
        
        
        
        /*Insert into temporary table all the values*/

        sprintf(buf, "INSERT INTO ventas_temp (purchaseid, isbn, amount, userid, time) VALUES (%s, '%s', %d, %s, '%s')", purchaseid, isbn, (int) amount, userid, time);
        ret = SQLExecDirect(stmt, (SQLCHAR*) buf, SQL_NTS);
        if (!SQL_SUCCEEDED(ret)) {
            printf("\nError inserting into temporary table in while loop\n");
            odbc_extract_error("SQLDriverConnect", dbc, SQL_HANDLE_DBC);
            return EXIT_FAILURE;
        }

        ret = SQLCloseCursor(stmt);
        if (!SQL_SUCCEEDED(ret)) {
            printf("\nError closing third cursor in while loop\n");
            odbc_extract_error("SQLDriverConnect", dbc, SQL_HANDLE_DBC);
            return EXIT_FAILURE;
        }
    }

    
    /*Insert into table with casts*/
    
    sprintf(buf, "INSERT INTO purchase (purchaseid, isbn, amount, userid, time) SELECT CAST(purchaseid AS INTEGER), isbn, CAST(amount AS NUMERIC(10,2)), CAST(userid AS INTEGER), CAST(time AS DATE) FROM ventas_temp WHERE isbn IN (SELECT isbn FROM edition)");
    ret = SQLExecDirect(stmt, (SQLCHAR*) buf, SQL_NTS);
    if (!SQL_SUCCEEDED(ret)) {
        printf("\nError inserting into table with casts\n");
        odbc_extract_error("SQLDriverConnect", dbc, SQL_HANDLE_DBC);
        return EXIT_FAILURE;
    }

    ret = SQLCloseCursor(stmt);
    if (!SQL_SUCCEEDED(ret)) {
        printf("\nError closing fourth cursor\n");
        odbc_extract_error("SQLDriverConnect", dbc, SQL_HANDLE_DBC);
        return EXIT_FAILURE;
    }


    /*Drop temporary table*/

    sprintf(buf, "DROP TABLE ventas_temp");
    
    ret = SQLExecDirect(stmt, (SQLCHAR*) buf, SQL_NTS);
    if (!SQL_SUCCEEDED(ret)) {
        printf("\nError droping temporary table\n");
        odbc_extract_error("SQLDriverConnect", dbc, SQL_HANDLE_DBC);
        return EXIT_FAILURE;
    }






    /* free up statement handle */
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