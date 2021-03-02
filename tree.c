#include <stdlib.h>

#include "bit_buf.h"
#include "heap.h"
#include "tree.h"

huffman_tree_node *huffman_tree;
char huffman_code[256][20];

/******************************************
 *                                        *
 *   Enter your code for huffman coding   *
 *                                        *
 ******************************************/

 huffman_tree_node *merge_nodes(HeapNode *one, HeapNode *two) {
     // Add a huffman node to the heap nodes if they don't have one already.
     if (one->t == NULL) {
         huffman_tree_node *one_huff_node = (huffman_tree_node *) malloc(sizeof(huffman_tree_node));
         one_huff_node->freq = one->freq;
         one_huff_node->c = one->c;
         one_huff_node->left = NULL;
         one_huff_node->right = NULL;
         one->t = one_huff_node;
     }
     if (two->t == NULL) {
         huffman_tree_node *two_huff_node = (huffman_tree_node *) malloc(sizeof(huffman_tree_node));
         two_huff_node->freq = two->freq;
         two_huff_node->c = two->c;
         two_huff_node->left = NULL;
         two_huff_node->right = NULL;
         two->t = two_huff_node;
     }

     // Make the huffman tree node and add data
     huffman_tree_node *new_tree_node = (huffman_tree_node *) malloc(sizeof(huffman_tree_node));
     new_tree_node->freq = one->freq + two->freq;
     new_tree_node->left = one->t;
     new_tree_node->right = two->t;
     new_tree_node->c = 9;

     return new_tree_node;
 }

 void set_huffman_tree_top_node(huffman_tree_node *node) {
     huffman_tree = node;
 }

 void make_huffman_table_at_node(huffman_tree_node *top_node, char current_encoding[], int current_size) {
     if (top_node->left == NULL) {
         for (int i = 0 ; i < 20 ; i++) {
             huffman_code[(unsigned char) top_node->c][i] = current_encoding[i];
         }
     } else {
         char next_encoding_left[20];
         for (int i = 0 ; i < 20 ; i++) {
             next_encoding_left[i] = current_encoding[i];
         }
         next_encoding_left[current_size] = 0;

         char next_encoding_right[20];
         for (int i = 0 ; i < 20 ; i++) {
             next_encoding_right[i] = current_encoding[i];
         }
         next_encoding_right[current_size] = 1;

         make_huffman_table_at_node(top_node->left, next_encoding_left, current_size + 1);
         make_huffman_table_at_node(top_node->right, next_encoding_right, current_size + 1);
     }
 }

 void make_huffman_table() {

     for (int i = 0 ; i < 256 ; i++) {
         for (int j = 0 ; j < 20 ; j++) {
             huffman_code[i][j] = -1;
         }
     }

     huffman_tree_node *traversal_node;
     traversal_node = huffman_tree;

     char start_encoding[20];
     for (int i = 0 ; i < 20 ; i++) {
         start_encoding[i] = -1;
     }

     make_huffman_table_at_node(traversal_node, start_encoding, 0);
 }

 /*
 Recursive preorder traversal algorithm,
 writes a 0 to say that the node is an inner node,
 writes a 1 to say that the node is an outer node,
 when this is written, the following 8 bits are the ascii representation
 of the character at that outer node.
 */
 static void add_huffman_bits_at_node(huffman_tree_node *top_node, write_bit_buffer *compressed_buffer) {
     if (top_node->left == NULL) {
         // The node is an outer node, write 1 and the character.
         bitbuf_write_bit(compressed_buffer, 1);
         int bit_mask = 1;
         for (int i = 0 ; i < 8 ; i++) {
             bitbuf_write_bit(compressed_buffer, (top_node->c & bit_mask) == (1 << i));
             bit_mask = bit_mask << 1;
         }
     } else {
         // The node is an inner node, print 0 and run recursion.
         bitbuf_write_bit(compressed_buffer, 0);
         add_huffman_bits_at_node(top_node->left, compressed_buffer);
         add_huffman_bits_at_node(top_node->right, compressed_buffer);
     }
 }

 void make_header_with_tree(write_bit_buffer *compressed_buffer) {
     add_huffman_bits_at_node(huffman_tree, compressed_buffer);
 }

 static huffman_tree_node *decompress_header_at_node(read_bit_buffer *reader) {
     int type = bitbuf_read_bit(reader);
     if (type == 0) {
         // Make inner node and use recursion to finish the tree at the node
         huffman_tree_node *new_tree_node = (huffman_tree_node *) malloc(sizeof(huffman_tree_node));
         new_tree_node->left = decompress_header_at_node(reader);
         new_tree_node->right = decompress_header_at_node(reader);
         new_tree_node->c = '~';
         return new_tree_node;
     } else if (type == 1) {
         // Make outer node and return a pointer to the node
         huffman_tree_node *new_tree_node = (huffman_tree_node *) malloc(sizeof(huffman_tree_node));
         unsigned char node_char = 0;
         for (int i = 0 ; i < 8 ; i++) {
             node_char = node_char >> 1;
             int bit = bitbuf_read_bit(reader);
             node_char += (128 * bit);
         }
         new_tree_node->c = node_char;
         new_tree_node->left = NULL;
         new_tree_node->right = NULL;
         return new_tree_node;
     } else {
         printf("Huh?");
         return NULL;
     }
 }

 void decompress_header(read_bit_buffer *reader) {
     huffman_tree = decompress_header_at_node(reader);
 }

 static char decompress_next_character(read_bit_buffer *reader) {
     huffman_tree_node *iterator = huffman_tree;
     while (1) {
         int next_bit = bitbuf_read_bit(reader);
         if (next_bit == 0) {
             iterator = iterator->left;
         } else {
             iterator = iterator->right;
         }

         if (iterator->left == NULL && iterator->right == NULL) {
             return iterator->c;
         }
     }
 }

 void decompress_remaining_file(read_bit_buffer *reader, FILE *out_file) {
     char next_char;
     while ((next_char = decompress_next_character(reader)) != 0) {
         fputc(next_char, out_file);
     }
 }
