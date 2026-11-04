#include <fcntl.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "utils/data.h"

#define PI 3.14159265358979323846

int main(void) {
  int fd = open("data.raw", O_WRONLY | O_CREAT, 0644);
  if (fd == -1) {
    perror("open");
    return 1;
  }

  int f = 10;   // 10 Hz
  int fs = 250; // 250 Hz
  int t = 5;    // 5s recording

  float sample;

  size_t N = (size_t)(t * fs);

  for (size_t i = 0; i < N; i++) {
    sample = sinf(2.0f * (float)PI * f * i / fs);
    utils_write(fd, &sample);
  }

  return 0;
}
