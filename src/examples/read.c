#include <stdio.h>
#include <syscall.h>

int main (int argc UNUSED, char **argv UNUSED)
{
  char buf[101] = {0};
  read(0, buf, 100);
  printf("%s\n", buf);
  return 0;
}
