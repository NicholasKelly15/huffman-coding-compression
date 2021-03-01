CC = gcc -Wall -g

all: huffman

huffman: huffman.o bit_buf.o heap.o tree.o
	$(CC) -o huffman huffman.o bit_buf.o heap.o tree.o

heap.o: heap.h heap.c

bit_buf.o: bit_buf.h bit_buf.c

tree.o: tree.h tree.c

huffman.o: huffman.c

clean:
	rm -rf *.o huffman
	rm -rf huffman.exe.stackdump
	rm -rf out.txt
	rm -rf b.txt
