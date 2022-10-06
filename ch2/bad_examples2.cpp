#include <thread>
#include <iostream>

struct func {
    int& i;
    func(int& i_): i(i_){}
    void operator()() {
        for(int j = 0; j < 1000; j++) {
            std::cout << i << std::endl;
        }
    }
};

int main() {
    int i = 100;
    func f(i);

    std::thread t(f);
    // give main function something to do to give time for thread to start running before hitting the end of the function
    for(int j = 0; j < 10; j++) {
        std::cout << "main " << j << std::endl;
    }
    t.detach();
    // as value i goes out of scope, the shared data is destroyed
    // and so the thread starts printing garbage
}