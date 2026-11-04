#include <complex.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define PI 3.14159265358979323846

double *read_all(int fd, size_t *N) {

  size_t size = 4096 * sizeof(double);
  double *buffer = malloc(size);
  if (buffer == NULL) {
    return NULL;
  }

  unsigned char *ptr = (unsigned char *)buffer;
  size_t count = sizeof(double);
  ssize_t n = 1;
  size_t total_bytes_read = 0;

  while (n > 0) {

    if (total_bytes_read + count > size) {
      size_t new_size = size * 2;
      void *tmp = realloc(buffer, new_size);
      if (tmp == NULL) {
        free(buffer);
        return NULL;
      }
      buffer = tmp;
      size = new_size;
      ptr = (unsigned char *)buffer + total_bytes_read;
    }

    n = read(fd, ptr, count);

    if (n < 0) {
      if (errno == EINTR) {
        continue;
      } else {
        free(buffer);
        return NULL;
      }
    }

    total_bytes_read += n;
    ptr += n;
  }

  *N = total_bytes_read / sizeof(double);
  void *tmp = realloc(buffer, total_bytes_read);
  if (tmp == NULL) {
    free(buffer);
    return NULL;
  }
  buffer = tmp;

  return buffer;
}

int fft(double *output, double *input, size_t N) {
  int ret = -1;
  double complex sum = 0.0;
  double angle = 0.0;
  for (size_t k = 0; k < N; k++) {
    for (size_t i = 0; i < N; i++) {
      angle = -2.0 * PI * k * i / N;
      sum += input[i] * cexp(I * angle);
    }
    output[k] = cabs(sum);
    sum = 0;
  }
  ret = 0;
  return ret;
}

int main(void) {

  int ret = EXIT_FAILURE;
  int fd = -1;
  double *input = NULL;
  double *fft_signal = NULL;

  fd = open("chirp.raw", O_RDONLY);
  if (fd == -1) {
    perror("open");
    goto cleanup;
  }

  size_t N;
  input = read_all(fd, &N);
  if (input == NULL) {
    perror("read");
    goto cleanup;
  }
  printf("Values read: %d\n", (int)N);

  fft_signal = malloc(N * sizeof(fft_signal));
  if (fft_signal == NULL) {
    fprintf(stderr, "Failed to allocate array for the FFT\n");
    goto cleanup;
  }

  int ans = fft(fft_signal, input, N);
  if (ans == -1) {
    fprintf(stderr, "FFT Failed\n");
    goto cleanup;
  }

  if (close(fd) == -1) {
    perror("close");
    fd = -1;
    goto cleanup;
  }
  fd = -1;

  fd = open("fft.raw", O_WRONLY | O_CREAT | O_TRUNC, 0644);
  if (fd == -1) {
    perror("open");
    goto cleanup;
  }

  size_t count = N * sizeof(*fft_signal);
  const unsigned char *ptr = (const unsigned char *)fft_signal;

  while (count > 0) {

    ssize_t n = write(fd, ptr, count);

    if (n < 0) {
      if (errno == EINTR) {
        continue;
      } else {
        fprintf(stderr, "Failed to write to FFT.raw\n");
        goto cleanup;
      }
    }

    if (n == 0) {
      fprintf(stderr, "Failed to write to FFT.raw\n");
      goto cleanup;
    }

    ptr += n;
    count -= (size_t)n;
  }

  ret = EXIT_SUCCESS;
  goto cleanup;

cleanup:
  if (fd != -1) {
    close(fd);
  }
  free(input);
  free(fft_signal);
  return ret;
}
