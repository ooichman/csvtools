/* 
 * int csvobjnum(cahr * csvLine) 
 * check for the amont of objects on the line
 * int csvobjnum(char* csvLine,char* csvDelimiter)
 * check for the amount of objects with the given delimiter
 * char* csvcheckdel(char* csvLine)
 * check for special characters and returning the most likly delimiter
 * char* csvgetheader(char* csvLine)
 * returns an array of string which contains the works for the line without the delimiter
 * char* csvgetheader(char* csvLine, char* csvDelimiter)
 * returns an array of string which contains the works for the line without the delimiter
 *
 */

#ifndef LIBCSVTOOLS_H_INCLUDED
#define LIBCSVTOOLS_H_INCLUDED

#define MAX_LENGTH 4096

void Print_csvtools();

void chomp(char* s);

const int csvobjnum(char *csvLine,char delimiter,int* quotation_val);

void nospaces(char* csvLine);

void rplccomma(char* csvLine,char delimiter);

int firstlinetst (char* firstLine,int vopt,char col_seperator,int *quot_val);

#endif
