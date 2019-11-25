#include "ComplexSystemsApp.h"

int main() {
	
	// allocation
	auto app = new ComplexSystemsApp();

	// initialise and loop
	app->run("AIE", 1280, 720, false);

	// deallocation
	delete app;

	return 0;
}

/*
// Multi Threading
#include <thread>
#include <mutex>

#include <vector>
#include <iostream>

void count(int start, int end) {
    for (int i = start; i < end; i++) {
        std::cout << i << " ";
    }
}

int main() {
    // Starts a seperate thread of execution
    std::vector<std::thread> threads;
    int max = 100;
    int numThreads = 10;

    for (int i = 1; i <= max; i+=numThreads) {
        threads.push_back(std::thread(count, i, i + numThreads));
    }

    // Waits until the thread has ended
    for (std::thread& thread : threads) {
        thread.join();
    }

    return 0;
}
*/