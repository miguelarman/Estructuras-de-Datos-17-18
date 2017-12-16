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
    
    
    
    if (strcmp(argv[1], "add") == 0) {
        if (argc < 4){
            printf("Error with buy add. Must receive more parameters");
            return -1;
        }
        
        /*Get the last purchaseid*/

        ret = SQLPrepare(stmt, (SQLCHAR *) "SELECT max(purchaseid) FROM purchase", SQL_NTS);
        if (!SQL_SUCCEEDED(ret)) {
            printf("\nError preparing query for purchaseid in \"add\"\n");
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
            ret = SQLGetData(stmt, 1, SQL_C_SLONG, &purchaseid, sizeof(SQLINTEGER), NULL);
        }
    
        purchaseid++;

        printf("Invoice: %d\n", (int)purchaseid);
            
        ret = SQLCloseCursor(stmt);
        if (!SQL_SUCCEEDED(ret)) {
            printf("\nError closing first cursor\n");
            odbc_extract_error("SQLDriverConnect", dbc, SQL_HANDLE_DBC);
            return EXIT_FAILURE;
        }
            
        
        
        
        /*Get userid*/

        ret = SQLPrepare(stmt, (SQLCHAR*) "SELECT userid FROM users WHERE scrname = ?", SQL_NTS);
        if (!SQL_SUCCEEDED(ret)) {
            printf("\nError preparing query for userid in \"add\"\n");
            odbc_extract_error("SQLDriverConnect", dbc, SQL_HANDLE_DBC);
            return EXIT_FAILURE;
        }
            
        ret = SQLBindParameter(stmt, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_CHAR, 0, 0, (SQLCHAR *)argv[2], 0, NULL);
        if (!SQL_SUCCEEDED(ret)) {
            printf("\nError binding parameter in query for userid\n");
            odbc_extract_error("SQLDriverConnect", dbc, SQL_HANDLE_DBC);
            return EXIT_FAILURE;
        }
    
        ret = SQLExecute(stmt);
        if (!SQL_SUCCEEDED(ret)) {
            printf("\nError executing quuery for userid in \"add\"\n");
            odbc_extract_error("SQLDriverConnect", dbc, SQL_HANDLE_DBC);
            return EXIT_FAILURE;
        }
    

        if (SQL_SUCCEEDED(ret = SQLFetch(stmt))) {
            ret = SQLGetData(stmt, 1, SQL_C_SLONG, &userid, sizeof(SQLINTEGER), NULL);
        }
        
        ret = SQLCloseCursor(stmt);
        if (!SQL_SUCCEEDED(ret)) {
            printf("\nError closing second cursor\n");
            odbc_extract_error("SQLDriverConnect", dbc, SQL_HANDLE_DBC);
            return EXIT_FAILURE;
        }

        
        for (i=4; i < argc; i++){
            
            /*Get the amount*/

            sprintf(buf, "SELECT prize FROM edition WHERE isbn = '%s'", argv[i]);
    
            ret = SQLExecDirect(stmt, (SQLCHAR*) buf, SQL_NTS);
            if (!SQL_SUCCEEDED(ret)) {
                printf("\nError executing query for amount for argv[%d]\n", i);
                odbc_extract_error("SQLDriverConnect", dbc, SQL_HANDLE_DBC);
                return EXIT_FAILURE;
            }
            
            if (SQL_SUCCEEDED(ret = SQLFetch(stmt))) {
                ret = SQLGetData(stmt, 1, SQL_C_FLOAT, &amount, sizeof(SQLREAL), NULL);
            }
            

            ret = SQLCloseCursor(stmt);
            if (!SQL_SUCCEEDED(ret)) {
                printf("\nError closing third cursor for argv[%d]\n", i);
                odbc_extract_error("SQLDriverConnect", dbc, SQL_HANDLE_DBC);
                return EXIT_FAILURE;
            }
            
            
            
            /*Get saleid*/

            sprintf(buf, "SELECT saleid FROM sale_isbn NATURAL JOIN sale WHERE isbn = '%s' AND '%s'>= start_date AND '%s'<= end_date", argv[i], argv[3], argv[3]);
    
            ret = SQLExecDirect(stmt, (SQLCHAR*) buf, SQL_NTS);
            if (!SQL_SUCCEEDED(ret)) {
                printf("\nError executing query for saleid for argv[%d]\n", i);
                odbc_extract_error("SQLDriverConnect", dbc, SQL_HANDLE_DBC);
                return EXIT_FAILURE;
            }
    
            if (SQL_SUCCEEDED(ret = SQLFetch(stmt))) {
                ret = SQLGetData(stmt, 1, SQL_C_SLONG, &saleid, sizeof(SQLINTEGER), NULL);
            }


            ret = SQLCloseCursor(stmt);
            if (!SQL_SUCCEEDED(ret)) {
                printf("\nError closing fourth cursor for argv[%d]\n", i);
                odbc_extract_error("SQLDriverConnect", dbc, SQL_HANDLE_DBC);
                return EXIT_FAILURE;
            }
            


            /*Get discount*/

            sprintf(buf, "SELECT discount FROM sale WHERE saleid = %d", saleid);
    
            ret = SQLExecDirect(stmt, (SQLCHAR*) buf, SQL_NTS);
            if (!SQL_SUCCEEDED(ret)) {
                printf("\nError executing query for discount with argv[%d]\n", i);
                odbc_extract_error("SQLDriverConnect", dbc, SQL_HANDLE_DBC);
                return EXIT_FAILURE;
            }
    

            if (SQL_SUCCEEDED(ret = SQLFetch(stmt))) {
                ret = SQLGetData(stmt, 1, SQL_C_SLONG, &discount, sizeof(SQLINTEGER), NULL);
            }

            ret = SQLCloseCursor(stmt);
            if (!SQL_SUCCEEDED(ret)) {
                printf("\nError closing fifth cursor for argv[%d]\n", i);
                odbc_extract_error("SQLDriverConnect", dbc, SQL_HANDLE_DBC);
                return EXIT_FAILURE;
            }
            
            
            
            
            /*Insert into table*/
            
            sprintf(buf, "INSERT INTO purchase (purchaseid, isbn, amount, saleid, userid, time) VALUES (%d, '%s', %d, %d, %d, '%s')", (int)purchaseid, argv[i], (int)amount, (int)saleid, (int)userid, argv[3]);
            ret = SQLExecDirect(stmt, (SQLCHAR*) buf, SQL_NTS);
            if (!SQL_SUCCEEDED(ret)) {
                printf("\nError executing query for inserting into purchases for argv[%d]");
                odbc_extract_error("SQLDriverConnect", dbc, SQL_HANDLE_DBC);
                return EXIT_FAILURE;
            }

            total += amount * ((100 - (int)discount)/100.0);
            
            ret = SQLCloseCursor(stmt);
            if (!SQL_SUCCEEDED(ret)) {
                printf("\nError closing sixth cursor for argv[%d]\n", i);
                odbc_extract_error("SQLDriverConnect", dbc, SQL_HANDLE_DBC);
                return EXIT_FAILURE;
            }
            
            
            

            /*Update sold_items field in edition*/
            
            sprintf(buf, "UPDATE edition SET sold_items = sold_items+1 WHERE isbn = '%s'", argv[i]);
            ret = SQLExecDirect(stmt, (SQLCHAR*) buf, SQL_NTS);
            if (!SQL_SUCCEEDED(ret)) {
                printf("\nError executing query for updating sold_items for argv[%d]", i);
                odbc_extract_error("SQLDriverConnect", dbc, SQL_HANDLE_DBC);
                return EXIT_FAILURE;
            }
            
            
            ret = SQLCloseCursor(stmt);
            if (!SQL_SUCCEEDED(ret)) {
                printf("\nError closing seventh cursor for argv[%d]\n", i);
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
        
        /*Delete the purchase*/
        
        sprintf(buf, "DELETE FROM purchase WHERE purchaseid = %s", argv[2]);
        ret = SQLExecDirect(stmt, (SQLCHAR*) buf, SQL_NTS);
        if (!SQL_SUCCEEDED(ret)) {
            printf("\nError executing query for delete");
            odbc_extract_error("SQLDriverConnect", dbc, SQL_HANDLE_DBC);
            return EXIT_FAILURE;
        }

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