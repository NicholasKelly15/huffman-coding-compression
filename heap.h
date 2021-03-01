#ifndef HEAP_H
#define HEAP_H

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <limits.h>


typedef struct _Heap_Node {
	char c;
	int freq;
	struct _Huffman_Node *t;
} HeapNode;

/********************************************************
 *
 * The heap starts at heap[1].  Position 0 is not used
 *
 *******************************************************/
extern HeapNode heap[256];
extern int heapSize;

void heapInit();

void bubbleUp(int pos);

void bubbleDown(int pos);

#ifndef TREE_H
#include "tree.h"
void HeapInsert(char c, huffman_tree_node *t, int freq);
#endif

HeapNode DeleteMin();

void heapPrint();

#endif
