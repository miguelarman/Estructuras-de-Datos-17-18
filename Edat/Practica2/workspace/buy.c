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
    int i;
    SQLINTEGER purchaseid, userid, saleid, discount;
    double total;
    SQLREAL amount = 0.0;
    char buf[500];

    /* CONNECT */
    ret = odbc_connect(&env, &dbc);
    if (!SQL_SUCCEEDED(ret)) {
        return EXIT_FAILURE;
    }

    /* Allocate a statement handle */
    ret = SQLAllocHandle(SQL_HANDLE_STMT, dbc, &stmt);
    if (!SQL_SUCCEEDED(ret)) {
        printf("Error allocating stmt");
        return EXIT_FAILURE;
    }
    
    
    
    if (strcmp(argv[1], "add") == 0) {
        if (argc < 4){
            printf("Error with buy add. Must receive more parameters");
            return -1;
        }
        
        /*Get the last purchaseid*/

        ret = SQLPrepare(stmt, (SQLCHAR *) "SELECT max(purchaseid) FROM purchase", SQL_NTS);
        if (!SQL_SUCCEEDED(ret)) {
            printf("Error preparing stmt");
            return EXIT_FAILURE;
        }
    
        ret = SQLExecute(stmt);
        if (!SQL_SUCCEEDED(ret)) {
            printf("Error executing stmt 7");
            return EXIT_FAILURE;
        }
    

        if (SQL_SUCCEEDED(ret = SQLFetch(stmt))) {
            ret = SQLGetData(stmt, 1, SQL_C_SLONG, &purchaseid, sizeof(SQLINTEGER), NULL);
        }
    
        purchaseid++;

        printf("Invoice: %d\n", (int)purchaseid);
            
        /*Get userid*/

        ret = SQLCloseCursor(stmt);

        ret = SQLPrepare(stmt, (SQLCHAR*) "SELECT userid FROM users WHERE scrname = ?", SQL_NTS);
        if (!SQL_SUCCEEDED(ret)) {
            printf("Error preparing stmt");
            return EXIT_FAILURE;
        }
            
        ret = SQLBindParameter(stmt, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_CHAR, 0, 0, (SQLCHAR *)argv[2], 0, NULL);
        if (!SQL_SUCCEEDED(ret)) {
            printf("Error with parameter stmt");
            return EXIT_FAILURE;
        }
    
        ret = SQLExecute(stmt);
        if (!SQL_SUCCEEDED(ret)) {
            printf("Error executing stmt 1");
            return EXIT_FAILURE;
        }
    

        if (SQL_SUCCEEDED(ret = SQLFetch(stmt))) {
            ret = SQLGetData(stmt, 1, SQL_C_SLONG, &userid, sizeof(SQLINTEGER), NULL);
        }
        
        
        for (i=4; i < argc; i++){
            
            SQLCloseCursor(stmt);
            /*Get the amount*/
            /*ret = SQLPrepare(stmt, (SQLCHAR*) "SELECT amount FROM edition WHERE isbn = ?", SQL_NTS);
            if (!SQL_SUCCEEDED(ret)) {
                printf("Error preparing stmt");
                return EXIT_FAILURE;
            }
            
            ret = SQLBindParameter(stmt, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_CHAR, 0, 0, (SQLCHAR *)argv[i], 0, NULL);
            if (!SQL_SUCCEEDED(ret)) {
                printf("Error parameter stmt");
                return EXIT_FAILURE;
            }*/

            sprintf(buf, "SELECT prize FROM edition WHERE isbn = '%s'", argv[i]);
    
            ret = SQLExecDirect(stmt, (SQLCHAR*) buf, SQL_NTS);
            if (!SQL_SUCCEEDED(ret)) {
                printf("Error executing stmt 2");
                odbc_extract_error("SQLDriverConnect", dbc, SQL_HANDLE_DBC);
                return EXIT_FAILURE;
            }
            
            if (SQL_SUCCEEDED(ret = SQLFetch(stmt))) {
                ret = SQLGetData(stmt, 1, SQL_C_FLOAT, &amount, sizeof(SQLREAL), NULL);
            }
            

            SQLCloseCursor(stmt);
            /*Get saleid*/
            /*ret = SQLPrepare(stmt, (SQLCHAR*) "SELECT saleid FROM sale WHERE ISBN = ?", SQL_NTS);
            if (!SQL_SUCCEEDED(ret)) {
                printf("Error preparing stmt");
                return EXIT_FAILURE;
            }
            
            ret = SQLBindParameter(stmt, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_CHAR, 0, 0, (SQLCHAR *)argv[i], 0, NULL);
            if (!SQL_SUCCEEDED(ret)) {
                printf("Error parameter stmt");
                return EXIT_FAILURE;
            }*/

            sprintf(buf, "SELECT saleid FROM sale_isbn NATURAL JOIN sale WHERE isbn = '%s' AND '%s'>= start_date AND '%s'<= end_date", argv[i], argv[3], argv[3]);
    
            ret = SQLExecDirect(stmt, (SQLCHAR*) buf, SQL_NTS);
            if (!SQL_SUCCEEDED(ret)) {
                printf("Error executing stmt 3");
                odbc_extract_error("SQLDriverConnect", dbc, SQL_HANDLE_DBC);
                return EXIT_FAILURE;
            }
    
            if (SQL_SUCCEEDED(ret = SQLFetch(stmt))) {
                ret = SQLGetData(stmt, 1, SQL_C_SLONG, &saleid, sizeof(SQLINTEGER), NULL);
            }




            SQLCloseCursor(stmt);
            
            /*Get discount*/
            
            
            /*ret = SQLPrepare(stmt, (SQLCHAR*) "SELECT discount FROM sale WHERE saleid = ?", SQL_NTS);
            if (!SQL_SUCCEEDED(ret)) {
                printf("Error preparing stmt");
                return EXIT_FAILURE;
            }
            
            ret = SQLBindParameter(stmt, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_CHAR, 0, 0, &saleid, 0, NULL);
            if (!SQL_SUCCEEDED(ret)) {
                printf("Error parameter stmt");
                return EXIT_FAILURE;
            }*/

            sprintf(buf, "SELECT discount FROM sale WHERE saleid = %d", saleid);
    
            ret = SQLExecDirect(stmt, (SQLCHAR*) buf, SQL_NTS);
            if (!SQL_SUCCEEDED(ret)) {
                printf("Error executing stmt 4");
                return EXIT_FAILURE;
            }
    

            if (SQL_SUCCEEDED(ret = SQLFetch(stmt))) {
                ret = SQLGetData(stmt, 1, SQL_C_SLONG, &discount, sizeof(SQLINTEGER), NULL);
            }

            SQLCloseCursor(stmt);

            
            sprintf(buf, "INSERT INTO purchase (purchaseid, isbn, amount, saleid, userid, time) VALUES (%d, '%s', %d, %d, %d, '%s')", (int)purchaseid, argv[i], (int)amount, (int)saleid, (int)userid, argv[3]);
            ret = SQLExecDirect(stmt, (SQLCHAR*) buf, SQL_NTS);
            if (!SQL_SUCCEEDED(ret)) {
                printf("Error executing stmt 5");
                odbc_extract_error("SQLDriverConnect", dbc, SQL_HANDLE_DBC);
                return EXIT_FAILURE;
            }

            total += amount * ((100 - (int)discount)/100.0);
            
            SQLCloseCursor(stmt);

            /*Update sold_items field in edition*/
            
            sprintf(buf, "UPDATE edition SET sold_items = sold_items+1 WHERE isbn = '%s'", argv[i]);
            ret = SQLExecDirect(stmt, (SQLCHAR*) buf, SQL_NTS);
            if (!SQL_SUCCEEDED(ret)) {
                printf("Error executing stmt 5");
                odbc_extract_error("SQLDriverConnect", dbc, SQL_HANDLE_DBC);
                return EXIT_FAILURE;
            }
            
        }
        printf("Total: %f\n", total);
        
        
    } else if (strcmp(argv[1], "del") == 0) {
        if (argc < 3) {
            printf("Error with buy del. Must specify invoice_id to be deleted");
            return -1;
        }

        SQLCloseCursor(stmt);
        
        sprintf(buf, "DELETE FROM purchase WHERE purchaseid = %s", argv[2]);
        ret = SQLExecDirect(stmt, (SQLCHAR*) buf, SQL_NTS);
        if (!SQL_SUCCEEDED(ret)) {
            printf("Error executing stmt 6");
            odbc_extract_error("SQLDriverConnect", dbc, SQL_HANDLE_DBC);
            return EXIT_FAILURE;
        }

    }
    
    
    
    /* free up statement handle */
    SQLFreeHandle(SQL_HANDLE_STMT, stmt);
    
    /* DISCONNECT */
    ret = odbc_disconnect(env, dbc);
    if (!SQL_SUCCEEDED(ret)) {
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}