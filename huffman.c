#include "heap.h"
#include "bit_buf.h"
#include "tree.h"

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

	HeapNode min1;
	HeapNode min2;
	HeapNode *min1_ptr;
	HeapNode *min2_ptr;
	while (heapSize > 1) {
		min1 = DeleteMin();
		min2 = DeleteMin();
		min1_ptr = &min1;
		min2_ptr = &min2;
		huffman_tree_node *new_node = merge_nodes(min1_ptr, min2_ptr);
		HeapInsert('~', new_node, min1.freq + min2.freq);
	}
	heapPrint();

	huffman_tree_node *main_huff_node;
	main_huff_node = DeleteMin().t;
	set_huffman_tree_top_node(main_huff_node);

	make_huffman_table();

	// Adding the header to the file (huffman tree)
	make_header_with_tree(&compressed_buffer);


	// Adding the compressed bits to the file
	reset_bitbuff_reader(&input_buffer);
	unsigned char next_char = 0;
	int next_bit;
	int increment = 0;
	while ((next_bit = bitbuf_read_bit(&input_buffer)) != -1) {
		next_char = ((next_char >> 1) | (next_bit * 128)); // Constructs the character with input_buffer
		increment++;
		if (increment == 8) {
			increment = 0;
			for (int encoded_bit = 0 ; encoded_bit < 20
				&& huffman_code[next_char][encoded_bit] != -1 ; encoded_bit++) {
				bitbuf_write_bit(&compressed_buffer, huffman_code[next_char][encoded_bit]);
			}
		}
	}
	// Adding the EOF character
	for (i = 0 ; i < 20 && huffman_code[0][i] != -1 ; i++) {
		bitbuf_write_bit(&compressed_buffer, huffman_code[0][i]);
	}

	bitbuf_store_final(output, &compressed_buffer);

	printf("\n\n");

}

void decompression(FILE *input, FILE *output)  {

 read_bit_buffer input_buffer;
 bitbuf_load(input, &input_buffer);

 decompress_header(&input_buffer);
 decompress_remaining_file(&input_buffer, output);

 printf("\n\n");

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
