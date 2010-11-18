// adapted from http://pw1.netcom.com/~tjensen/ptr/ch3x.htm

#include <stdio.h>

char strA[80] = "this is a string declared globally, and we're going to erase it..";
char strB[80];

int main(void)
{
  char *pA;
  char *pB;
  pA = strA;
  pB = strB;

  puts(strA);
  puts(pA);
  putchar('\n');

  while(*pA != '\0')
    {
      //      *pB++ = *pA++; //  copy same string
      *pB++ = ++(*pA++);   // copy magic-ring  encoded string
    }
  *pB = '\0';
  puts(strB);

  putchar('\n');

  while((pB-1) > strB)
    {
      *(pB -= 2) = '\0';
      puts(strB);
    }
}
