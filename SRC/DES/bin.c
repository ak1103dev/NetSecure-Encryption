#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* charToBinary(char c) {
  char* b = malloc(8);
  int i;
  for (i = 0; i < 8; i++) {
    b[7-i] = c % 2 + '0';
    c /= 2;
  }
  return b;
}

char* strToBinary(char* str) {
  int strlen = sizeof(str);
  char* b = malloc(8 * strlen);
  int i;
  for (i = 0; i < strlen; i++) {
    memcpy(b + 8 * i, charToBinary(str[i]), 8 * sizeof(char));
  }
  return b;
}

void main()
{
  char data[] = {'0', '1', '0', '1', '0', '1', '1', '0'};
  char c = strtol(data, 0, 2);
  printf("%s = %c = %d = 0x%.2X\n", data, c, c, c);

  printf("ctb = %s\n", charToBinary(c));

  printf("stb = %s\n", strToBinary(data));
}
