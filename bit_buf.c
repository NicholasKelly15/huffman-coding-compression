#include "bit_buf.h"


// Read one bit from the buffer
int bitbuf_read_bit(read_bit_buffer *bb)  {
	unsigned char c;

        if (bb->next_index == bb->last_index)
                return -1;

       	c = bb->buffer[bb->next_index];
        c = (c >> bb->next_bit) & 0x01;

	bb->next_bit = (bb->next_bit + 1) % BITS_IN_BUF;
	if(bb->next_bit == 0)
		bb->next_index++;

        return (int) c;
}

// Load a file into the buffer
int bitbuf_load(FILE *fp, read_bit_buffer *bb) {
	int c;
    bb->next_index = bb->last_index = 0;
	bb->next_bit = 0;

        while(bb->last_index < BUFFER_SIZE)    {
                c = fread(&bb->buffer[bb->last_index], sizeof(char), 1, fp);
		if(c)
			bb->last_index++;
		else
			break;
	}

        return bb->last_index;
}

void reset_bitbuff_reader(read_bit_buffer *bb) {
	bb->next_index = 0;
	bb->next_bit = 0;
}

// Write one bit to the buffer
int bitbuf_write_bit(write_bit_buffer *bb, unsigned char bit) {
	unsigned char mask =  0X1 << bb->next_bit;

	if(bb->next_index == BUFFER_SIZE)
	       return -1;

	if(bit == 0)
		bb->buffer[bb->next_index] &= (~mask);
	else if(bit == 1)
		bb->buffer[bb->next_index] |= mask;
	else
		fprintf(stderr, "attemp to write not a bit\n");


	bb->next_bit++;
	bb->next_bit = bb->next_bit % BITS_IN_BUF;

	if(bb->next_bit == 0)
		bb->next_index++;
	return 1;
}

// Store what is in the buffer to a file
int bitbuf_store(FILE *fp, write_bit_buffer *bb) {
	int i;
	for(i = 0; i < bb->next_index; i++)  {
                fwrite(&bb->buffer[i], sizeof(char), 1, fp);
        }
	bb->next_index = 0;
	bb->next_bit = 0;
        return i;
}

int bitbuf_store_final(FILE *fp, write_bit_buffer *bb) {
	while (bb->next_bit != 0) {
		bitbuf_write_bit(bb, 0);
	}

	int i;
	for(i = 0; i < bb->next_index; i++)  {
                fwrite(&bb->buffer[i], sizeof(char), 1, fp);
        }
	bb->next_index = 0;
	bb->next_bit = 0;
        return i;
}

/*
int main(int argc, char **argv)
{
    FILE *in_file, *out_file;
	int bit;
	read_bit_buffer input_buffer;
    write_bit_buffer output_buffer;


        if (argc != 3) {
                fprintf(stderr, "Error: usage: %s [source] [dest]\n", argv[0]);
                return -1;
        }


	in_file = fopen(argv[1], "rb");
	out_file = fopen(argv[2], "wb");

	if(!in_file || !out_file)   {
		fprintf(stderr, "Error: usage: %s [source] [dest]\n", argv[0]);
                return -1;
	}

	bitbuf_load(in_file, &input_buffer);

	// printf("%d\n", bitbuf_read_bit(&input_buffer));

	while((bit = bitbuf_read_bit(&input_buffer)) != -1)  {
		// printf("%d\n", bit);
		// bitbuf_write_bit(&output_buffer, bit);
	}

	for(bit = 0; bit < input_buffer.last_index; bit++)
		// printf("%x %x\n", input_buffer.buffer[bit], output_buffer.buffer[bit]);

	bitbuf_store(out_file, &output_buffer);

	fclose(in_file);
	fclose(out_file);

	return 0;
}
*/
