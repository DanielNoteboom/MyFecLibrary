#include "config.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <math.h>
#include <memory.h>
#include <sys/time.h>
#include <sys/resource.h>
#ifdef HAVE_GETOPT_H
#include <getopt.h>
#endif
#include "fec.h"
#define MAXBYTES 10000
#define NUM 1
#define DENOM 100000

int getFlips(int flips, int framebits, int* randomFlips) {
  int count = 0;
  int totalCount = 0;
  while(count < flips && totalCount < flips * 5) {
    int next = rand() % framebits;
    if(randomFlips[next] != 1) {
      randomFlips[next] = 1;
      count++;
    }
    //srand(time(NULL));
    totalCount++;
  }
  return totalCount < flips * 10;
}

int getDigits(int errCnt) {
  int count = 0;
  while(errCnt > 0) {
    errCnt /= 10;
    count++;
  }
  return count;
}
void clearRandomFlips(int* randomFlips, int framebits) {
  int i;
  for(i = 0; i < framebits; i++) {
    randomFlips[i] = 0;
  }
}

int main(int argc, char *argv[]) {
  int i, j, tr;
  int sr=0,errcnt = 0,framebits=2048;
  long long int tot_errs=0;
  unsigned char bits[MAXBYTES];
  unsigned char data[MAXBYTES];
  unsigned char xordata[MAXBYTES];
  unsigned char symbols[8*2*(MAXBYTES+6)];
  FILE* fp;
  char* newLine = "\n";
  fp = fopen("trialsoutput.txt", "w");
  
  int randomFlips[2 * (framebits + 5) + 1];
  void *vp;
  //time_t t;
  //time(&t);
  srand(time(NULL));
  if((vp = create_viterbi27(framebits)) == NULL) {
    printf("create_viterbi27 failed\n");
    exit(1);
  }
  for(tr = 0; tr < 100; tr++) {
  for(i = 0; i < 1000; i++ ) {
    //printf("entering %d stage of loop\n", i); 
    for(j = 0; j < framebits + 6; j++) {
      int bit = (j < framebits) ? (random() & 1) : 0;
      sr = (sr << 1) | bit;
      bits[j / 8] = sr & 0xff;
      symbols[2 * j + 0] = parity(sr & V27POLYA) == 0 ? 0: 255;
  //    printf("parity is %d\n", parity(sr & V27POLYA));
      symbols[2 * j + 1] = parity(sr & V27POLYB) == 0 ? 0: 255;
    }
    if(!getFlips(i, 2 * (framebits + 5) + 1, randomFlips))
      continue;
    for(j = 0; j < 2 * (framebits + 6) + 1; j++) {
      if(randomFlips[j] == 1) {
        symbols[j] = symbols[j] == 255 ? 0: 255;
      }
    }
    init_viterbi27(vp,0);

    update_viterbi27_blk(vp,symbols,framebits+6);

    chainback_viterbi27(vp,data,framebits,0);
    errcnt = 0; 
    for(j = 0; j < framebits / 8; j++) {
      int e = Bitcnt[xordata[j] = data[j] ^ bits[j]];
      errcnt += e;
      tot_errs += e;
    }
    char c[20];
    sprintf(c, "%d", errcnt);
    int digitCount = getDigits(errcnt);
    digitCount = digitCount == 0 ? 1: digitCount;
    fwrite(&c, digitCount, 1, fp);
    fputc(32, fp);
    clearRandomFlips(randomFlips, 2 * (framebits + 5) + 1); 
    //printf("%d iteration of inner loop", i);
  }
    fwrite(newLine, 1, 1, fp);
  }
  return 0;
}
