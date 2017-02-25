#include <stdlib.h>
#include <stdio.h>
#include <string.h>

static const int DesInitial[64] = {
  58, 50, 42, 34, 26, 18, 10,  2, 60, 52, 44, 36, 28, 20, 12,  4,
  62, 54, 46, 38, 30, 22, 14,  6, 64, 56, 48, 40, 32, 24, 16,  8,
  57, 49, 41, 33, 25, 17,  9,  1, 59, 51, 43, 35, 27, 19, 11,  3,
  61, 53, 45, 37, 29, 21, 13,  5, 63, 55, 47, 39, 31, 23, 15,  7
};

unsigned char* charToBinary(unsigned char c) {
  char* b = malloc(8);
  int i;
  for (i = 0; i < 8; i++) {
    b[7-i] = c % 2 + '0';
    c /= 2;
  }
  return b;
}

unsigned char* strToBinary(unsigned char* str) {
  int strlen = sizeof(str);
  char* b = malloc(8 * strlen);
  int i;
  for (i = 0; i < strlen; i++) {
    memcpy(b + 8 * i, charToBinary(str[i]), 8 * sizeof(char));
  }
  return b;
}

void permutation(unsigned char* bits, const int* mapping, int n) {
  unsigned char* temp = malloc(n);
  unsigned char* bin = strToBinary(bits);
  int i;
  printf("perm\n");
  for (i = 0; i < n; i++) {
    temp[i] = bin[mapping[i] - 1];
    printf("%c", temp[i]);
  }
  printf("\n");
}

unsigned char* des(unsigned char* buffer, int start, int end) {
  unsigned char* text = malloc(8);
  unsigned char lblock[4], rblock[4];
  int i;
  for (i = start; i < end; i++) {
    text[i % 8] = buffer[i];
    printf("%x ", text[i % 8]);
  }
  printf("\n");

  permutation(text, DesInitial, 64);

  memcpy(lblock, &text[0], 4);
  memcpy(rblock, &text[4], 4);

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

  /*
  int i;
  for (i = 0; i < 8; i++) {
    printf("%x ", text[i]);
  }
  */

  return 1;
}

