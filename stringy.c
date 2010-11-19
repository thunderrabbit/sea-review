#include "stringy.c.config"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int get_args(int, char **, char *);
char *get_year3(int, char **);
char *get_year2(int, char **);
char *get_year1(int, char **);
char *get_year(int, char **);

int main(int argc, char **argv)
{
  char *year;

  printf("in main %p year\n",year);
  if(!get_args(argc, argv, year))
    {
      printf("failed getting args.\n");
      exit(1);
    }

  printf("in main %p year\n",year);
  printf("year: %s\n", year);
  free(year);
}

int get_args(int argc, char **argv, char *year)
{
  int int_year;

  if(argc != (NUM_ARGS + 1))
    {
      printf("usage %s -y YYYY\n",argv[0]);
      return 0;
    }
  
  printf("in get_args %p year %s\n",year,year);
  year = get_year3(argc, argv);
  printf("in get_args %p year %s\n",year,year);
  
  if(!year)
    {
      printf("gimme a year\n");
      return 0;
    }

  printf("ptr |%p|\n", year);
  printf("str |%s|\n", year);
  printf("int |%d|\n", year);
  int_year = atol(year);
  if(1990 < int_year && int_year < 2020)
    {
      // success
      return 1;
    }
  else
    {
      printf("year %s out of range.  but fuckit; let's move on.\n",year);
      return 1;
    }
}

char * get_year3(int argc, char **argv)
{
  char * year;
  printf("in get_year3 %p\n",year);
  year = get_year2(argc,argv);
  printf("in get_year3 %p year %s\n",year,year);
  return year;
}

char * get_year2(int argc, char **argv)
{
  char * year;
  printf("in get_year2 %p year %s\n",year,year);
  year = get_year1(argc,argv);
  printf("in get_year2 %p year %s\n",year,year);
  return year;
}

char * get_year1(int argc, char **argv)
{
  char * year;
  printf("in get_year1 %p year %s\n",year,year);
  year = get_year(argc,argv);
  printf("in get_year1 %p year %s\n",year,year);
  return year;
}

char * get_year(int argc, char **argv)
{
  int i;
  char *year;
  printf("in get_year %p year\n",year);
  year = malloc(4);
  printf("in get_year %p year\n",year);
  strcpy(year,"dog");

  for(i = 0; i < argc; i++)
    {
      if(argv[i][0] == '-')
	{
	  switch(argv[i][1])
	    {
	    case 'y':
	      free(year);
	      year = malloc(strlen(argv[++i]) + 1);
	      strcpy (year,argv[i]);
	      break;
	    }
	}
    }
  return year;
}
