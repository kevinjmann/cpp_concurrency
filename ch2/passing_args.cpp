#include <string>
#include <iostream>
#include <thread>

void func(int i, std::string const& text) {
    for (int j = 0; j < i; j++) {
        std::cout << j << " : " << text << std::endl;
    }
}

void oops() {
    char buffer[1024];
    sprintf(buffer, "test text");
    std::thread t(func, 1000, buffer);
    t.detach();
    // when oops finishes, the buffer gets destroyed
    // the function on the thread ends up printing garbage data afterwards
}

void not_oops() {
    char buffer[1024];
    sprintf(buffer, "test text");
    std::thread t(func, 1000, std::string(buffer));
    t.detach();
    // when not_oops finishes, the thread has its own string variable
    // so it doesn't print garbage.
}
int main() {
    // oops();
    not_oops();
    for(int i = 0; i < 1000; i++) {
        std::cout << "main " << i << std::endl;
    }
}