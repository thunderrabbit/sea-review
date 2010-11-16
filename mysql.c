#include "db.config"
#include <mysql/my_global.h>
#include <mysql/mysql.h>
#include <string.h>

MYSQL * mysql_connect()
{
  MYSQL *conn;
  conn = mysql_init(NULL);

  if (conn == NULL)
    {
      printf("Error %u: %s\n", mysql_errno(conn), mysql_error(conn));
      exit(1);
    }
  
  if(mysql_real_connect(conn, HOST, USER, PASS, DATA, 0, NULL, 0) == NULL)
    {
      printf("Error %u: %s\n", mysql_errno(conn), mysql_error(conn));
      exit(1);
    }

  return conn;
}

void auto_inserter(MYSQL * conn)
{
  //   mysql_query(conn, "IF TABLE NOT EXISTS CREATE TABLE writers(name VARCHAR(25))");
   mysql_query(conn, "INSERT INTO writers VALUES('Jon McNamara3')");
   mysql_query(conn, "INSERT INTO writers VALUES('Jon McNamara4')");
}

int row_inserter(MYSQL * conn, char * table, int fieldsc, char ** fieldsv)
{
  int i;
  char field_string[1001];
  char trunc_field_string[1000];
  char query_string[1050];

  strcpy(field_string, "");
  strcpy(trunc_field_string, "");
  strcpy(query_string, "");

  // wrap each field with quotes and append comma
  for(i = 1; i < fieldsc; i++)
    {
      strcat(field_string, "\"");
      strcat(field_string, fieldsv[i]);
      strcat(field_string, "\",");
    }
  strncpy(trunc_field_string, field_string, strlen(field_string) - 1);        // remove last comma
  trunc_field_string[strlen(field_string) -1] = '\0';                         // add NULL

  sprintf(query_string,"INSERT INTO %s VALUES (%s)", table, trunc_field_string);
  mysql_query(conn, query_string);
}

int main(int argc, char **argv)
{
   printf("MySQL client version: %s\n", mysql_get_client_info());

   MYSQL *conn;
   MYSQL_RES *result;
   MYSQL_ROW row;
   int num_fields, i;

   conn = mysql_connect();

   //   auto_inserter(conn);

//   mysql_query(conn, "ALTER TABLE writers MODIFY name VARCHAR(70)");
//   mysql_query(conn, "ALTER TABLE writers ADD age INT");
//   mysql_query(conn, "ALTER TABLE writers ADD city VARCHAR(70)");
   row_inserter(conn, "writers", argc, argv);

   mysql_query(conn, "SELECT * FROM writers");
   result = mysql_store_result(conn);

   num_fields = mysql_num_fields(result);

   while((row = mysql_fetch_row(result)))
     {
       for(i = 0; i < num_fields; i++)
	 {
	   printf("%s ", row[i] ? row [i] : "(missing)");
	 }
       printf("\n");
     }

   mysql_free_result(result);
   mysql_close(conn);
}
