#include <thread>
#include <iostream>

void func(int i) {
    for (int j = 0; j < i; j++) {
        std::cout << "func " << j << std::endl;
    }
}

std::thread f() {
    // return temporary object thread
    return std::thread(func, 1000);
}
void func2(int i) {
        for (int j = 0; j < i; j++) {
            std::cout << "g " << j << std::endl;
        }
    }
std::thread g() {
    // return named thread
    std::thread t(func2, 100);
    return t;
}

void thread_input_func(std::thread t)  {
    t.join();
}
int main() {
    // thread objects can only be owned in one place
    std::thread t = f();
    // move is required
    thread_input_func(std::move(t));
}