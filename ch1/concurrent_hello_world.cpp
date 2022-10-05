#include <thread>
#include <iostream>

void hello() {
	std::cout << "Hello world from a thread\n";
}

int main() {
	std::thread t(hello);
	t.join();
}
