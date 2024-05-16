#include <stdio.h>

#include <stdio.h>
#include <stdint.h>
#include <limits.h>
#include <arpa/inet.h>
#include <errno.h>
void print_bits(char * ty, char * val, unsigned char * bytes, size_t num_bytes);
#define SHOW(T,V) do { T x = V; print_bits(#T, #V, (unsigned char*) &x, sizeof(x)); } while(0);
int bit_test();
