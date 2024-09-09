//
// Created by guoweiu on 9/9/24.
//
//
// Created by tank on 4/19/24.
//

#include <stdio.h>
#include <numa.h> // Include the necessary header file for NUMA support

int main() {
	// Check if NUMA is available on the system
	int ret = numa_available();
	if (ret < 0) {
		perror("NUMA not available");
		return -1;
	}

	// Get the number of NUMA nodes
	int num_nodes = numa_num_configured_nodes();
	printf("Number of NUMA nodes: %d\n", num_nodes);

	// Iterate through all NUMA nodes
	for (int i = 0; i < num_nodes; i++) {
		printf("NUMA Node %d:\n", i);
		printf("  CPUs: ");

		// Allocate a CPU mask to store the CPUs associated with the node
		struct bitmask *cpumask = numa_allocate_cpumask();

		// Get the CPUs attached to the node
		numa_node_to_cpus(i, cpumask);

		for (int j = 0; j < numa_num_configured_cpus(); j++) {
			if (numa_bitmask_isbitset(cpumask, j))
				printf("%d ", j);
		}
		printf("\n");

		// Get the memory size of the node
		long long unsigned node_memory = numa_node_size64(i, NULL)/(1024 * 1024);


		// Print the CPU mask
		//        numa_bitmask_printf(stdout, cpumask);
		printf("  Memory Size: %llu MB\n", node_memory);

		// Free the allocated CPU mask
		numa_free_cpumask(cpumask);
	}

	return 0;
}