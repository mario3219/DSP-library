#include "utils/data.h"

#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

int main(void) {

  const char *path = "../../../../data/read-write_test.txt";
  int fd = open(path, O_RDWR | O_CREAT, 0644);

  if (fd == -1) {
    perror("open");
    return 1;
  }

  float value = 100.00;
  float tmp = 0.0;

  utils_write(fd, &value);

  printf("Write: %.2f\n", value);

  lseek(fd, 0, SEEK_SET);

  utils_read(fd, &tmp);

  printf("Read: %.2f\n", tmp);

  close(fd);

  return 0;
}
