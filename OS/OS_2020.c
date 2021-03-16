#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

#define SIZE 200000

struct PAGE {
	int memory_location;
	int valid_bit;
};
struct PAGE page[SIZE];

struct MEMORY {
	int page_num;
	int reference_bit;
	int dirty_bit;
};

int main(int argc, char* argv[]) {
	int i, pg, op;
	int pageFault = 0, diskWrite = 0, headMovement = 0;
	int headPos = 0, clockPos = 0;
	int memoryCount = 0;
	int memory_size = atoi(argv[1]);
	FILE* input_file = fopen("C:\\OS_data\\a.txt", "r");
	struct MEMORY* memory = (struct MEMORY*)malloc(memory_size * sizeof(struct MEMORY));

	for (i = 0; i < memory_size; i++) {
		memory[i].reference_bit = 0;
		memory[i].dirty_bit = 0;
	}
	for (i = 0; i < SIZE; i++)
		page[i].valid_bit = 0;

	while (!feof(input_file)) {
		fscanf(input_file, "%d %d", &pg, &op);
		if (page[pg].valid_bit == 1) {
			memory[page[pg].memory_location].reference_bit = 1;
			if (op == 1)
				memory[page[pg].memory_location].dirty_bit = 1;
		}
		else {
			pageFault++;
			if (memoryCount==memory_size) {
				while (1) {
					if (clockPos == memory_size)
						clockPos = 0;
					if (memory[clockPos].reference_bit == 1) {
						memory[clockPos].reference_bit = 0;
						clockPos++;
					}
					else if (memory[clockPos].reference_bit == 0) {
						page[memory[clockPos].page_num].valid_bit = 0;
						memoryCount--;
						if (memory[clockPos].dirty_bit == 1) {
							diskWrite++;
							headMovement = headMovement + abs(headPos - (memory[clockPos].page_num / 1000));
							headPos = memory[clockPos].page_num / 1000;
						}
						break;
					}
				}
			}
			headMovement = headMovement + abs(headPos - (pg / 1000));
			headPos = pg / 1000;
			memory[clockPos].page_num = pg;
			if (op == 1)
				memory[clockPos].dirty_bit = 1;
			else
				memory[clockPos].dirty_bit = 0;
			memory[clockPos].reference_bit = 0;
			page[pg].memory_location = clockPos;
			page[pg].valid_bit = 1;
			memoryCount++;
			clockPos++;
		}
	}
	printf("%d\n", pageFault);
	printf("%d\n", diskWrite);
	printf("%d\n", headMovement);
}