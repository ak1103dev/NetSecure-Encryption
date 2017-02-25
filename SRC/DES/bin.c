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

void main()
{
  char hex[16] = "1234567887654321";
  char data[] = {'0', '1', '0', '1', '0', '1', '1', '0'};
  char c = strtol(data, 0, 2);
  printf("%s = %c = %d = 0x%.2X\n", data, c, c, c);

  printf("ctb = %s\n", charToBinary(c));
  printf("stb = %s\n", strToBinary(data));
  unsigned char* ans = malloc(8);
  ans  = hexToStr(hex);
  int i;
  for (i = 0; i < 8; i++) {
    printf("%x ", ans[i]);
  }
}
