// from http://pw1.netcom.com/~tjensen/ptr/ch2x.htm
#include <stdio.h>

int int_array[] = {1, 2, 3, 5, 7, 11, 13, 17, 19, 23, 31};
int *p, i;

int main(int argc, char ** argv)
{
  //  p = &int_array[0];
  p = int_array;            // equivalent

  for(i = 0; i < sizeof(int_array)/sizeof(int); i++)
    {
      printf("%p int_array[%d] = %d\n", (void *)&int_array[i], i, int_array[i]);
      printf("%p     ptr + %d  = %d\n", (void *)p, i, *p++);
      //      printf("%p     ptr + %d  = %d\n", (void *)p, i, *(p + i));        // equivalent
      //      printf("%p     ptr + %d  = %d\n", (void *)p, i, *(++p));          // buggy
    }
}
