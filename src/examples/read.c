#include <stdio.h>
#include <syscall.h>

int main (int argc, char **argv)
{
  char buf[101] = {0};
  read(0, buf, 100);
  printf("%s\n", buf);
  return 0;
}
