#include <stdlib.h>

#include "bit_buf.h"
#include "heap.h"
#include "tree.h"

huffman_tree_node *huffman_tree;
char huffman_code[256][20];

/****************************************************************************************
// Function used to help build the huffman tree. Takes 2 Heap_Node's as arguments,
// finds the huffman nodes that each heap node points to (creates one if the heap
// node does not have a huffman node), and makes a new huffman_tree_node that
// points to the 2 nodes that were merged. This new huffman_tree_node is returned
// as a pointer. By popping min from the heap, merging the nodes, and pushing
// them back to the heap, a huffman tree can be created.

// Args:
// HeapNode *one: First HeapNode popped from the heap
// HeapNode *two: Second HeapNode popped from the heap

// Return: huffman_tree_node *: New tree pointer created from smaller trees
*****************************************************************************************/
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
     new_tree_node->c = '~';

     return new_tree_node;
 }

 /****************************************************************************************
 // Creates the huffman tree using the heap of frequencies. Does so by popping
 // the two smallest elements from the heap, calling the merge function, and then
 // adding the returned node back into the heap. The huffman tree main node
 // is set with the remaining tree node in the single heap node left after
 // the process is repeated enough times.

 // Args: None, uses that heap it already has access to

 // Return: void, sets the huffman_tree external node
 *****************************************************************************************/
 void create_huffman_tree_from_frequencies() {
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
	huffman_tree = DeleteMin().t;
 }

 /****************************************************************************************
 // Recursive function that is used to create the huffman table. Takes a node in
 // huffman tree, checks if it is a outer node. If it is, the outer node has
 // a char to add to the table, if not, then the function calls itself on the
 // 2 child nodes of the given node.

 // Args:
 // huffman_tree_node *top_node: Node with the desired char or node to branch
 // off of.
 //
 // char current_encoding[]: Holds an array of 1's and 0's for the encoding that
 // the char at the current node would have assuming that the char is on an
 // outer node and is relevant. Changed at each recursive call.
 //
 // int current_size: Used to reference how long the current encoding is.

 // Return: void
 *****************************************************************************************/
 void make_huffman_table_at_node(huffman_tree_node *top_node, char current_encoding[], int current_size) {
     if (top_node->left == NULL) { // if the node is an outer node
         for (int i = 0 ; i < 20 ; i++) {
             huffman_code[(unsigned char) top_node->c][i] = current_encoding[i];
         }
     } else { // inner node
         char next_encoding_left[20];
         for (int i = 0 ; i < 20 ; i++) {
             next_encoding_left[i] = current_encoding[i];
         }
         // encoding for the left branch of the node
         next_encoding_left[current_size] = 0;

         char next_encoding_right[20];
         for (int i = 0 ; i < 20 ; i++) {
             next_encoding_right[i] = current_encoding[i];
         }
         // encoding for the right branch of the node
         next_encoding_right[current_size] = 1;

         // recursive call on the children of the node with updated encodings
         make_huffman_table_at_node(top_node->left, next_encoding_left, current_size + 1);
         make_huffman_table_at_node(top_node->right, next_encoding_right, current_size + 1);
     }
 }

 /****************************************************************************************
 // Function to create the huffman table mean to be used. This calls the previous
 // recursive version of the function, acting like a starter function for the
 // recursive loop.
 *****************************************************************************************/
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

 /*****************************************************************************
 Recursive preorder traversal algorithm,
 writes a 0 to say that the node is an inner node,
 writes a 1 to say that the node is an outer node,
 when this is written, the following 8 bits are the ascii representation
 of the character at that outer node.
 *******************************************************************************/
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

 /*****************************************************************************
 Calls the previous recursive version of the function, meant to be used by
 external modules, starts the recursion.
 *******************************************************************************/
 void make_header_with_tree(write_bit_buffer *compressed_buffer) {
     add_huffman_bits_at_node(huffman_tree, compressed_buffer);
 }

 /*****************************************************************************
 Builds the huffman tree with the header of the input file. This is the recursive
 function which acts as follows: create an inner node if bit read is 0, create
 outer node if the bit is 1, make the character at the outer node the character
 that is encoded in the next 8 bits. Repeat until the tree is full. Returns
 the huffman node at the top of the tree that is created.
 *******************************************************************************/
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

 /*****************************************************************************
 Starts the recursion and sets the huffman tree according to the recusive
 result of the previous function.
 *******************************************************************************/
 void decompress_header(read_bit_buffer *reader) {
     huffman_tree = decompress_header_at_node(reader);
 }

 /*****************************************************************************
 Takes the bit buffer reader, reads a bit at a time and traverses the
 huffman tree according to the bit. Once an outer node is reached on the
 huffman tree, the character at the node is returned.
 *******************************************************************************/
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

 /*****************************************************************************
 Decompresses the content (everything but the header/huffman tree) of the
 file stored in the bit buffer reader. Decodes characters until the EOF character
 is reached. Each character is outputted into the output file.
 *******************************************************************************/
 void decompress_remaining_file(read_bit_buffer *reader, FILE *out_file) {
     char next_char;
     while ((next_char = decompress_next_character(reader)) != 0) {
         fputc(next_char, out_file);
     }
 }
