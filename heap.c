#include "heap.h"


HeapNode heap[256];
int heapSize;

/*Initialize Heap.  The first heap element starts at 1.  Position 0 is not used */
void heapInit() {
    heapSize = 0;
    heap[0].c = 0;
    heap[0].t = NULL;
    heap[0].freq = 0;
}

// BubbleUp an element at positon pos
void bubbleUp(int pos)  {
    HeapNode element;

    element.c = heap[pos].c;
    element.t = heap[pos].t;
    element.freq = heap[pos].freq;

    while (heap[pos / 2].freq > element.freq) {
        heap[pos].c = heap[pos/2].c;
        heap[pos].t = heap[pos/2].t;
        heap[pos].freq = heap[pos/2].freq;

        pos /= 2;
    }
    heap[pos].c = element.c;
    heap[pos].t = element.t;
    heap[pos].freq = element.freq;

}

// BubbleDown the element at pos, assuming head has already been deleted.
void bubbleDown(int pos)  {
    int child;
    HeapNode element;
    element.c = heap[pos].c;
    element.t = heap[pos].t;
    element.freq = heap[pos].freq;

    while(1)   {
        // first child
        child = pos * 2;
	if(child > heapSize)
	       break;

        if (child != heapSize && heap[child + 1].freq < heap[child].freq) {
	     // second child
             child++;
        }
        /* To check if the last element fits ot not it suffices to check if the last element
         is less than the minimum element among both the children*/

        if (element.freq > heap[child].freq) {
		heap[pos].c = heap[child].c;
		heap[pos].t = heap[child].t;
		heap[pos].freq = heap[child].freq;
        } else /* It fits there */
        {
            break;
        }
	pos = child;
    }
    heap[pos].c = element.c;
    heap[pos].t = element.t;
    heap[pos].freq = element.freq;
}


/*Insert an element into the heap */
void HeapInsert(char c, huffman_tree_node *t, int freq) {
    heapSize++;
    if(heapSize >= 256)   {
	  fprintf(stderr, "Exceeded heap capacity\n");
	  assert(0);
    }


    /*Insert in the last place*/
    heap[heapSize].t = t;
    heap[heapSize].c = c;
    heap[heapSize].freq = freq;

    bubbleUp(heapSize);
}

HeapNode DeleteMin() {
    HeapNode minElement;
    /* heap[1] is the minimum element. So we remove heap[1]. Size of the heap is decreased.
     Now heap[1] has to be filled. We put the last element in its place and see if it fits.
     If it does not fit, take minimum element among both its children and replaces parent with it.
     Again See if the last element fits in that place.*/

    minElement.c = heap[1].c;
    minElement.t = heap[1].t;
    minElement.freq = heap[1].freq;

    heap[1].c = heap[heapSize].c;
    heap[1].t = heap[heapSize].t;
    heap[1].freq = heap[heapSize].freq;
    /* now refers to the index at which we are now */
    bubbleDown(1);

    heapSize--;

    return minElement;
}

void heapPrint()  {
    // int  i;
    // printf("Heap\n---------------\n");
    // for (i = 1; i <= heapSize; i++) {
    //     printf("%d ", heap[i].c);
    // 	if(isprint(heap[i].c)) {
    //         printf("(%c) ", heap[i].c);
    //         printf("%d\n", heap[i].freq);
    //     } else {
    //         printf("(-) ");
    //         printf("%d\n", heap[i].freq);
    //     }
    // }
    // printf("----------------\n");
}


/*
int main() {
    HeapNode h;

    heapInit();

        HeapInsert('a', NULL, 44);
        HeapInsert('b', NULL, 32);
        HeapInsert('d', NULL, 99);
        HeapInsert('f', NULL, 43);
        HeapInsert('u', NULL, 51);
        HeapInsert('y', NULL, 1);

    heapPrint();

    h = DeleteMin();
    heapPrint();

    h = DeleteMin();
    heapPrint();
    return 0;
}
*/
