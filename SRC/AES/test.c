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
static void test_encrypt_ecb_verbose(void);

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

    fread(buffer, sizeof(buffer), 1, ptr);

    /*
    printf("plaintext = ");
    int j;
    for (j = 0; j < 64; j++) {
      printf("%.2x ", buffer[j]);
    }
    printf("\n");
    */

    if (i == num - 1 && reminder != 0) {
      for (j = reminder; j < 64; j++) {
        buffer[j] = 0;
      }
    }

    fwrite(buffer, sizeof(buffer), 1, write_ptr);
    //free(buffer);
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

static void test_encrypt_ecb_verbose(void) {
    // Example of more verbose verification

    // uint8_t i, buf[64], buf2[64];
    uint8_t i, buf[64];

    // 128bit key
    uint8_t key[16] =        { (uint8_t) 0x2b, (uint8_t) 0x7e, (uint8_t) 0x15, (uint8_t) 0x16, (uint8_t) 0x28, (uint8_t) 0xae, (uint8_t) 0xd2, (uint8_t) 0xa6, (uint8_t) 0xab, (uint8_t) 0xf7, (uint8_t) 0x15, (uint8_t) 0x88, (uint8_t) 0x09, (uint8_t) 0xcf, (uint8_t) 0x4f, (uint8_t) 0x3c };
    // 512bit text
    uint8_t plain_text[64] = { (uint8_t) 0x6b, (uint8_t) 0xc1, (uint8_t) 0xbe, (uint8_t) 0xe2, (uint8_t) 0x2e, (uint8_t) 0x40, (uint8_t) 0x9f, (uint8_t) 0x96, (uint8_t) 0xe9, (uint8_t) 0x3d, (uint8_t) 0x7e, (uint8_t) 0x11, (uint8_t) 0x73, (uint8_t) 0x93, (uint8_t) 0x17, (uint8_t) 0x2a,
                               (uint8_t) 0xae, (uint8_t) 0x2d, (uint8_t) 0x8a, (uint8_t) 0x57, (uint8_t) 0x1e, (uint8_t) 0x03, (uint8_t) 0xac, (uint8_t) 0x9c, (uint8_t) 0x9e, (uint8_t) 0xb7, (uint8_t) 0x6f, (uint8_t) 0xac, (uint8_t) 0x45, (uint8_t) 0xaf, (uint8_t) 0x8e, (uint8_t) 0x51,
                               (uint8_t) 0x30, (uint8_t) 0xc8, (uint8_t) 0x1c, (uint8_t) 0x46, (uint8_t) 0xa3, (uint8_t) 0x5c, (uint8_t) 0xe4, (uint8_t) 0x11, (uint8_t) 0xe5, (uint8_t) 0xfb, (uint8_t) 0xc1, (uint8_t) 0x19, (uint8_t) 0x1a, (uint8_t) 0x0a, (uint8_t) 0x52, (uint8_t) 0xef,
                               (uint8_t) 0xf6, (uint8_t) 0x9f, (uint8_t) 0x24, (uint8_t) 0x45, (uint8_t) 0xdf, (uint8_t) 0x4f, (uint8_t) 0x9b, (uint8_t) 0x17, (uint8_t) 0xad, (uint8_t) 0x2b, (uint8_t) 0x41, (uint8_t) 0x7b, (uint8_t) 0xe6, (uint8_t) 0x6c, (uint8_t) 0x37, (uint8_t) 0x10 };

    memset(buf, 0, 64);
    // memset(buf2, 0, 64);

    // print text to encrypt, key and IV
    printf("ECB encrypt verbose:\n\n");
    printf("plain text:\n");
    for(i = (uint8_t) 0; i < (uint8_t) 4; ++i)
    {
        phex(plain_text + i * (uint8_t) 16);
    }
    printf("\n");

    printf("key:\n");
    phex(key);
    printf("\n");

    // print the resulting cipher as 4 x 16 byte strings
    printf("ciphertext:\n");
    for(i = 0; i < 4; ++i)
    {
        AES128_ECB_encrypt(plain_text + (i*16), key, buf+(i*16));
        phex(buf + (i*16));
    }
    printf("\n");
}

