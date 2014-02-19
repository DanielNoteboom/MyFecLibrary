#include "easyViterbi.h"
#define SIZE 1000
int main(int argc, char** argv) {
  unsigned char data[SIZE];
  unsigned char bits[SIZE];
  int framebits = SIZE * 8;
  unsigned char symbols[8 * 2 * (SIZE + 6)];
  int i = 0;
  for(i = 0; i < SIZE; i++) {
    data[i] = i % 256;
  }
  unsigned char data2[SIZE];
  easyVitEncoder(data, framebits, symbols, bits);
  easyVitDecoder(symbols, framebits, data2);
  int errors = 0;
  for(i = 0; i < SIZE; i++) {
    errors += Bitcnt[data[i] ^ data2[i]];
  }
  printf("The amount of errors is equal to %d\n", errors);
  return 0;
}
