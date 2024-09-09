//
// Created by guoweiu on 9/9/24.
//
//
// Created by tank on 4/19/24.
//
#include <stdio.h>
#include <stdlib.h>
#include <numa.h>
#include <unistd.h>

int main() {

	void *ptr_arr[1030];

	// Calculate the size of memory to allocate (1 MB)
	size_t alloc_size = 1ULL << 20;

	for (int i = 0; i < 1024; i++) {
		// Try to allocate memory
		ptr_arr[i] = malloc(alloc_size);
		if (ptr_arr[i] == NULL) {
			perror("malloc");
			exit(1);
		}

		// Note that:
		// When you allocate memory using 'malloc', the OS reserves virtual memory for your process.
		// However, the physical memory is allocated only when you process actually accesses the memory.

		// Access the allocated memory (write some data)
		memset(ptr_arr[i], i, alloc_size);

		sleep(1);
	}

	for (int i = 0; i < 1024; i++) {
		free(ptr_arr[i]);
	}

	return 0;
}