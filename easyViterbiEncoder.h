#ifndef EASY_VITERBI
#define EASY_VITERBI

#include <stdio.h>
#include "config.h"
#include "fec.h"
/** param: data
 *         the array that holds the data that you want encoded according
 *         to viterbi encoding with n = 2 and k = 7
 *  param: arrayBits
 *         how many bits are stored in data(equivalent to len(data) * 8)
 *  param: symbols
 *         holds the encoded data. Must be an array that is size of
 *         2 * 8 *(arrayBits + 6) the extra 6 bits are because the decoder
 *         takes the parity of 7 symbols so there is 6 extra bits(that are
 *         0's for these funtions. Multiply by 8 because each bit of data
 *         generates a byte of noise. For the purposes of this funciton there
 *         is no noise so the bit is either 0 or 255(a waste of space but
 *         thats how the function calls are set up for Viterbi) Multiply by
 *         2 because each bit of input produces two bits of output
 *  requires: length of data >= arrayBits * 8
 *            length of symbols >= 2 * 8 * (arrayBits + 6)
 *  modifies: symbols
 *  effects:  encodes arrayBits of data stored in data placing the encode
 *             output in the symbols array
 */
void easyVitEncoder(char[] data, int arrayBits, char[] symbols) {
  int sr = 0;
  for(int i = 0; i < arrayBits + 6; i++) {
    int bit = i < arrayBits ? (data[i / 8] >> ((i % 8) & 1)) : 0;
    sr = (sr << 1) | bit;
    symbols(2 * i + 0] = parity(sr & V27POLYA) == 0 ? 0: 255;
    symbols(2 * i + 1] = parity(sr & V27POLYB) == 0 ? 0: 255;
  }
}
/** param: symbols
 *         encoded symbols that need to be decoded
 *  param: framebits
 *         the amount of bits to be decoded in the symbols array
 *  param: output of the resulting symbols
 *  requires: symbols was encodede using framebits worth of data using
 *            viterbi with n = 2 and k = 7
 *            length of data >= framebits * 8
 *  modifies: data
 *  effects: places the result of decoding framebits worth of bytes
 *  in the symbosl array into the data array
 *  if viterbi fails for any reason the method prints a message and exits
 */
int easyVitDecoder(char[] symbols, int framebits, char[] data) {
  void* vp;
  if(vp = create_viterbi27(framebits)) == NULL) {
    printf("create_viterbi27 failed\n");
    exit(1)
  }
  init_viterbi27(vp, 0);
  update_viterbi27(vp, symbols, framebits);
  chainback_viterbi27(vp, data, framebits, 0);
  delete_viterbi27(vp);
}
#endif // EASY_VITERBI
