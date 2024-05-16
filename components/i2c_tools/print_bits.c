#include "print_bits.h"


void print_byte_as_bits(char val) {
  for (int i = 7; 0 <= i; i--) {
    printf("%c", (val & (1 << i)) ? '1' : '0');
  }
}

void print_bits(char * ty, char * val, unsigned char * bytes, size_t num_bytes) {
  printf("(%*s) %*s = [ ", 15, ty, 16, val);
  for (size_t i = 0; i < num_bytes; i++) {
    print_byte_as_bits(bytes[i]);
    printf(" ");
  }
  printf("]\n");
}


int bit_test() {
  SHOW(int, 0);
  SHOW(int, 1);
  SHOW(int, 17);
  SHOW(int, -17);
  SHOW(int, 256);
  SHOW(int, INT_MAX);
  SHOW(int, ~INT_MAX);
  SHOW(unsigned int, 17);
  SHOW(unsigned int, -17);  // no compiler error!
  SHOW(unsigned int, UINT_MAX);
  SHOW(unsigned int, UINT_MAX+1);
  SHOW(unsigned char, 255);
  SHOW(long, 17);
  SHOW(short, 17);
  SHOW(uint32_t, 17);
  SHOW(uint32_t, htonl(17));
  SHOW(uint16_t, 17*256+18);
  SHOW(uint16_t, htons(17*256+18));
  SHOW(void*, &errno);
  SHOW(unsigned int, 1 << 1);
  SHOW(unsigned int, 1 << 2);
  SHOW(unsigned int, 1 << 4);
  SHOW(unsigned int, 1 << 8);
  SHOW(unsigned int, 1 << 16);
  return 0;
}