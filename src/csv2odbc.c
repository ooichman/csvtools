/*
 * the csv-tools is an open source project for Enterprise central
 * tasks scheduling for the Unix/Linux platform
 * Copyright (C) yyyy  Oren Oichman
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 *
 * Gnomovision version 1, Copyright (C) Oren Oichman
 * Gnomovision comes with ABSOLUTELY NO WARRANTY; for details
 * type `csv2* -h'.  This is free software, and you are welcome
 * to redistribute it under certain conditions; type `canoectl -h' 
 * for details.
 */


#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <stdlib.h>
#include <unistd.h>
#include "csvtools.h"
#include <sql.h>
#include <sqlext.h>
#include <sqltypes.h>

typedef struct {
	unsigned int ver_opt:1;
	unsigned int file_opt:1;
	unsigned int list_opt:1;
} arg_options;

void tablecheck(char *col_stmt,char *tname,SQLRETURN sql_ret,SQLHSTMT stmt, int vopt) {

char *tsql_q;

        tsql_q = (char *)malloc(sizeof(char) * ( strlen(tname) + strlen(col_stmt) + 30));
        strncpy(tsql_q,"select ",sizeof(tsql_q));
        strcat(tsql_q,col_stmt);
        strcat(tsql_q," from ");
        strcat(tsql_q,tname);
        strcat(tsql_q," limit 1");

        sql_ret = SQLExecDirect(stmt, tsql_q , SQL_NTS);
                if (!SQL_SUCCEEDED(sql_ret)) {
                        fprintf(stderr,"problem with the first row of the clumns names\n");
                        exit(7);
                }
                if(vopt == 1) {
                        printf("the clumns are consistant with the table\n");
                }

	free(tsql_q);
}


void Help(int rcode) {

	printf("Usage:  csv2odbc\n");
	printf("\t\t -v - Be Verbose when running the command\n");
	printf("\t\t -s - the seperator between the columns (Default \",\") \n");
	printf("\t\t -h - Display this manu\n");
	printf("\t\t -f - set The CSV file to read from (Default stdin)\n");
	printf("\t\t -d - DSN name - set by the odbc.ini file\n");
	printf("\t\t -c - Column's names - set the columns name (Defualt: First Line)\n");
	printf("\t\t -t - Destantion Table Name\n");
	printf("\t\t -l - Listing Installed Data Sources\n");
	printf("\n");
	
	exit(rcode);

}

