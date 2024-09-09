//
// Created by lfz on 9/5/24.
//
#include <cstdlib>
#include <iostream>

int main() {
	// Replace "your-image-name" with the name of your Docker image
	const char* image_name = "your-image-name";
	const char* container_name = "your-container-name";
	const char* port_mapping = "8080:80"; // Map host port 8080 to container port 80

	// Build the Docker image
	system(("docker build -t " + std::string(image_name) + " .").c_str());

	// Run the Docker container
	system(("docker run -d --name " + std::string(container_name) + " -p " + std::string(port_mapping) + " " + std::string(image_name)).c_str());

	// Wait for the container to start
	std::cout << "Container is starting..." << std::endl;
	std::this_thread::sleep_for(std::chrono::seconds(5));

	// Send a request to the container
	// This example assumes the container runs a web server and you're using `curl` to send a request
	system(("curl http://localhost:" + std::to_string(8080)).c_str());

	// Clean up: Remove the container
	// Uncomment the following lines to remove the container after it's done running
	// system(("docker rm -f " + std::string(container_name)).c_str());

	return 0;
}