#include <errno.h>
#include <fcntl.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

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

int delay(double *input, double *output, size_t N, size_t D, double A,
          double a) {
  double w;
  for (size_t i = 0; i < N; i++) {
    w = A * 2.0f * ((float)rand() / RAND_MAX) - 1.0f;
    if (i < D) {
      output[i] = w;
    } else {
      output[i] = a * input[i - D] + w;
    }
  }
  return 0;
}

int write_all(int fd, const void *buffer, size_t count) {
  const unsigned char *ptr = buffer;
  while (count > 0) {
    ssize_t n = write(fd, ptr, count);
    if (n < 0) {
      if (errno == EINTR) {
        continue;
      } else {
        return -1;
      }
    }
    if (n == 0) {
      return -1;
    }
    ptr += n;
    count -= (size_t)n;
  }
  return 0;
}

int main(void) {

  int ret = EXIT_FAILURE;
  int fd = -1;
  double *input = NULL;
  double *output = NULL;

  size_t D = 500;
  double A = 0.2;
  double a = 0.9;

  fd = open("chirp.raw", O_RDONLY, 0644);
  if (fd == -1) {
    perror("open");
    goto cleanup;
  }

  size_t N = 0;
  input = read_all(fd, &N);
  if (input == NULL) {
    fprintf(stderr, "Failed to read from chirp.raw\n");
    goto cleanup;
  }

  output = malloc(N * sizeof(*output));
  if (output == NULL) {
    fprintf(stderr, "Failed to allocate for delayed signal\n");
    goto cleanup;
  }
  int ans = delay(input, output, N, D, A, a);

  if (close(fd) == -1) {
    perror("close");
    goto cleanup;
  }

  fd = -1;
  fd = open("received.raw", O_WRONLY | O_CREAT | O_TRUNC, 0644);
  if (fd == -1) {
    perror("close");
    goto cleanup;
  }

  ans = write_all(fd, output, N * sizeof(*output));
  if (ans == -1) {
    fprintf(stderr, "Failed to write to received.raw\n");
    goto cleanup;
  }

  ret = EXIT_SUCCESS;

cleanup:
  if (close(fd) == -1) {
    perror("close");
  }
  free(input);
  free(output);
  return ret;
}
