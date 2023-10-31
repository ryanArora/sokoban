#include "random.h"
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>

#define RANDOM_SIZE 512

unsigned char r[RANDOM_SIZE];
size_t rIdx;

void my_random_init() {
  int urandom_fd = open("/dev/urandom", O_RDONLY);
  if (urandom_fd < 0)
  {
    perror("open /dev/urandom");
    exit(EXIT_FAILURE);
  }

  ssize_t bytes_read = read(urandom_fd, &r, RANDOM_SIZE);
  if (bytes_read < 0)
  {
    perror("read /dev/urandom");
    exit(EXIT_FAILURE);
  }

  rIdx = 0;
}

unsigned char my_random_next(unsigned char min, unsigned char max) {
  if (rIdx >= RANDOM_SIZE)
  {
    perror("no more random");
    exit(EXIT_FAILURE);

    my_random_init();
    return min + r[rIdx++] % (max - min);
  }
  
  return min + r[rIdx++] % (max - min);
}
