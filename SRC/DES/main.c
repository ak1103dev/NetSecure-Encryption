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

static const int DesExpansion[48] = {
  32,  1,  2,  3,  4,  5,  4,  5,  6,  7,  8,  9,
  8,  9, 10, 11, 12, 13, 12, 13, 14, 15, 16, 17,
  16, 17, 18, 19, 20, 21, 20, 21, 22, 23, 24, 25,
  24, 25, 26, 27, 28, 29, 28, 29, 30, 31, 32,  1
};

static const int DesSbox[8][4][16] = {
  {
  {14,  4, 13,  1,  2, 15, 11,  8,  3, 10,  6, 12,  5,  9,  0,  7},
  { 0, 15,  7,  4, 14,  2, 13,  1, 10,  6, 12, 11,  9,  5,  3,  8},
  { 4,  1, 14,  8, 13,  6,  2, 11, 15, 12,  9,  7,  3, 10,  5,  0},
  {15, 12,  8,  2,  4,  9,  1,  7,  5, 11,  3, 14, 10,  0,  6, 13},
  },

  {
  {15,  1,  8, 14,  6, 11,  3,  4,  9,  7,  2, 13, 12,  0,  5, 10},
  { 3, 13,  4,  7, 15,  2,  8, 14, 12,  0,  1, 10,  6,  9, 11,  5},
  { 0, 14,  7, 11, 10,  4, 13,  1,  5,  8, 12,  6,  9,  3,  2, 15},
  {13,  8, 10,  1,  3, 15,  4,  2, 11,  6,  7, 12,  0,  5, 14,  9},
  },

  {
  {10,  0,  9, 14,  6,  3, 15,  5,  1, 13, 12,  7, 11,  4,  2,  8},
  {13,  7,  0,  9,  3,  4,  6, 10,  2,  8,  5, 14, 12, 11, 15,  1},
  {13,  6,  4,  9,  8, 15,  3,  0, 11,  1,  2, 12,  5, 10, 14,  7},
  { 1, 10, 13,  0,  6,  9,  8,  7,  4, 15, 14,  3, 11,  5,  2, 12},
  },

  {
  { 7, 13, 14,  3,  0,  6,  9, 10,  1,  2,  8,  5, 11, 12,  4, 15},
  {13,  8, 11,  5,  6, 15,  0,  3,  4,  7,  2, 12,  1, 10, 14,  9},
  {10,  6,  9,  0, 12, 11,  7, 13, 15,  1,  3, 14,  5,  2,  8,  4},
  { 3, 15,  0,  6, 10,  1, 13,  8,  9,  4,  5, 11, 12,  7,  2, 14},
  },

  {
  { 2, 12,  4,  1,  7, 10, 11,  6,  8,  5,  3, 15, 13,  0, 14,  9},
  {14, 11,  2, 12,  4,  7, 13,  1,  5,  0, 15, 10,  3,  9,  8,  6},
  { 4,  2,  1, 11, 10, 13,  7,  8, 15,  9, 12,  5,  6,  3,  0, 14},
  {11,  8, 12,  7,  1, 14,  2, 13,  6, 15,  0,  9, 10,  4,  5,  3},
  },

  {
  {12,  1, 10, 15,  9,  2,  6,  8,  0, 13,  3,  4, 14,  7,  5, 11},
  {10, 15,  4,  2,  7, 12,  9,  5,  6,  1, 13, 14,  0, 11,  3,  8},
  { 9, 14, 15,  5,  2,  8, 12,  3,  7,  0,  4, 10,  1, 13, 11,  6},
  { 4,  3,  2, 12,  9,  5, 15, 10, 11, 14,  1,  7,  6,  0,  8, 13},
  },

  {
  { 4, 11,  2, 14, 15,  0,  8, 13,  3, 12,  9,  7,  5, 10,  6,  1},
  {13,  0, 11,  7,  4,  9,  1, 10, 14,  3,  5, 12,  2, 15,  8,  6},
  { 1,  4, 11, 13, 12,  3,  7, 14, 10, 15,  6,  8,  0,  5,  9,  2},
  { 6, 11, 13,  8,  1,  4, 10,  7,  9,  5,  0, 15, 14,  2,  3, 12},
  },

  {
  {13,  2,  8,  4,  6, 15, 11,  1, 10,  9,  3, 14,  5,  0, 12,  7},
  { 1, 15, 13,  8, 10,  3,  7,  4, 12,  5,  6, 11,  0, 14,  9,  2},
  { 7, 11,  4,  1,  9, 12, 14,  2,  0,  6, 10, 13, 15,  3,  5,  8},
  { 2,  1, 14,  7,  4, 10,  8, 13, 15, 12,  9,  0,  3,  5,  6, 11},
  }
};

static const int DesPbox[32] = {
  16,  7, 20, 21, 29, 12, 28, 17,  1, 15, 23, 26,  5, 18, 31, 10,
   2,  8, 24, 14, 32, 27,  3,  9, 19, 13, 30,  6, 22, 11,  4, 25
};

static const int DesFinal[64] = {
  40,  8, 48, 16, 56, 24, 64, 32, 39,  7, 47, 15, 55, 23, 63, 31,
  38,  6, 46, 14, 54, 22, 62, 30, 37,  5, 45, 13, 53, 21, 61, 29,
  36,  4, 44, 12, 52, 20, 60, 28, 35,  3, 43, 11, 51, 19, 59, 27,
  34,  2, 42, 10, 50, 18, 58, 26, 33,  1, 41,  9, 49, 17, 57, 25
};

void show(unsigned char* str, int n) {
  int i;
  for (i = 0; i < n; i++) {
    printf("%c", str[i]);
  }
  printf("\n");
}

