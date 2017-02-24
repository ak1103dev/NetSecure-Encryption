#include <stdlib.h>
#include <stdio.h>

unsigned char* des(unsigned char* buffer, int start, int end) {
  unsigned char text[8];
  int i;
  for (i = start; i < end; i++) {
    text[i % 8] = buffer[i];
    printf("%x ", text[i % 8]);
  }
  printf("\n");
  return text;
}

int main(int argc, char **argv) {
  if (argc < 2) {
    printf("error\n");
    return 0;
  }
  FILE *ptr;
  int filesize;
  char* filename = argv[1];

  ptr = fopen(filename, "rb");

  fseek(ptr, 0, SEEK_END);
  filesize = ftell(ptr);
  fseek(ptr, 0, SEEK_SET);

  unsigned char buffer[filesize];

  fread(buffer, sizeof(buffer), 1, ptr);

  unsigned char* text = malloc(8);
  text = des(buffer, 0, 8);

  int i;
  for (i = 0; i < 8; i++) {
    printf("%x ", text[i]);
  }

  return 1;
}

