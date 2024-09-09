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
#include <numaif.h>

int main() {
	int value = 10;  // 定义一个整型变量
	void *addr = malloc(4096);  // 让 addr 指向 value 的地址
	unsigned long nodemask;
	int ret;

	// Check if NUMA is available
	if (numa_available() < 0) {
		perror("NUMA not available");
		return -1;
	}

	// Choose the target NUMA node (e.g., node 1)
	int target_node = 1;

	// Calculate the size of memory to allocate (1 MB)
	size_t alloc_size = 1ULL << 20;

	void *ptr_arr[1030];

	nodemask = (1UL << 0);
	ret = mbind(addr, 4096, MPOL_BIND, &nodemask, 1, 0);
	printf("%d\n", ret);
//	if (ret != 0) {
////		perror("mbind");
//		return 1;
//	}
/*
	for (int i = 0; i < 1024; i++) {
		// Try to allocate memory on the specified node
		ptr_arr[i] = numa_alloc_onnode(alloc_size, target_node);

		// Note that:
		// When you allocate memory using 'numa_alloc_onnode', the OS reserves virtual memory for your process.
		// However, the physical memory is allocated only when you process actually accesses the memory.

		// Access the allocated memory (write some data)
		memset(ptr_arr[i], i, alloc_size);

		sleep(1);
	}

	for (int i = 0; i < 1024; i++) {
		numa_free(ptr_arr[i], alloc_size);
	}*/
	return 0;
}