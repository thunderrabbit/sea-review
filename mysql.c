#include "db.config"
#include "file.config"
#include <mysql/my_global.h>
#include <mysql/mysql.h>
#include <string.h>

char command[20];
char **options;
int optionsc;

MYSQL * mysql_connect();
int row_inserter(MYSQL * conn, char * table, int fieldsc, char ** fieldsv);
void log_args(int argc, char **argv);
void get_args(int argc, char **argv);

int main(int argc, char **argv)
{
   printf("MySQL client version: %s\n", mysql_get_client_info());

   MYSQL *conn;
   MYSQL_RES *result;
   MYSQL_ROW row;
   int num_fields, i;

   get_args(argc, argv);

   printf("command is %s\n", command);
   for(i = 0; i < optionsc; i++)
     {
       printf("option[%d] is %s\n", i, options[i]);
     }

   exit(0);
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

void log_args(int argc, char **argv)
{
  int i;
  FILE * log_file;

  if((log_file = fopen(ARG_LOG, "ab")) == NULL)
    {
      printf("fail opening %s\n",ARG_LOG);
    }
  else
    {
      for(i = 0; i < argc; i++)
	{
	  fprintf(log_file,"%s ",argv[i]);
	}
      fprintf(log_file,"\n");
      fclose(log_file);
    }
}

void get_args(int argc, char **argv)
{

  log_args(argc, argv);

  int arg_counter = 0;
  optionsc = 0;
  options = malloc((argc - 2) * sizeof(char *));    // argc - 2 because don't save command nor first switch (e.g. -c, -d)

  for(arg_counter = 1; arg_counter < argc; arg_counter++)
    {
      if(argv[arg_counter][0] == '-')
	{
	  switch(argv[arg_counter][1])
	    {
	    case 'c':
	      strcpy(command, "CREATE");
	      break;
	    case 'd':
	      strcpy(command, "DELETE");
	      break;
	    default:
	      printf("unknown switch %s\n",argv[arg_counter]);
	    }
	}
      else
	{
	  // grab all non -hyphen prefixed args
	  options[optionsc] = malloc(strlen(argv[arg_counter] + 1));
	  strcpy(options[optionsc],argv[arg_counter]);
	  optionsc++;
	}
    }
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

