#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sql.h>
#include <sqlext.h>
#include "odbc.h"

/*
 Program that generates purchases
 */

int buy(int argc, char **argv) {
    
    SQLHENV env;
    SQLHDBC dbc;
    SQLHSTMT stmt1, stmt2;
    SQLRETURN ret; /* ODBC API return status */
    SQLINTEGER ret1;
    SQLCHAR y[512];

    /* CONNECT */
    ret = odbc_connect(&env, &dbc);
    if (!SQL_SUCCEEDED(ret)) {
        return EXIT_FAILURE;
    }

    /*Insertar valores de compras*/
    if (argv[1]=="add"){
       
        /* First query: get the purchaseid */
        SQLAllocHandle(SQL_HANDLE_STMT, dbc, &stmt1);
        SQLExecDirect(stmt, (SQLCHAR*) "SELECT purchaseid FROM purchase ORDER BY purchaseid ASC LIMIT 1", SQL_NTS);
        SQLBindCol(stmt, 1, SQL_C_SLONG, &ret, sizeof(SQLINTEGER), NULL);
        
        /* free up statement handle */
        SQLFreeHandle(SQL_HANDLE_STMT, stmt1);
    }
    
    if (argv[1]=="del"){
        /* Allocate a statement handle */
        SQLAllocHandle(SQL_HANDLE_STMT, dbc, &stmt2);
        
        SQLExecDirect(stmt, (SQLCHAR*) "SELECT purchaseid FROM purchase ORDER BY purchaseid ASC LIMIT 1", SQL_NTS);
        
        /* free up statement handle*/
        SQLFreeHandle(SQL_HANDLE_STMT, stmt2);
    }
    
    
    ret = odbc_disconnect(env, dbc);
    if (!SQL_SUCCEEDED(ret)) {
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}