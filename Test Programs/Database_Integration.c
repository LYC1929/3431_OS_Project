#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <my_global.h>
#include <mysql.h>
#define MAXCRS 7
#define MAXLEN 20
#define MAXQUERY 1024

int printRes(MYSQL_RES *result);	//print result to the console

int main(void){
	char course1[MAXCRS], course2[MAXCRS];	//Stores user inputs

	MYSQL_RES *result;	//Stores the resulting rows of students
	char uName[MAXLEN], password[MAXLEN];	//Stores username and password
	MYSQL *sql;			//Declare mysql connection
	char query[MAXQUERY];	//Stores mysql query
	//Promote user to login his/her database
	printf("Please enter your username: ");
	scanf("%s", uName);
	printf("Please enter your password: ");
	system("stty -echo");
	scanf("%s", password);
	system("stty echo");
	
	sql=mysql_init(NULL);	//Initialize sql
	//If initialization failed
	if (mysql_real_connect(sql, "db.cs.dal.ca", uName, password, "farrag", 0, NULL, 0) == NULL){
		printf("Fail to Initialize mysql connection, error code: %d, error message: %s\nProgram will exit in 3 seconds.\n", mysql_errno(sql), mysql_error(sql));
		sleep(3);
		exit(EXIT_FAILURE);
	}
	else
		printf("\n\n*** Connected to mysql server %s as user: %s ***\n\n", mysql_get_client_info(), uName);

	//Read two course names
	printf("\nPlease enter the first course name: ");
	scanf("%s", course1);
	printf("Please enter the second course name: ");
	scanf("%s", course2);

	//Build an sql query
	sprintf(query, "SELECT S FROM farrag.enrolls WHERE C IN (SELECT C FROM farrag.enrolls WHERE C=\"%s\" OR C=\"%s\") GROUP BY S HAVING COUNT(S)>1;", course1, course2);

	//Execute query and print result
	printf("\nThe sql query is: \n%s\n", query);
	printf("\nThe result is: \n\n");

	if(mysql_query(sql, query) != 0){
		printf("Fail to execute mysql query! Program will exit in 3 seconds.\n");
		sleep(3);
		exit(EXIT_FAILURE);
	}
	result=mysql_store_result(sql);
	if (result == NULL)
		printf("Program failed to store the results, nothing will be printed.");
	else{
		if (printRes(result) == 0)
			printf("Failed to print the result to the console.\n");
		//Free result
		mysql_free_result(result);
	}
	//Disconnect mysql
	mysql_close(sql);
	return 0;
}

//print the results to the console
int printRes(MYSQL_RES *result){
	int all_ok=1;
	MYSQL_ROW row;		//Stores one row
	int numOfFields;	//stores how many fields are there in the result
	numOfFields=mysql_num_fields(result);

	while( (row=mysql_fetch_row(result)) ){
		int i;
		for (i=0; i<numOfFields; i++)
			printf("%s ", row[i] ? row[i] : "NULL");
		printf("\n");
	}
	printf("\n");
	return all_ok;
}
