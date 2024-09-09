//
// Created by guoweiu on 9/9/24.
//
#include <stdio.h>
#include <numa.h>

int main() {
	if (numa_available()) {
		printf("NUMA is not available.\n");
		return 1;
	}

	// Get the number of NUMA nodes
	int num_nodes = numa_num_configured_nodes();

	// Iterate over NUMA nodes
	for (int node = 0; node < num_nodes; ++node) {
		// Get the size of the NUMA node's memory in bytes
		// Return node size and free memory
		long long total, free;
		total = numa_node_size64(node, &free);
		if (free < 0) {
			perror("numa_node_size64");
			return 1;
		}

		printf("node %d: size %lld Bytes, free %lld Bytes\n", node, total, free);
	}

	return 0;
}