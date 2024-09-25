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
//	int value = 10;  // 定义一个整型变量
//	void *addr = malloc(4096);  // 让 addr 指向 value 的地址
//	unsigned long nodemask;
//	int ret;
//
//	// Check if NUMA is available
//	if (numa_available() < 0) {
//		perror("NUMA not available");
//		return -1;
//	}
//
//	// Choose the target NUMA node (e.g., node 1)
//	int target_node = 1;
//
//	// Calculate the size of memory to allocate (1 MB)
//	size_t alloc_size = 1ULL << 20;
//
//	void *ptr_arr[1030];
	size_t page_size = sysconf(_SC_PAGESIZE);
	void *addr;

	// Allocate memory with page alignment
	if (posix_memalign(&addr, page_size, page_size) != 0) {
		perror("posix_memalign");
		return -1;
	}


	if (numa_available() < 0) {
		perror("NUMA not available");
		free(addr);
		return -1;
	}

	// Only one NUMA node, so nodemask is a single byte
	size_t nodemask_size = 1; // Only one node
	unsigned char nodemask = 1UL; // Node 0

	int ret = mbind(addr, page_size, MPOL_BIND, &nodemask, nodemask_size * 8, MPOL_MF_STRICT);
	if (ret != 0) {
		perror("mbind");
		free(addr);
		return 1;
	}

	// Cleanup
	free(addr);

	return 0;
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