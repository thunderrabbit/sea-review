// adapted from http://pw1.netcom.com/~tjensen/ptr/ch6x.htm

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
  int row, col;

  int ROWS, COLS;

  if(argc != 3)
    {
      printf("usage: %s n m\n",argv[0]);
      puts("where n is number of rows and m is number of columns.");
      return 1;
    }
  
  ROWS = atoi(argv[1]);    // pretending these are CONSTANTS
  COLS = atoi(argv[2]);

  if(! (ROWS * COLS))
    {
      puts("enter positive integers, or there will be nothing to see.");
    }

  int **rowptr;
  rowptr = malloc(ROWS * sizeof(int *));

  if(rowptr == NULL)
    {
      printf("error allocating array of int pointers\n");
      return 1;
    }

  printf("\n\nIndex      Pointer(hex)       Pointer(dec)      Diff.(dec)");

  for(row = 0; row < ROWS; row++)
    {
      rowptr[row] = malloc(COLS * sizeof(int));
      if(rowptr[row] == NULL)
	{
	  printf("\nerror allocating memory for row[%d]\n",row);
	  return 1;
	}
      printf("\n %3d        %p              %d", row, rowptr[row], rowptr[row]);
      if(row > 0)
	{
	  printf("        %d",(int)(rowptr[row] - rowptr[row-1]));
	}

//      for(col = 0; col <= COLS; col++)
//	{
//	  multi[row][col] = row * col;
//	}
    }


//  for(row = 0; row < ROWS; row++)
//    {
//      printf("row %2d: ", row);
//      for(col = 0; col < COLS; col++)
//	{
//	  printf("%3d, ", *(*( multi + row) + col));
//	}
//      printf("\n");
//    }
  puts("\n");


  for(row = 0; row < ROWS; row++)
    {
      free(rowptr[row]);
    }

  free(rowptr);

}
