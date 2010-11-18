// adapted from http://pw1.netcom.com/~tjensen/ptr/ch6x.htm

#include <stdio.h>
#define ROWS 20
#define COLS 30

int multi[ROWS][COLS];

void main()
{
  int row, col;

  for(row = 0; row <= ROWS; row++)
    {
      for(col = 0; col <= COLS; col++)
	{
	  multi[row][col] = row * col;
	}
    }

  for(row = 0; row < ROWS; row++)
    {
      printf("row %2d: ", row);
      for(col = 0; col < COLS; col++)
	{
	  printf("%3d, ", *(*( multi + row) + col));
	}
      printf("\n");
    }
}
