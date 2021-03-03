# CSCI 2021: Project 1, Huffman Coding

## Table of Contents
* [General Information](#general-information)
* [Setup](#setup)
* [Running](#running)
* [Limitations](#limitations)

## General Information
The files make up a functional compression/decompression algorithm using Huffman Coding. This is implemented by reading the input file, counting the frequencies and appending them to a heap, building an optimized Huffman tree using the heap, building an encoding table according to the tree, and finally rereading the file, using the table to encode every character. The Huffman tree is stored via a preorder traversal, a 1 representing an outer node and a 0 representing an inner node. To decompress a file, the algorithm reads in the header of the file to generate the Huffman tree, then reads in bits, traversing the Huffman tree until an outer node is reached, at which point the character at the outer node is added to the new file. This is repeated until the entire file is read.

## Setup
Download the files into a directory. A Makefile is included. Use the following to generate an executable and remove the excess files including the executable. Run the commands on the command line or terminal within the directory the files are located.
```
make
make clean
```

## Running
To compress a file, navigate to the folder where the executable lies in the terminal. Use the following to compress or decompress a file.
```
huffman -c start_file compressed_file
huffman -d compressed_file decompressed_file
```

## Limitations
The compression algorithm currently only works with files that have a size up to 1,000,000 bytes (1 MB). It works with any file type, although there are no guarantees the end file will actually be smaller than the starting file (expansion may occur for very small file sizes or incredibly evenly distributed files).
