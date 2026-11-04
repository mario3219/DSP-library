#include <errno.h>
#include <fcntl.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define PI 3.14159265358979323846

int chirp(double *output, size_t N_out, double fs, double f0, double k) {
  for (size_t i = 0; i < N_out; i++) {
    output[i] = sin(2 * PI * (f0 * (i / fs) + (k / 2) * pow((i / fs), 2)));
  }
  return 0;
}

int main(void) {

  int ret = EXIT_FAILURE;

  int fd = -1;
  fd = open("chirp.raw", O_WRONLY | O_TRUNC | O_CREAT, 0644);

  if (fd == -1) {
    perror("open");
    goto cleanup;
  }

  size_t N_out = 2500;
  double f0 = 10.0;
  double f1 = 100.0;
  double T = 5.0;
  double fs = 500.0;

  double k = (f1 - f0) / T;

  double *output = malloc(N_out * sizeof(*output));
  if (output == NULL) {
    fprintf(stderr, "Allocating array failed\n");
    goto cleanup;
  }
  int ans = chirp(output, N_out, fs, f0, k);
  if (ans != 0) {
    fprintf(stderr, "Generating signal failed\n");
    goto cleanup;
  }

  const unsigned char *ptr = (const unsigned char *)output;
  size_t count = N_out * sizeof(*output);
  while (count > 0) {
    ssize_t n = write(fd, ptr, count);
    if (n < 0) {
      if (errno == EINTR) {
        continue;
      } else {
        fprintf(stderr, "Failed to write data\n");
        goto cleanup;
      }
    }
    if (n == 0) {
      fprintf(stderr, "Failed to write data\n");
      goto cleanup;
    }
    ptr += n;
    count -= (size_t)n;
  }

  ret = EXIT_SUCCESS;

cleanup:
  if (fd != -1) {
    close(fd);
  }
  if (output != NULL) {
    free(output);
  }

  return ret;
}
