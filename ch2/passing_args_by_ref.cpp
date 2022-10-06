#include <thread>
#include <iostream>

class X {
    int i;
    public:
    X(int i_): i(i_){} 
    void doSomething() {
        for(int i =0; i < 1000; i++) {
            std::cout << i << std::endl;
        }
    }
};

int main() {
    X x(10);
    std::thread t(&X::doSomething, &x);
    t.join();
}
