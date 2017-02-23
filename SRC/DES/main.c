#include <stdio.h>
#include <string.h>

void main(int argc, char **argv) {
  FILE *ptr;
  int filesize;
  char* filename = argv[1];

  ptr = fopen(filename, "rb");

  fseek(ptr, 0, SEEK_END);
  filesize = ftell(ptr);
  fseek(ptr, 0, SEEK_SET);

  unsigned char buffer[filesize];

  fread(buffer, sizeof(buffer), 1, ptr);

  int i;
  for (i = 0; i < filesize; i++) {
    printf("%x ", buffer[i]);
  }
  printf("\n");
  // printf("file size = %d\n", filesize);
}

