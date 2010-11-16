#include <stdio.h>
#include <math.h>

#define SIZE 3
#define MAX_INSANITY 4
#define TRUE 1
#define FALSE 0
#define DEFAULT_STEP_LENGTH 10

int  main (int argc, char **argv)
{
  int    angle_degree, i, sanity = 0, num_steps, step_length;
  float  angle_radian, pi, value, increment, x[SIZE], *fp;

  int got_num_steps = FALSE;
  int got_step_length = FALSE;
  int got_pi = FALSE;

  for(i = 1; i < argc; i++)
    {
      if(argv[i][0] == '-')
	{
	  switch(argv[i][1])
	    {
	    case 'l':  step_length = atoi(argv[++i]); got_step_length = TRUE;  break;
	    case 's':  num_steps = atoi(argv[++i]); got_num_steps = TRUE; break;
	    case 'p':  pi = atof(argv[++i]); got_pi = TRUE; break;
	    default :  printf("skipping switch \"%s\"\n", argv[i]);
	    }
	}
    }

  if(!got_step_length && !got_num_steps)
    {
      while(!got_num_steps && sanity++ < MAX_INSANITY)
	{
	  printf("enter number of steps\n");
	  scanf("%d", &num_steps);
	  if(5 < num_steps && num_steps < 100)
	    {
	      got_num_steps = TRUE;
	    }
	}
    }

  if(got_num_steps)
    {
      step_length = 360 / (float)num_steps;
    }
  else if(got_step_length)
    {
      num_steps = 360 / (float)step_length;
    }
  else
    {
      printf("step up or step off!\n");
      return 1;
    }

  if(!got_pi)
    {
      pi = 4.0*atan(1.0);
    }

  printf("num_steps = %d\n",num_steps);
  printf("step_length = %d\n",step_length);
  printf("pi = %f\n\n", pi);

  return 1;


  printf("enter new value for pi\n");
  scanf("%f", &pi);

  printf("\nCompute a table of the sine function \n\n");


  printf("angle    Sine \n");
  angle_degree = 0;

  sanity = 0;
  while(angle_degree <= 360 && sanity < MAX_INSANITY)
    {
      angle_radian = pi * angle_degree / 180.0;
      value = sin(angle_radian);
      printf("%3d      %f\n", angle_degree, value);
      angle_degree = angle_degree + increment;
      sanity++;
    }

  printf("increment = %f\n", increment);
  printf("pi = %f\n", pi);

  for (i=0; i < SIZE; i++)
    {
      x[i] = 0.5*(float)i;
    }

  for(i=0; i < SIZE; i++)
    {
      printf("  %d  %f  \n", i, x[i]);
    }

  fp = x;

  for(i=0; i < SIZE; i++)
    {
      printf("   %d    %f\n", i, *(fp+i));
    }

}
