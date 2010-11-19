#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int get_args_works(int, char **, char **);
int get_args_broken(int, char **, char *);
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

int get_args_works(int argc, char **argv, char **string)
{
    *string = get_string(argc, argv);
    printf("in get_args %p string %s\n",*string,*string);
}

int get_args_broken(int argc, char **argv, char *string)
{
  string = get_string(argc, argv);
  printf("in get_args %p string %s\n",string,string);
}

char * get_string(int argc, char **argv)
{
  int i;
  char *string;
  string = malloc(40);
  strcpy(string,"initialized string var");   // placeholder

  for(i = 0; i < argc; i++)
    {
      if(argv[i][0] == '-')
	{
	  switch(argv[i][1])
	    {
	    case 's':
	      free(string);
	      string = malloc(strlen(argv[++i]) + 1);
	      strcpy (string,argv[i]);     // what we actually want
	      break;
	    }
	}
    }
  return string;
}
