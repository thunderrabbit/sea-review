// adapted from http://pw1.netcom.com/~tjensen/ptr/ch5x.htm

#include <stdio.h>
#include <string.h>

struct tag 
{
  char last[50];
  char first[50];
  int age;
  float rate;
};

struct tag employee;
void show_name(struct tag *p);

int main(int argc, char **argv)
{
  struct tag *emp_ptr;
  emp_ptr = &employee;
  strcpy(employee.last, "Nugen");
  strcpy(employee.first, "Rob");

  puts(employee.last);
  puts(employee.first);

  employee.age = 40;

  show_name(emp_ptr);
  return 0;
}

void show_name(struct tag *p)
{
  printf("Awesome: %s %s, ", p->first, p->last);
  printf("age %d\n\n", p->age);
}