unsigned char* charToBinary(unsigned char c) {
  char* b = malloc(8);
  int i;
  for (i = 0; i < 8; i++) {
    b[7-i] = c % 2 + '0';
    c /= 2;
  }
  return b;
}

unsigned char* strToBinary(unsigned char* str, int n) {
  char* b = malloc(8 * n);
  int i;
  for (i = 0; i < n; i++) {
    memcpy(b + 8 * i, charToBinary(str[i]), 8 * sizeof(char));
  }
  return b;
}

void xor(unsigned char* result, unsigned char* a, unsigned char* b, int n) {
  unsigned char* temp = malloc(n);
  int i;
  for (i = 0; i < n; i++) {
    if (a[i] == b[i]) {
      temp[i] = '0';
    } else {
      temp[i] = '1';
    }
  }
  memcpy(result, temp, n);
}

unsigned char* binaryToStr(unsigned char* bin, int n) {
  unsigned char* temp = malloc(n);
  int i;
  for (i = 0; i < n; i++) {
    int j;
    unsigned char byte[8];
    for (j = 0; j < 8*i + 8; j++) {
      byte[j % 8] = bin[j];
    }
    temp[i] = strtol(byte, 0, 2);
  }
  return temp;
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
  unsigned char* str = malloc(len);
  int i;
  for (i = 0; i < len; i++) {
    str[i] = 16 * hexToDec(hex[2*i]) + hexToDec(hex[2*i + 1]);
  }
  return str;
}

void rotateLeft(unsigned char* bits, int len, int n) {
  int i;
  unsigned char* temp = malloc(len);
  if (n == 1) {
    for (i = 0; i < len - 1; i++) {
      temp[i] = bits[i + 1];
    }
    temp[len - 1] = bits[0];
  } else if (n == 2) {
    for (i = 0; i < len - 2; i++) {
      temp[i] = bits[i + 2];
    }
    temp[len - 2] = bits[0];
    temp[len - 1] = bits[1];
  }
  memcpy(bits, temp, len);
}

void permute(unsigned char* bits, const int* mapping, int n) {
  unsigned char* temp = malloc(n);
  int i;
  for (i = 0; i < n; i++) {
    temp[i] = bits[mapping[i] - 1];
  }
  memcpy(bits, temp, n);
  return;
}

unsigned char* des(unsigned char* key, unsigned char* buffer, int start, int end) {
  unsigned char* text = malloc(64);

  unsigned char roundKeys[16][56];
  unsigned char lkey[28], rkey[28];
  unsigned char lblock[32], rblock[32],
                fblock[48], xblock[48],
                sblock;

  int i, j, k;
  int row, col;

  // printf("block = ");
  for (i = start; i < end; i++) {
    text[i % 8] = buffer[i];
    // printf("%x ", text[i % 8]);
  }
  // printf("\n");

  // key schedule
  key = strToBinary(key, 8);
  permute(key, PC1, 56);

  memcpy(lkey, &key[0], 28);
  memcpy(rkey, &key[28], 28);

  for (i = 0; i < 16; i++) {
    rotateLeft(lkey, 28, DesRotations[i]);
    rotateLeft(rkey, 28, DesRotations[i]);

    memcpy(roundKeys[i], lkey, 28);
    memcpy(roundKeys[i] + 28, rkey, 28);

    permute(roundKeys[i], PC2, 48);
  }

  // plaintext flow
  text = strToBinary(text, 8);
  permute(text, DesInitial, 64);

  memcpy(lblock, &text[0], 32);
  memcpy(rblock, &text[32], 32);

  for (i = 0; i < 16; i++) {
    memcpy(fblock, rblock, 32);
    permute(fblock, DesExpansion, 48);

    xor(xblock, fblock, roundKeys[i], 48);

    for (j = 0; j < 8; j++) {
      row = 2 * (xblock[6*j] - '0') + (xblock[6*j + 5] - '0');
      col = 8 * (xblock[6*j + 1] - '0') + 4 * (xblock[6*j + 2] - '0') + 2 * (xblock[6*j + 3] - '0') + (xblock[6*j + 4] - '0');
      sblock = (unsigned char)DesSbox[j][row][col];
      unsigned char* sBin = charToBinary(sblock);
      for (k = 0; k < 4; k++) {
        fblock[4*j + k] = sBin[k + 4];
      }
    }

    permute(fblock, DesPbox, 32);

    xor(fblock, fblock, lblock, 32);

    memcpy(lblock, rblock, 32);
    memcpy(rblock, fblock, 32);
  }

  memcpy(text, rblock, 32);
  memcpy(text + 32, lblock, 32);

  permute(text, DesFinal, 64);

  return binaryToStr(text, 8);
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

  FILE *write_ptr;
  write_ptr = fopen("encrypt.des","wb");

  // printf("cipher = \n");
  int num = filesize / 8;
  int reminder = filesize % 8;
  if (reminder != 0) num += 1;
  int i;
  for (i = 0; i < num; i++) {
    unsigned char buffer[8];

    fread(buffer, sizeof(buffer), 1, ptr);

    if (i == num - 1 && reminder != 0) {
      int j;
      for (j = reminder; j < 8; j++) {
        buffer[j] = 0;
      }
    }

    unsigned char* text = malloc(8);

    text = des(key, buffer, 0, 8);

    /*
    int j;
    for (j = 0; j < 8; j++) {
      printf("%x ", text[j]);
    }
    printf("\n");
    */
    fwrite(text, sizeof(text), 1, write_ptr);
  }
  fclose(write_ptr);

  return 1;
}

