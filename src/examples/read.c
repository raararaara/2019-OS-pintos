#include <stdio.h>
#include <syscall.h>

int main (int argc, char **argv)
{
  char buf[100];
  read(1, buf, 100);
  printf("%s\n", buf);
  return 0;
}