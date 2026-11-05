#include "utils.h"

#include <errno.h>
#include <fcntl.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int utils_read(int fd, double **buffer, size_t N_in, size_t *N_out) {

  size_t size = N_in * sizeof(double);
  unsigned char *ptr = (unsigned char *)*buffer;
  size_t count = sizeof(double);
  ssize_t n = 1;
  size_t total_bytes_read = 0;
  while (n > 0) {
    if (total_bytes_read + count > size) {
      size_t new_size = size + N_in;
      double *tmp = realloc(*buffer, new_size);
      if (tmp == NULL) {
        return -1;
      }
      *buffer = tmp;
      size = new_size;
      ptr = (unsigned char *)*buffer + total_bytes_read;
    }
    n = read(fd, ptr, count);
    if (n < 0) {
      if (errno == EINTR) {
        continue;
      } else {
        perror("read");
        return -2;
      }
    }
    total_bytes_read += n;
    ptr += n;
  }

  *N_out = total_bytes_read / sizeof(double);

  double *tmp = realloc(*buffer, total_bytes_read);
  if (tmp == NULL) {
    return -3;
  }
  *buffer = tmp;
  return 0;
}

int utils_write(int fd, const void *buffer, size_t N_in) {
  size_t count = N_in * sizeof(double);
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
      return -2;
    }
    ptr += n;
    count -= (size_t)n;
  }
  return 0;
}

Signal *load_signal(const char *path) {
  int fd = -1;
  fd = open(path, O_RDONLY);
  if (fd == -1) {
    perror("open");
    return NULL;
  }

  Signal *signal = malloc(sizeof(*signal));
  if (signal == NULL) {
    printf("Failed to allocate memory for signal struct\n");
    close(fd);
  }
  signal->data = malloc(4096 * sizeof(double));
  if (signal->data == NULL) {
    printf("Failed to allocate memory\n");
    free(signal);
    close(fd);
    return NULL;
  }
  signal->N = 4096;

  int ret = utils_read(fd, &(signal->data), 4096, &(signal->N));
  if (ret != 0) {
    printf("Error code %d, Failed to read from %s\n", ret, path);
    free(signal->data);
    signal->data = NULL;
    signal->N = 0;
    free(signal);
    close(fd);
    return NULL;
  }
  close(fd);
  return signal;
}

int save_signal(const char *path, Signal *signal) {
  int fd = -1;
  fd = open(path, O_WRONLY | O_CREAT | O_TRUNC, 0644);
  if (fd == -1) {
    perror("open");
    return -1;
  }
  int ret = utils_write(fd, signal->data, signal->N);
  if (ret != 0) {
    printf("Failed to write to %s", path);
    close(fd);
    return -2;
  }
  close(fd);
  return 0;
}
