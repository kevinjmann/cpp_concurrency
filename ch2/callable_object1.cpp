#include <thread>
#include <iostream>

struct CallableObject {
    int& i;
    CallableObject(int& i_): i(i_){}
    void operator()() {
        std::cout << "called object with val: " << i <<std::endl;
    }
};

int main() {
    int i = 10;
    CallableObject callableObj(i);
    std::thread t(callableObj);
    t.join();
    // callableObj uses a shared data value. 
    // value goes out of scope, it gets deleted and we get undefined behavior
    // we have to join in this thread
} 