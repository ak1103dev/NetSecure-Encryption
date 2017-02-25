#include <stdlib.h>
#include <stdio.h>
#include <string.h>

static const int PC1[56] = {
  57, 49, 41, 33, 25, 17,  9,  1, 58, 50, 42, 34, 26, 18,
  10,  2, 59, 51, 43, 35, 27, 19, 11,  3, 60, 52, 44, 36,
  63, 55, 47, 39, 31, 23, 15,  7, 62, 54, 46, 38, 30, 22,
  14,  6, 61, 53, 45, 37, 29, 21, 13,  5, 28, 20, 12,  4
};
static const int DesRotations[16] = {
  1, 1, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 1
};
static const int PC2[48] = {
  14, 17, 11, 24,  1,  5,  3, 28, 15,  6, 21, 10,
  23, 19, 12,  4, 26,  8, 16,  7, 27, 20, 13,  2,
  41, 52, 31, 37, 47, 55, 30, 40, 51, 45, 33, 48,
  44, 49, 39, 56, 34, 53, 46, 42, 50, 36, 29, 32
};

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

unsigned char hexToDec(char x) {
  switch (x) {
    case 'a':
    case 'A':
      return 10;
    case 'b':
    case 'B':
      return 11;
    case 'c':
    case 'C':
      return 12;
    case 'd':
    case 'D':
      return 13;
    case 'e':
    case 'E':
      return 14;
    case 'f':
    case 'F':
      return 15;
    default:
      return x - '0';
  }
}
unsigned char* hexToStr(char* hex) {
  int len = 8;
  printf("len = %d", len);
  unsigned char* str = malloc(len);
  int i;
  for (i = 0; i < len; i++) {
    str[i] = 16 * hexToDec(hex[2*i]) + hexToDec(hex[2*i + 1]);
    printf("str = %d\n", str[i]);
  }
  return str;
}

void permute(unsigned char* bits, const int* mapping, int n) {
  unsigned char* temp = malloc(n);
  unsigned char* bin = strToBinary(bits);
  int i;
  printf("perm\n");
  for (i = 0; i < n; i++) {
    temp[i] = bin[mapping[i] - 1];
    // printf("%c", temp[i]);
  }
  // printf("\n");
  memcpy(bits, temp, n);
  return;
}

unsigned char* des(unsigned char* key, unsigned char* buffer, int start, int end) {
  unsigned char roundKeys[16][7];
  unsigned char* text = malloc(8);
  unsigned char lblock[4], rblock[4];
  int i;
  for (i = start; i < end; i++) {
    text[i % 8] = buffer[i];
    printf("%x ", text[i % 8]);
  }
  printf("\n");

  permute(key, PC1, 56);
  for (i = 0; i < 56; i++) {
    printf("%c", key[i]);
  }
  printf("\n");
 // for (i = 0; i < 16; i++) {
  //}

  permute(text, DesInitial, 64);

  memcpy(lblock, &text[0], 4);
  memcpy(rblock, &text[4], 4);

  return text;
}

int main(int argc, char **argv) {
  if (argc < 3) {
    printf("error\n");
    return 0;
  }
  unsigned char* keyHex = argv[2];
  unsigned char* key = hexToStr(keyHex);
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

  printf("key = %s\n", key);
  text = des(key, buffer, 0, 8);

  /*
  int i;
  for (i = 0; i < 8; i++) {
    printf("%x ", text[i]);
  }
  */

  return 1;
}

