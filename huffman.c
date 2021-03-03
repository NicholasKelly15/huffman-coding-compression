#include "heap.h"
#include "bit_buf.h"
#include "tree.h"

/****************************************************************************************
// Compression function, runs when the argument -c is used with huffman.
// First counts the character frequencies of the file, then builds a huffman
// table based on the requencies which are inserted into a heap. The huffman
// tree is used to make a table that takes characters and outputs an encoding.
// The encoding table is then used to encode the input file and the results are
// printed into the output file.

// Args:
// FILE *input: pointer to input file to encode
// FILE *output: pointer to output file for the compressed file

// Return: void
*****************************************************************************************/
void compression(FILE *input, FILE *output)  {
	int i;
	read_bit_buffer input_buffer;
	write_bit_buffer compressed_buffer;
	int counting_bin[256];


    bitbuf_load(input, &input_buffer);

	for(i = 0; i < 256; i++)
		counting_bin[i] = 0;

	for(i = 0; i < input_buffer.last_index; i++)
		counting_bin[(int)input_buffer.buffer[i]]++;

	heapInit();

	for(i = 0; i < 256; i++)
		if(counting_bin[i])
			HeapInsert(i, NULL, counting_bin[i]);

	HeapInsert(0, NULL, 1);   // Insert end of file marker

	create_huffman_tree_from_frequencies();

	make_huffman_table();

	// Adding the header to the file (huffman tree)
	make_header_with_tree(&compressed_buffer);


	// Adding the compressed bits to the file
	reset_bitbuff_reader(&input_buffer);
	input_buffer.buffer[input_buffer.last_index] = 0; // Add EOF char to buffer
	input_buffer.last_index++; // Increment last index
	unsigned char next_char = 0;
	int encoded_bit;

	for (i = 0 ; i < input_buffer.last_index ; i++) {
		next_char = input_buffer.buffer[i];
		for (encoded_bit = 0 ; encoded_bit < 20
			&& huffman_code[next_char][encoded_bit] != -1 ; encoded_bit++) {
			bitbuf_write_bit(&compressed_buffer, huffman_code[next_char][encoded_bit]);
		}
	}

	bitbuf_store_final(output, &compressed_buffer);

}

/****************************************************************************************
// Decompression, runs when the arg -d is used with huffman.
// Builds a huffman tree based on the input file header and then
// uses the huffman tree to decode the rest of the file and return the
// characters into the output file.

// Args:
// FILE *input: pointer to the file to decode
// FILE *output: pointer to the file to return the decompressed data

// Return: void
*****************************************************************************************/
void decompression(FILE *input, FILE *output)  {

	// Make buffer to read the file.
	read_bit_buffer input_buffer;
 	bitbuf_load(input, &input_buffer);

	// Build huffman tree from header.
	decompress_header(&input_buffer);
	// Decode the file with the huffman tree.
	decompress_remaining_file(&input_buffer, output);

}

/****************************************************************************************
// Main function parsers parameters and file name(s)
// This program expects two arguments:
// The first argument specified whether to compress the input or decompress the input
*****************************************************************************************/

int main(int argc, char **argv)   {
	FILE *in_file;
	FILE *out_file;

        if (argc != 4) {
                fprintf(stderr, "Error: usage: %s -[c/d] [source] [dest]\n", argv[0]);
                fprintf(stderr, "       -c compress \n");
                fprintf(stderr, "       -d decompress \n");
                return -1;
        }

        in_file = fopen(argv[2], "rb");
        out_file = fopen(argv[3], "wb");

        if(!in_file || !out_file)   {
                fprintf(stderr, "Error: usage: %s [source] [dest]\n", argv[0]);
                return -1;
        }

	if(argv[1][1] == 'c')  {
		printf("Compress %s and store to %s\n", argv[2], argv[3]);
		compression(in_file, out_file);
	}

	if(argv[1][1] == 'd')  {
		printf("Decompress %s and store to %s\n", argv[2], argv[3]);
		decompression(in_file, out_file);
	}

        fclose(in_file);
        fclose(out_file);

        return 0;
}
