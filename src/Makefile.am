AM_CFLAGS = @CFLAGS@
AM_LDFLAGS =
# this lists the binaries to produce, the (non-PHONY, binary) targets in
# # the previous manual Makefile
#
bin_PROGRAMS = csvsplit csvcombine
csvsplit_SOURCES = csvsplit.c
csvsplit_CFLAGS = -I$(top_srcdir)/include
csvsplit_LDFLAGS =
csvsplit_LDADD = ../lib/libcsvtools.la
#
csvcombine_SOURCES = csvcombine.c
csvcombine_CFLAGS = -I$(top_srcdir)/include
csvcombine_LDFLAGS = 
csvcombine_LDADD = ../lib/libcsvtools.la
#
if USE_ODBC_H
bin_PROGRAMS += csv2odbc
csv2odbc_SOURCES = csv2odbc.c 
csv2odbc_CFLAGS = -I$(top_srcdir)/include
csv2odbc_LDFLAGS = -lodbc
csv2odbc_LDADD = ../lib/libcsvtools.la
endif
#
if USE_XML_H
bin_PROGRAMS += csv2xml
csv2xml_SOURCES = csv2xml.c
csv2xml_CFLAGS = -I$(top_srcdir)/include
csv2xml_LDFLAGS = 
csv2xml_LDADD = ../lib/libcsvtools.la
endif
#
if USE_JSON_H
bin_PROGRAMS += csv2json
csv2json_SOURCES = csv2json.c
csv2json_CFLAGS = -I$(top_srcdir)/include
csv2json_LDFLAGS =
csv2json_LDADD = ../lib/libcsvtools.la
endif
#
if USE_MYSQL_H
bin_PROGRAMS += csv2mysql
csv2mysql_SOURCES = csv2mysql.c 
csv2mysql_CFLAGS = -I$(top_srcdir)/include $(MYSQL_INCLUDE)
csv2mysql_LDFLAGS = $(MYSQL_LIBS)
csv2mysql_LDADD = ../lib/libcsvtools.la
endif
#
if USE_PGSQL_H
bin_PROGRAMS += csv2pgsql
csv2pgsql_SOURCES = csv2pgsql.c
csv2pgsql_CFLAGS = -I$(top_srcdir)/include -I$(PG_INCLUDE)
csv2pgsql_LDFLAGS = -lpq -L$(PG_LIBS)
csv2pgsql_LDADD = ../lib/libcsvtools.la
endif
#
if USE_SQLITE_H
bin_PROGRAMS += csv2sqlite
csv2sqlite_SOURCES = csv2sqlite.c
csv2sqlite_CFLAGS = -I$(top_srcdir)/include
csv2sqlite_LDFLAGS = -lsqlite3
csv2sqlite_LDADD = ../lib/libcsvtools.la
endif
#