int main(int argc,char *argv[]) {

	arg_options arg_opt = { 0 , 0 ,0 };
	int c=0,exline=0,rcode=0;
	int quot_val=0,i=0,k=0;
	FILE *ifile = NULL;
	char *dname = NULL,buff[MAX_LENGTH];
	char *tname=NULL,*sql_q=NULL;
	char *collist=NULL;
	char *col_stmt=NULL;
	char col_seperator=',';
	char *lenbuff=NULL;
	int linenum=0,colnum=0;
	char dsn[256];
	char desc[256];
	SQLSMALLINT dsn_ret;
	SQLSMALLINT desc_ret;
	SQLUSMALLINT direction;
	
	opterr = 0;

	SQLHDBC dbc;
	SQLHENV env;
	SQLHSTMT stmt;
	SQLRETURN ret;
	SQLRETURN sql_ret;
	

	 while ((c = getopt (argc, argv, "hs:vlf:d:t:c:")) != -1)
		switch (c) {

			case 'l':
				arg_opt.list_opt = 1;
			break;

			case 'h':
				Help(0);
			break;

			case 'c':
				col_stmt = optarg;
			break;

			case 's':
				 col_seperator = optarg[0];
			break;
			case 'f':
				if (ifile = fopen(optarg,"rt")){
					printf("the file %s is o.k.\n",optarg);
					arg_opt.file_opt = 1;
				}
				else {
					fprintf(stderr,"the file \"%s\" can not be opened\n",optarg);
					exit(1);
				}
			break;
			case 'd':
				dname = optarg;
			break;

			case 't':
				tname = optarg;
			break;
			case 'v':
				arg_opt.ver_opt = 1;
			break;
			case '?':
                        if ( (optopt == 'f') || (optopt == 'd') || (optopt == 't') )
            		{
                           fprintf (stderr," " "Option -%c requires an argument.\n", optopt);
			}
            		else if (isprint (optopt))
              		{
				fprintf (stderr, "Unknown option `-%c'.\n", optopt);
			}
              		else {
                           fprintf (stderr, "Unknown option character `\\x%x'.\n", optopt);
                           exit(EXIT_FAILURE);
			}

			default:
				Help(1);
			break;
				
	}

	if ( arg_opt.list_opt == 0 ) {

		if ((!getenv("C2ODSNAME")) && (!dname)) {
			fprintf(stderr,"error - missing target DS name\n\n");
			Help(3);
		}
	
		else if (getenv("C2ODSNAME"))
			dname = getenv("C2ODSNAME");

		if ((!getenv("C2OTABLENAME")) && (!tname)) {
			fprintf(stderr,"error - missing target table name\n\n");
			Help(2);
		}

		else if (getenv("C2OTABLENAME"))
			tname = getenv("C2OTABLENAME");
	
	}

	// trying to connect to the Database 
	

        SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &env);
        SQLSetEnvAttr(env, SQL_ATTR_ODBC_VERSION, (void *) SQL_OV_ODBC3, 0);
        SQLAllocHandle(SQL_HANDLE_DBC, env, &dbc);

	if ( arg_opt.list_opt == 1 ) {
		
		 direction = SQL_FETCH_FIRST;
		while(SQL_SUCCEEDED(ret = SQLDataSources(env, direction,
			dsn, sizeof(dsn), &dsn_ret,
			desc, sizeof(desc), &desc_ret))) {
		direction = SQL_FETCH_NEXT;
		printf("Data Sources Name: [%s] \nDescription - %s\n", dsn, desc);
		if (ret == SQL_SUCCESS_WITH_INFO) printf("\tdata truncation\n");
		}

		exit(0);
	}

        SQLCHAR dsnstr[30] = "DSN=";
        strcat(dsnstr,dname);
        strcat(dsnstr,";");

	if(arg_opt.ver_opt == 1)
		printf("trying to connect to %s with odbc\n",dname);
        ret = SQLDriverConnect(dbc, NULL, dsnstr, SQL_NTS,
                                NULL, 0, NULL, SQL_DRIVER_COMPLETE);

        if (!SQL_SUCCEEDED(ret)) {
                fprintf(stderr,"error - problem connecting to DSN - \"%s\" \n",dname);
                exit(2);
        }
	else {
		if(arg_opt.ver_opt == 1)
			printf("the SQL connection is looking good\n");
	}
	ret = SQLAllocHandle(SQL_HANDLE_STMT, dbc, &stmt);

	/// checking if a file has been select or to start listening to STDIN
	
	if(arg_opt.file_opt == 0) {
		ifile = stdin;
		if(arg_opt.ver_opt == 1)
			printf("listening to STDIN for input\n");
	}

	// checking if the columns names are from the -c option
	
	if (col_stmt != NULL) {
		colnum = firstlinetst(col_stmt,arg_opt.ver_opt,col_seperator,&quot_val);
		tablecheck(col_stmt,tname,sql_ret,stmt,arg_opt.ver_opt);
	}

	while( ((fgets(buff,MAX_LENGTH,ifile)) != NULL) ) {

		if (strlen(buff) < (colnum+1))
			break;

		// handling the first line to extract the columns names 
		// from the CSV file
		
		if ( (linenum == 0) && (col_stmt == NULL)) {

			chomp(buff);
			colnum = firstlinetst(buff,arg_opt.ver_opt,col_seperator,&quot_val);
			col_stmt = (char *)calloc(strlen(buff),sizeof(char));
			strcpy(col_stmt,buff);
			
			tablecheck(col_stmt,tname,sql_ret,stmt,arg_opt.ver_opt);
			continue;
		}


		if (collist == NULL ) {
			collist = (char *)calloc((strlen(buff)+(colnum*2)+1),sizeof(char));
			k = colnum;
		}
		else {
			collist = (char *)realloc(collist,((strlen(collist)+strlen(buff)+1)*sizeof(char)));
		}

		if (quot_val == 0) {
			collist[0] = '\'';
			i=1;
		}

		for (c=0;c<strlen(buff);c++) {

			if (k < 0) {
				fprintf(stderr,"error - the number of columns is to much at line %d \n",linenum);
				rcode=5;
				break;
			}

			if ((buff[c] == col_seperator) && (quot_val == 0)) {
				collist[i] = '\'';
				i++;
				collist[i] = ',';
				i++;
				collist[i] = '\'';
				k--;
				i++;
			}
			
			else if ((buff[c] == '\"') || (buff[c] == '\'')) {
                        	if(quot_val == 0)
                                	quot_val=1;
                        	else if (quot_val == 1)
                                	quot_val=0;
                	}

			else if ( buff[c] == '\n' ) {
				collist[i] = '\'';
				i++;
			}

			else {
				collist[i] = buff[c];
				i++;
			}

		}


		if (quot_val == 0) {
	
			/* put the sql insert and query build here
 			 * do not forget to print is for testing and for 
 			 * verbose request 
 			 */

			sql_q = (char *)calloc(((strlen(collist)+strlen(col_stmt)+30)*2),sizeof(char));
			strcpy(sql_q,"insert into ");
			strcat(sql_q,tname);
			strcat(sql_q," (");
			strcat(sql_q,col_stmt);
			strcat(sql_q,") values(");
			strcat(sql_q,collist);
			strcat(sql_q,");");

			sql_ret = SQLExecDirect(stmt, sql_q , SQL_NTS);
                		if (!SQL_SUCCEEDED(sql_ret)) {
                        		fprintf(stderr,"error - problem inserting row number %d\n",linenum);
                        		exit(7);
                		}
                		
			if (arg_opt.ver_opt == 1)
				printf("%s\n",sql_q);


			free(collist);
			free(sql_q);
			collist = NULL;
		}
		else {
			if (exline == 0)
				exline++;
			else {
				fprintf(stderr,"error - Invalid single quotation use in line %d\n",linenum);
				rcode=7;
				break;
			}
			
		}

		linenum++;
		memset(buff,'\0',0);
	}

	if (arg_opt.ver_opt == 1)
		printf("the number of lines is : %d \n",linenum);

	SQLFreeHandle(SQL_HANDLE_DBC, dbc);
  	SQLFreeHandle(SQL_HANDLE_ENV, env);
        free(lenbuff);
        free(sql_q);

	return rcode;
}
