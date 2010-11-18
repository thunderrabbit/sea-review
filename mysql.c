#include "db.config"
#include "file.config"
#include <mysql/my_global.h>
#include <mysql/mysql.h>
#include <string.h>
#include <sys/stat.h>

char command[20];    // determined by the - switch (e.g. -c, -d, -r, -s)
char **options;      // command line args after command and not including the - switch
int optionsc;        // number of options sent  (usually argc - 2)

MYSQL * mysql_connect();
int row_inserter(MYSQL * conn, char * table, int fieldsc, char ** fieldsv);
void log_args(int argc, char **argv);
void get_args(int argc, char **argv);
int create_table(int optionsc, char ** optionsv);

char * get_filename()
{
  char return_filename[250];    // where we will compose the string for output
  char filecount_name[250];     // name of file in directory which hold latest filecount
  int filecount, i;
  struct stat buf;
  char zero_filled_filename[10];
  char * output_string;
  FILE * file;

  strcpy(filecount_name, LOG_DIR);                 //   /full/path/to/logs_dir/
  strcat(filecount_name, command);                 //   /full/path/to/logs_dir/CREATE
  strcat(filecount_name, "/");

  strcpy(return_filename,filecount_name);          //   /full/path/to/logs_dir/CREATE/
  strcat(filecount_name,FILECOUNT_NAME);           //   /full/path/to/logs_dir/CREATE/.filecount

  i = stat(filecount_name, &buf);

  if(i)
    {
      // file DNE
      filecount = 0;
    }
  else
    {
      file = fopen(filecount_name,"r");
      fscanf(file, "%d", &filecount);
      fclose(file);
    }

  file = fopen(filecount_name,"w");
  if(file)
    {
      fprintf(file,"%d",filecount + 1);
      fclose(file);
    }

  sprintf(zero_filled_filename,"%06d",filecount);
  strcat(return_filename,zero_filled_filename);

  output_string = malloc(strlen(return_filename) + 1);
  strcpy(output_string,return_filename);

  return(output_string);
}

int store_command(char * filename, char * query)
{
  FILE * file;
  file = fopen(filename,"w");
  if(file)
    {
      fprintf(file,"%s\n",query);
      fclose(file);
      return 1;
    }
  else
    {
      return 0;
    }
}

char * load_command(char * filename)
{
  FILE * file;
  char query[500];
  char nothing[1];

  strcpy(nothing,"");

  file = fopen(filename,"r");
  if(file)
    {
      return fgets(query, 500, file);
    }
}

int go_do_it(char * query)
{
  char loaded_query[500];
  char * filename;

  filename = get_filename();
  if(store_command(filename, query))
    {
      if(strcpy(loaded_query, load_command(filename)))
	{
	  MYSQL *conn;
	  MYSQL_RES *result;
	  MYSQL_ROW row;
	  int num_fields, i;

	  conn = mysql_connect();
	  if(mysql_query(conn, loaded_query))
	    {
	      printf("%s\n",mysql_error(conn));
	      int errno;
	      errno = mysql_errno(conn);
	      mysql_free_result(result);
	      mysql_close(conn);

	      return errno;
	    }
	  printf("executed command stored in file %s\n",filename);
	}
    }
  free(filename);
}

int create_table(int optionsc, char ** optionsv);
int insert_row(int optionsc, char ** optionsv);

int main(int argc, char **argv)
{
  int success;

  printf("MySQL client version: %s\n", mysql_get_client_info());

  get_args(argc, argv);

  if(strcmp(command,"CREATE") == 0)
    {
      success = create_table(optionsc, options);
    }
  else if(strcmp(command,"INSERT") == 0)
    {
      success = insert_row(optionsc, options);
    }
  else if(strcmp(command,"ALTER") == 0)
    {
      success = alter_table(optionsc, options);
    }

  return success;
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
	    case 'i':
	      strcpy(command, "INSERT");
	      break;
	    case 's':
	      strcpy(command, "SELECT");
	      break;
	    case 'a':
	      strcpy(command, "ALTER");
	      break;
	    case 'r':
	      strcpy(command, "REMOVE");
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

int create_table(int optionsc, char ** optionsv)
{
  int i;
  char query[500];
  strcpy(query,"CREATE TABLE ");

  for(i = 0; i < optionsc; i++)
    {
      if(i == 1)
	{
	  strcat(query,"(");
	}
      strcat(query, optionsv[i]);
      strcat(query, " ");
    }
  strcat(query,");");

  return go_do_it(query);
}

int alter_table(int optionsc, char ** optionsv)
{
  int i;
  char query[500];
  strcpy(query,"ALTER TABLE ");

  for(i = 0; i < optionsc; i++)
    {
      strcat(query, optionsv[i]);
      strcat(query, " ");
    }

  return go_do_it(query);
}

int insert_row(int optionsc, char ** optionsv)
{
  int i;
  char query[500];
  strcpy(query,"INSERT INTO ");

  strcat(query,optionsv[0]);     // table name
  strcat(query," VALUES (");
  for(i = 1; i < optionsc; i++)
    {
      strcat(query, optionsv[i]);
      strcat(query, " ");
    }
  strcat(query,");");

  return go_do_it(query);
}
