//Specificaiton of easyViterbi
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
 *         2 * 8 *(arrayBits / 8 + 6) the extra 6 bits are because the decoder
 *         takes the parity of 7 symbols so there is 6 extra bits(that are
 *         0's for these funtions. Multiply by 8 because each bit of data
 *         generates a byte of noise. For the purposes of this funciton there
 *         is no noise so the bit is either 0 or 255(a waste of space but
 *         thats how the function calls are set up for Viterbi) Multiply by
 *         2 because each bit of input produces two bits of output
 *  param: bits
 *         copy of the data bits, essentially a check to make sure the bits were 
 *         calculated correctly
 *  requires: length of data >= arrayBits / 8
 *            length of symbols >= 2 * 8 * (arrayBits / 8 + 6)
 *  modifies: symbols
 *  effects:  encodes arrayBits of data stored in data placing the encode
 *             output in the symbols array
 */
void easyVitEncoder(unsigned char data[], int arrayBits, unsigned char symbols[], unsigned char bits[]);

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
int easyVitDecoder(unsigned char symbols[], int framebits, unsigned char data[]);
#endif // EASY_VITERBI
