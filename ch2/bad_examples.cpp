#include <thread>
#include <iostream>

void printHello() {
    std::cout << "Hello world\n";
}

int main() {

    std::thread t(printHello);
    for(int i = 0; i< 1000; i++) {
        std::cout << i << std::endl;
    }
    // no joining or detaching running thread calls terminate
    // add t.join();
} 