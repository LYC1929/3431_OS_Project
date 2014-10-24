#ifndef _common_h_
#define _common_h_

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <my_global.h>
#include <mysql.h>
#define MAXCRS 7
#define MAXLEN 20
#define MAXQUERY 1024
#include "Queue.h"

int database(char *, char *);
int printRes(MYSQL_RES *);



#endif
