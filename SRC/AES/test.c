#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>

// Enable both ECB and CBC mode. Note this can be done before including aes.h or at compile-time.
// E.g. with GCC by using the -D flag: gcc -c aes.c -DCBC=0 -DECB=1
#define ECB 1

#include "aes.h"

static void strToHex(unsigned char* str, uint8_t* hex);
static void phex(uint8_t* str);

int main(int argc, char **argv) {
  if (argc < 4) {
    printf("error\n");
    return 0;
  }
  unsigned char* keyHex = argv[2];
  uint8_t key[16];
  strToHex(keyHex, key);

  printf("key = ");
  phex(key);

  FILE *ptr;
  int filesize;
  char* filename = argv[1];

  ptr = fopen(filename, "rb");

  fseek(ptr, 0, SEEK_END);
  filesize = ftell(ptr);
  fseek(ptr, 0, SEEK_SET);

  FILE *write_ptr;
  write_ptr = fopen(argv[3],"wb");

  int num = filesize / 64;
  int reminder = filesize % 64;
  if (reminder != 0) num += 1;
  int i, j;
  for (i = 0; i < num; i++) {
    uint8_t buffer[64];
    uint8_t cipher[64];

    memset(cipher, 0, 64);

    fread(buffer, sizeof(buffer), 1, ptr);

    if (i == num - 1 && reminder != 0) {
      for (j = reminder; j < 64; j++) {
        buffer[j] = 0;
      }
    }

    for (j = 0; j < 4; j++) {
      AES128_ECB_encrypt(buffer + (j*16), key, cipher + (j*16));
    }

    fwrite(cipher, sizeof(cipher), 1, write_ptr);
  }
  fclose(write_ptr);

  return 1;
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
static void strToHex(unsigned char* str, uint8_t* hex) {
  int i;
  for (i = 0; i < 16; i++) {
    hex[i] = 16 * hexToDec(str[2*i]) + hexToDec(str[2*i + 1]);
  }
}

// prints string as hex
static void phex(uint8_t* str) {
  unsigned char i;
  for(i = 0; i < 16; ++i)
    printf("%.2x ", str[i]);
  printf("\n");
}

