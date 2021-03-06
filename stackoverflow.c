#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void get_args_works(int, char **, char **);
void get_args_broken(int, char **, char *);
char *get_string(int, char **);

int main(int argc, char **argv)
{
  char *string_works;
  char *string_broken;

  get_args_works(argc, argv, &string_works);
  get_args_broken(argc, argv, string_broken);

  printf("in main string_works (%p) = %s\n",string_works,string_works);
  free(string_works);

  printf("in main string_broken (%p) = %s\n",string_broken,string_broken);
  free(string_broken);
}

void get_args_works(int argc, char **argv, char **string)
{
    *string = get_string(argc, argv);
    printf("in get_args_works %p string %s\n",*string,*string);
}

void get_args_broken(int argc, char **argv, char *string)
{
  string = get_string(argc, argv);
  printf("in get_args_broken %p string %s\n",string,string);
}

char * get_string(int argc, char **argv)
{
  int i;
  char *string;
  string = malloc(40);
  strcpy(string,"-s switch not found below");                // placeholder in case -s switch not found below

  for(i = 0; i < argc; i++)
    {
      if(argv[i][0] == '-')
	{
	  switch(argv[i][1])
	    {
	    case 's':
	      free(string);                               // relase above malloc(40) for "-s switch not found below"
	      string = malloc(strlen(argv[++i]) + 1);     // make room for storing variable
	      strcpy (string,argv[i]);                    // the argv just after -s
	      break;
	    }
	}
    }
  return string;
}
