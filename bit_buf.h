#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#ifndef BIT_BUF_H
#define BIT_BUF_H

#define BUFFER_SIZE 1000000
#define BITS_IN_BUF 8

typedef struct r_buffer {
 	char buffer[BUFFER_SIZE];
	int next_index;
	int next_bit;
	int last_index;
} read_bit_buffer;

typedef struct w_buffer {
        char buffer[BUFFER_SIZE];
        int next_index;
        int next_bit;
} write_bit_buffer;

int bitbuf_read_bit(read_bit_buffer *bb);

int bitbuf_load(FILE *fp, read_bit_buffer *bb);

void reset_bitbuff_reader(read_bit_buffer *bb);

int bitbuf_write_bit(write_bit_buffer *bb, unsigned char bit);

int bitbuf_store(FILE *fp, write_bit_buffer *bb);

int bitbuf_store_final(FILE *fp, write_bit_buffer *bb);

#endif
