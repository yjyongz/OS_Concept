#include <stdio.h>
#include <math.h>
#include <unistd.h>
#include <string.h> // memset
#include <stdint.h> // uint64_t
#include <stdlib.h> //calloc

#define MIN_PID 300
#define MAX_PID 5000

/*
 * it's a segment-tree based approach.
 * allocate_pid logN where N is number of "segment" nodes
 * release_pid logN where N ...
 * 1. construct full binary tree where bottom leaves have one-to-one mapping to MIN_PID - MAX_PID, and there might be some leftovers.
 * 2. node value zero means there is a free node under its subtree
 */
int allocate_map();
int allocate_pid();
void release_pid(pid_t pid);
int base = 0;
int end = 0;
int height = 0;
uint64_t *bitarray = NULL;
int bitsize = 0;

int get_value(int index) {
	int q,r;
	q = (int)(index / (8 * sizeof(uint64_t)));
	r = (int)(index % (8 * sizeof(uint64_t)));
	return (int)((bitarray[q] & (1UL << r)) > 0);
}

void set_value(int index) {
	int q,r;
	q = (int)(index / (8 * sizeof(uint64_t)));
	r = (int)(index % (8 * sizeof(uint64_t)));
	bitarray[q] |= (1UL << r);
}

void clear_value(int index) {
	int q,r;
	q = (int)(index / (8 * sizeof(uint64_t)));
	r = (int)(index % (8 * sizeof(uint64_t)));
	bitarray[q] &= ~(1UL << r);
}

int allocate_map() {
	int total_nodes = MAX_PID - MIN_PID;
	int h = (int)log2(total_nodes);
	int bottom_leaves_nodes = pow(2, h+1);
	int total_bits = pow(2, h+1+1) - 1;
	int size = (int)((total_bits+1)/64);
	bitarray = (uint64_t*)calloc(size, sizeof(uint64_t));
	bitsize = size;
	base = bottom_leaves_nodes;
	end  = (base << 1) - 1;
	height = h+1;
	printf("bottom leaves: %d, total_bits: %d size of bitarray: %d\n", bottom_leaves_nodes, total_bits, size);
	printf("base: %d end: %d\n", base, end);
	return 1;
}

int allocate_pid() {
	int root = 1;
	if (get_value(root) != 0) {
		return -2;
	}
	do {
		int left = root << 1;
		int right = root << 1 | 1;
		if (get_value(left) == 0) {
			root = left;
		} else if (get_value(right) == 0) {
			root = right;
		} else {
			return  -1;
		}
	} while (!(base <= root && root <= end));

	set_value(root);

	int index = root;
	while (index != 0) {
		int parent = index >> 1;
		if (parent == 0) break;
		int leftv, rightv;
		rightv = get_value(parent << 1 | 1);
		leftv = get_value(parent << 1);
		//printf("r: %d l: %d\n", rightv, leftv);
		if (rightv == 1 && leftv == 1) {
				set_value(parent);	
		}
		index = parent;
	}
	return root - base;
}

void release_pid(pid_t pid) {
	pid -= MIN_PID;
	int index = base + pid;
	clear_value(index);
	while (index != 0) {
		int parent = index >> 1;
		int leftv, rightv;
		if ((parent << 1) == index) {
			// from left child
			rightv = get_value(parent << 1 | 1);
			if (rightv == 0) {
				clear_value(parent);	
			}
		} else if ((parent << 1 | 1) == index) {
			// from right child
			leftv = get_value(parent << 1);
			if (leftv == 0) {
				clear_value(parent);	
			}
		}
		index = parent;
	}
	return;
}

int main(int argc, char *argv[]) {
	allocate_map();
	for (int idx = MIN_PID; idx <= MAX_PID;idx++) {
		printf("%d ", allocate_pid()+MIN_PID);
	}
	printf("\n");
	char output[50];
	memset(output, '#', sizeof(output));
	output[17] = 0;
	printf("%*s releaseing ids %*s\n",16, output, 16, output);
	for (int idx = MIN_PID; idx <= MAX_PID;idx++) {
		release_pid(idx);
	}
	printf("\n");
	for (int idx = MIN_PID; idx <= MAX_PID;idx++) {
		printf("%d ", allocate_pid()+MIN_PID);
	}
	printf("\n");
	return 0;
}
