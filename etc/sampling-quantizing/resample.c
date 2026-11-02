#include "utils/data.h"
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

int main(void) {
  int fd1 = open("data.raw", O_RDONLY);
  int fd2 = open("data.downsampled", O_RDONLY | O_WRONLY | O_CREAT, 0644);
  if (fd1 == -1 || fd2 == -1) {
    perror("open");
    return 1;
  }

  size_t N = (size_t)(5 * 250);
  float data[N];
  float sample;
  size_t N_new;
  for (size_t i = 0; i < N; i++) {
    utils_read(fd1, &sample);
    data[i] = sample;
  }

  float *resampled = utils_resample(data, N, 250.0, 100.0, &N_new);

  for (size_t i = 0; i < N_new; i++) {
    utils_write(fd2, &resampled[i]);
  }

  close(fd1);
  close(fd2);
  return 0;
}
