// from http://pw1.netcom.com/~tjensen/ptr/ch1x.htm
#include <stdio.h>

int j;
int *pk;

int main(int argc, char **argv)
{
  int k;
  int *pj;

  j = 2;
  k = 7;

  pk = &k;
  pj = &j;

  printf("the value of j is %d and its address is %p\n", j, (void *)&j);
  printf("the value of k is %d and its address is %p\n", k, (void *)&k);
  printf("the value of pj is %p and its address is %p\n", pj, (void *)&pj);
  printf("the value of pk is %p and its address is %p\n", pk, (void *)&pk);
  printf("the value stored at pj is %d\n", *pj);
  printf("the value stored at pk is %d\n", *pk);
  return 0;
}
