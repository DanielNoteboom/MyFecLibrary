//Implementation of Viterbi Decoding
#include "easyViterbi.h"
#include <stdlib.h>
#include <stdio.h>
void easyVitEncoder(unsigned char data[], int arrayBits, unsigned char symbols[], unsigned char bits[]) {
  int sr = 0;
  int i = 0;
  for(i = 0; i < arrayBits + 6; i++) {
    int bit = i < arrayBits ? ((data[i / 8]) >> (i % 8)) & 1 : 0;
    sr = (sr << 1) | bit;
    bits[i / 8] = sr & 0xFF;
    symbols[2 * i + 0] = parity(sr & V27POLYA) == 0 ? 0: 255;
    symbols[2 * i + 1] = parity(sr & V27POLYB) == 0 ? 0: 255;
  }
}

//simple private method to flip the bits
//at the given point. Needed because
//the viterbi algorithm flips the bits
void flipBits(unsigned char* dataPoint) {
  unsigned char data = *dataPoint;
  unsigned char newData = 0;
  int i = 0;
  for(i = 7; i >= 0; i--) {
    if(i >= 4) {
      newData = newData | (((1 << i) & data) >> (7 - (7 - i) * 2));
    } else {
      newData = newData | (((1 << i) & data) << (7 - 2 * i));
    }
  }
  *dataPoint = newData;
}
int easyVitDecoder(unsigned char symbols[], int framebits, unsigned char data[]) {
  void* vp;
  unsigned char test = 53;
  flipBits(&test);
  if((vp = create_viterbi27(framebits)) == NULL) {
    printf("create_viterbi27 failed\n");
    return 1;
  }
  init_viterbi27(vp, 0);
  update_viterbi27_blk(vp, symbols, framebits + 6);
  chainback_viterbi27(vp, data, framebits, 0);
  int i = 0;
  for(i = 0; i < framebits / 8; i++) {
    flipBits(data + i);
  }
  delete_viterbi27(vp);
  return 0;
}
