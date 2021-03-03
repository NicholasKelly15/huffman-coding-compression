# CSCI 2021: Project 1, Huffman Coding

## Table of Contents
* [General Information](#general-information)
* [Setup](#setup)
* [Running](#running)
* [Status](#status)

## General Information
The files make up a functional compression/decompression algorithm using Huffman Coding. This is implemented by reading the input file, counting the frequencies and appending them to a heap, building an optimized Huffman tree using the heap, building an encoding table according to the tree, and finally rereading the file, using the table to encode every character. The Huffman tree is stored via a preorder traversal, a 1 representing an outer node and a 0 representing an inner node. To decompress a file, the algorithm reads in the header of the file to generate the Huffman tree, then reads in bits, traversing the Huffman tree until an outer node is reached, at which point the character at the outer node is added to the new file. This is repeated until the entire file is read.

## Setup
```
run
```

## Running


## Status
