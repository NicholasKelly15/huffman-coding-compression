#ifndef TREE_H
#define TREE_H

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>

#include "bit_buf.h"

typedef struct _Huffman_Node {
    char c;
    int freq;
    struct _Huffman_Node *left, *right;
} huffman_tree_node;

/******************************************
 *                                        *
 *   Enter your code for huffman coding   *
 *                                        *
 ******************************************/


extern huffman_tree_node *huffman_tree;
extern char huffman_code[256][20];

huffman_tree_node *merge_nodes(HeapNode *one, HeapNode *two);
void create_huffman_tree_from_frequencies();
void make_huffman_table();
void make_header_with_tree(write_bit_buffer *compressed_buffer);
void decompress_header(read_bit_buffer *reader);
void decompress_remaining_file(read_bit_buffer *reader, FILE *out_file);

#endif
