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
void set_huffman_tree_top_node(huffman_tree_node *node);
void make_huffman_table();
// static void add_huffman_bits_at_node(huffman_tree_node *top_node, write_bit_buffer *compressed_buffer);
void make_header_with_tree(write_bit_buffer *compressed_buffer);
void decompress_header(read_bit_buffer *reader);
void decompress_remaining_file(read_bit_buffer *reader, FILE *out_file);

#endif
