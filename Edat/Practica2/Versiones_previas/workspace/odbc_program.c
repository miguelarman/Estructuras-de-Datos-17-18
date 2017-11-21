#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sql.h>
#include <sqlext.h>
#include "odbc.h"

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/
/*IMPORTANTE: NO MODIFIQUES NADA EN ESTE FICHERO, SINO EN LOS OTROS QUE TIENEN LAS FUNCIONES YA ESCRITAS ALLÍ*/
/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

int sale (int argc, char ** argv);
int buy (int argc, char ** argv);
int fill_sales (int argc, char ** argv) ;

int main (int argc, char **argv) {
    
    if (strcmp(argv[0], "sale") == 0) {
        sale (argc, argv);
        return 0;
    } else if (strcmp(argv[0], "buy") == 0) {
        buy (argc, argv);
        return 0;
    } else if (strcmp(argv[0], "fill_sales") == 0) {
        buy (argc, argv);
        return 0;
    }

    return -1;
}

int sale (int argc, char **argv) {
    SQLHENV env;
    SQLHDBC dbc;
    SQLHSTMT stmt1, stmt2, stmt3;
    SQLRETURN ret; /* ODBC API return status */
    SQLINTEGER saleid;
    /*SQLDATE start, end;
    SQLCHAR isbn[256];*/
    char *isbn;
    
    int i;
    char buf[500];

    /* CONNECT */
    ret = odbc_connect(&env, &dbc);
    if (!SQL_SUCCEEDED(ret)) {
        return EXIT_FAILURE;
    }

    /* Allocate a statement handle */
    SQLAllocHandle(SQL_HANDLE_STMT, dbc, &stmt1);
    SQLAllocHandle(SQL_HANDLE_STMT, dbc, &stmt2);
    SQLAllocHandle(SQL_HANDLE_STMT, dbc, &stmt3);
   
   
   
   
   
   
    /*First query: add sale to sale table*/

    /*SQLPrepare(stmt1, (SQLCHAR*) "INSERT INTO sale (startdate, enddate, discount) VALUES (?, ?, ?)", SQL_NTS);
    
    start = TO_DATE(argv[2], 'YYYY-MM-DD');
    end = TO_DATE(argv[3], 'YYYY-MM-DD');
    discount = atoi(argv[1]);
    
    SQLBindParameter(stmt1, 1, SQL_PARAM_INPUT, SQL_C_TYPE_DATE, SQL_DATE, 0, 0, &start, 0, NULL);
    SQLBindParameter(stmt1, 2, SQL_PARAM_INPUT, SQL_C_TYPE_DATE, SQL_DATE, 0, 0, &end, 0, NULL);
    SQLBindParameter(stmt1, 3, SQL_PARAM_INPUT, SQL_C_SLONG, SQL_INTEGER, 0, 0, &discount, 0, NULL);
        
    SQLExecute(stmt1);*/
    

    sprintf(buf, "INSERT INTO sale (startdate, enddate, discount) VALUES (%s, %s, %s)", argv[2], argv[3], argv[1]);
    
    SQLExecDirect(stmt1, (SQLCHAR*) buf, SQL_NTS);
    
    
    
    
    
    
    /*Second query: get saleid*/
    
    
    SQLPrepare(stmt2, (SQLCHAR*) "SELECT max(saleid) FROM sale", SQL_NTS);
    
    SQLExecute(stmt2);
    
    ret = SQLGetData(stmt2, 1, SQL_C_SLONG, &saleid, sizeof(SQLINTEGER), NULL);
    
    
    
    
    
    
    
    /*Third query: add isbns to sale*/
    
    
    SQLPrepare(stmt3, (SQLCHAR*) "INSERT INTO sale_isbn (sale, isbn) VALUES (?, ?)", SQL_NTS);
    
    for (i = 4; i < argc; i++) {
        strcpy(isbn, argv[i]);
        
        SQLBindParameter(stmt3, 1, SQL_PARAM_INPUT, SQL_C_SLONG, SQL_INTEGER, 0, 0, &saleid, 0, NULL);
        SQLBindParameter(stmt3, 2, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_CHAR, 0, 0, (SQLCHAR *)isbn, 0, NULL);
        
        SQLExecute(stmt3);
    }
    
    /*for (i = 4; i < argc; i++) {
        sprintf(buf, "INSERT INTO sale_isbn (sale, isbn) VALUES (%d, %s)", saleid, argv[i]);
    
        SQLExecDirect(stmt3, (SQLCHAR*) buf, SQL_NTS);
    }
    */
    



    /* free up statement handle */
    SQLFreeHandle(SQL_HANDLE_STMT, stmt1);
    SQLFreeHandle(SQL_HANDLE_STMT, stmt2);
    SQLFreeHandle(SQL_HANDLE_STMT, stmt3);

    /* DISCONNECT */
    ret = odbc_disconnect(env, dbc);
    if (!SQL_SUCCEEDED(ret)) {
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}


/*buy*/

int buy (int argc, char ** argv) {
    
    SQLHENV env;
    SQLHDBC dbc;
    SQLHSTMT stmt_del, stmt1, stmt2;
    SQLRETURN ret; /* ODBC API return status */
    int i;
    SQLINTEGER purchaseid, amount;
    
    char buf[500];

    /* CONNECT */
    ret = odbc_connect(&env, &dbc);
    if (!SQL_SUCCEEDED(ret)) {
        return EXIT_FAILURE;
    }

    /* Allocate a statement handle */
    SQLAllocHandle(SQL_HANDLE_STMT, dbc, &stmt_del);
    SQLAllocHandle(SQL_HANDLE_STMT, dbc, &stmt1);
    SQLAllocHandle(SQL_HANDLE_STMT, dbc, &stmt2);
    SQLAllocHandle(SQL_HANDLE_STMT, dbc, &stmt3);
    SQLAllocHandle(SQL_HANDLE_STMT, dbc, &stmt4);
    
    
    
    if (strcmp(argv[1], "add") == 0) {
        if (argc<4){
            printf("Error with buy add. Must receive more parameters");
            return -1;
        }
        
        SQLPrepare(stmt1, (SQLCHAR*) "SELECT max(purchaseid) FROM purchase", SQL_NTS);
    
        SQLExecute(stmt1);
    
        ret = SQLGetData(stmt1, 1, SQL_C_SLONG, &purchaseid, sizeof(SQLINTEGER), NULL);
        
        for (i=3; i < argc; i++){
            /*Get the amount*/
            SQLPrepare(stmt3, (SQLCHAR*) "SELECT amount FROM edition WHERE isbn = ?", SQL_NTS);
            
            SQLBindParameter(stmt3, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_CHAR, 0, 0, (SQLCHAR *)argv[i], 0, NULL);
    
            SQLExecute(stmt3);
    
            ret = SQLGetData(stmt3, 1, SQL_C_SLONG, &amount, sizeof(SQLINTEGER), NULL);
            
            /*Get userid*/
            SQLPrepare(stmt4, (SQLCHAR*) "SELECT userid FROM users WHERE scrname = ?", SQL_NTS);
            
            SQLBindParameter(stmt4, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_CHAR, 0, 0, (SQLCHAR *)argv[2], 0, NULL);
    
            SQLExecute(stmt4);
    
            ret = SQLGetData(stmt3, 1, SQL_C_SLONG, &userid, sizeof(SQLINTEGER), NULL);
            

            
            
            
            sprintf(buf, "INSERT INTO purchase (purchaseid, isbn, amount, saleid, userid, time) VALUES (%d, %s, %s)", (int)purchaseid, argv[i+3], (int)amount, );
    
            SQLExecDirect(stmt2, (SQLCHAR*) buf, SQL_NTS);
        }
        
        
        
    
    
    
    
    
    
    } else if (strcmp(argv[1], "del") == 0) {
        if (argc < 3) {
            printf("Error with buy del. Must specify invoice_id to be deleted");
            return -1;
        }
        
        sprintf(buf, "DELETE FROM purchases WHERE purchaseid = %s", argv[2]);
        SQLExecDirect(stmt_del, (SQLCHAR*) buf, SQL_NTS);

    }
    
    
    
    /* free up statement handle */
    SQLFreeHandle(SQL_HANDLE_STMT, stmt_del);
    SQLFreeHandle(SQL_HANDLE_STMT, stmt1);
    SQLFreeHandle(SQL_HANDLE_STMT, stmt2);
    SQLFreeHandle(SQL_HANDLE_STMT, stmt3);
    SQLFreeHandle(SQL_HANDLE_STMT, stmt4);
    
    /* DISCONNECT */
    ret = odbc_disconnect(env, dbc);
    if (!SQL_SUCCEEDED(ret)) {
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
    
    
    
    /*SQLHENV env;
    SQLHDBC dbc;
    SQLHSTMT stmt_del, stmt1, stmt2;
    SQLRETURN ret;*/ /* ODBC API return status */
    /*int i, purchaseid;
    
    char buf[500];*/

    /* CONNECT */
   /* ret = odbc_connect(&env, &dbc);
    if (!SQL_SUCCEEDED(ret)) {
        return EXIT_FAILURE;
    }

    /* Allocate a statement handle */
   /* SQLAllocHandle(SQL_HANDLE_STMT, dbc, &stmt_del);
    SQLAllocHandle(SQL_HANDLE_STMT, dbc, &stmt1);
    SQLAllocHandle(SQL_HANDLE_STMT, dbc, &stmt2);
    
    
    
    if (strcmp(argv[1], "add") == 0) {
        if (argc<4){
            printf("Error with buy add. Must receive more parameters");
            return -1;
        }
        
        SQLPrepare(stmt1, (SQLCHAR*) "SELECT max(purchaseid) FROM purchase", SQL_NTS);
    
        SQLExecute(stmt1);
    
        ret = SQLGetData(stmt1, 1, SQL_C_SLONG, &purchaseid, sizeof(SQLINTEGER), NULL);
        
        for (i=0; i<= (argc - 2); i++){
            sprintf(buf, "INSERT INTO purchase ( , , ) VALUES (%s, %s, %s)", argv[2], argv[3], argv[1]);
    
            SQLExecDirect(stmt2, (SQLCHAR*) buf, SQL_NTS);
        }
        
        
        
    
    
    
    
    
    
    } else if (strcmp(argv[1], "del") == 0) {
        if (argc < 3) {
            printf("Error with buy del. Must specify invoice_id to be deleted");
            return -1;
        }
        
        sprintf(buf, "DELETE FROM purchases WHERE purchaseid = %s", argv[2]);
        SQLExecDirect(stmt_del, (SQLCHAR*) buf, SQL_NTS);

    }
    
    
    
    /* free up statement handle */
    /*SQLFreeHandle(SQL_HANDLE_STMT, stmt_del);
    SQLFreeHandle(SQL_HANDLE_STMT, stmt1);
    SQLFreeHandle(SQL_HANDLE_STMT, stmt2);
    
    /* DISCONNECT */
    /*ret = odbc_disconnect(env, dbc);
    if (!SQL_SUCCEEDED(ret)) {
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;*/
}


/*Fill sales*/

int fill_sales (int argc, char ** argv) {

    SQLHENV env;
    SQLHDBC dbc;
    SQLHSTMT stmt1, stmt2;
    SQLRETURN ret; /* ODBC API return status */
    SQLINTEGER amount;
    


    char purchaseid[256], isbn[256], userid[256], time[256], buf[500];
    FILE *pf = NULL;

    /* CONNECT */
    ret = odbc_connect(&env, &dbc);
    if (!SQL_SUCCEEDED(ret)) {
        return EXIT_FAILURE;
    }

    /* Allocate a statement handle */
    SQLAllocHandle(SQL_HANDLE_STMT, dbc, &stmt1);
    SQLAllocHandle(SQL_HANDLE_STMT, dbc, &stmt2);

    if (argc < 2) {
        printf("Must specify input_file");
        return -1;
    }

    pf = fopen (argv[1], "r");
    if (pf == NULL) {
        printf("Error opening file");
        return -1;
    }

    while (!feof(pf)) {
        fscanf(pf, "%s\t%s\t%s\t%s", purchaseid, userid, isbn, time);

        SQLPrepare(stmt1, (SQLCHAR*) "SELECT prize FROM edition WHERE isbn = ?", SQL_NTS);
        SQLBindParameter(stmt1, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_CHAR, 0, 0, (SQLCHAR *) isbn, 0, NULL);

        SQLExecute(stmt1);
    
        ret = SQLGetData(stmt1, 1, SQL_C_SLONG, &amount, sizeof(SQLINTEGER), NULL);

        sprintf(buf, "INSERT INTO purchase (purchaseid, isbn, amount, userid, time) VALUES (%s, %s, %d, %s, %s)", purchaseid, isbn, (int) amount, userid, time);
        SQLExecDirect(stmt2, (SQLCHAR*) buf, SQL_NTS);

    }










    /* free up statement handle */
    SQLFreeHandle(SQL_HANDLE_STMT, stmt1);
    SQLFreeHandle(SQL_HANDLE_STMT, stmt2);
    
    /* DISCONNECT */
    ret = odbc_disconnect(env, dbc);
    if (!SQL_SUCCEEDED(ret)) {
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;


}