#include "utils.h"
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void print_pass(int ret, int *total, int *pass) {
  ++(*total);
  if (ret == 0) {
    printf("-- Passed!\n");
    ++(*pass);
  } else {
    printf("XX FAILED!\n");
  }
  return;
}

int utils_write_test(void) {

  printf("utils_write(): \n");

  int fd = -1;
  int status = -1;

  fd = open("utils_test_data.raw", O_WRONLY | O_CREAT | O_TRUNC, 0644);

  if (fd == -1) {
    perror("open");
    return status;
  }

  double samples[1000] = {0};
  memset(&samples, 1, 1000 * sizeof(double));

  status = utils_write(fd, &samples, 1000);
  if (status != 0) {
    printf("Error code %d, Failed to write\n", status);
  }

  close(fd);

  return status;
}

int utils_read_test(void) {

  printf("utils_read(): \n");

  int fd = -1;
  int status = -1;

  fd = open("utils_test_data.raw", O_RDONLY);

  if (fd == -1) {
    perror("open");
    return status;
  }

  double *samples = malloc(1000 * sizeof(double));
  if (samples == NULL) {
    printf("Failed to allocate memory for the test buffer\n");
    return status;
  }

  size_t tmp = 0;
  status = utils_read(fd, &samples, 1000, &tmp);
  switch (status) {
  case -1:
    printf("Failed to dynamically reallocate\n");
    break;
  case -2:
    printf("Failed to read\n");
    break;
  case -3:
    printf("Failed to read the correct amount of data\n");
    break;
  case -4:
    printf("Failed to downsize\n");
    break;
  }

  free(samples);
  return status;
}

int save_signal_test(void) {

  printf("save_signal(): \n");

  int status = -1;

  Signal *signal = malloc(sizeof(*signal));
  if (signal == NULL) {
    printf("Failed to allocate memory for signal struct\n");
    return -1;
  }
  signal->data = malloc(1000 * sizeof(double));
  if (signal->data == NULL) {
    printf("Failed to allocate memory\n");
    goto exit;
  }
  signal->N = 1000;
  memset(signal->data, 0, 1000 * sizeof(double));

  status = save_signal("utils_save_signal_test.raw", signal);

exit:
  free(signal->data);
  signal->data = NULL;
  signal->N = 0;

  free(signal);

  return status;
}

int load_signal_test(void) {

  printf("load_signal(): \n");

  Signal *status = load_signal("utils_save_signal_test.raw");
  if (status->N != 1000) {
    printf("Incomplete loading of data: %d/%d\n", (int)status->N, 1000);
    return -1;
  }
  if (status == NULL) {
    return -1;
  } else {
    return 0;
  }
}

int main(void) {

  int total = 0;
  int pass = 0;

  print_pass(utils_write_test(), &total, &pass);
  print_pass(utils_read_test(), &total, &pass);
  print_pass(save_signal_test(), &total, &pass);
  print_pass(load_signal_test(), &total, &pass);

  printf("Result: %d/%d\n", pass, total);

  return 0;
}
