#include <thread>
#include <iostream>
#include <stdexcept>

struct func{
    int& i;
    func(int& i_): i(i_){}
    void operator()() {
        for (int j =0; j < 1000; j++) {
            std::cout << j << ":" << i << std::endl;
        }
    }
};
void exceptional_function(){
    throw std::invalid_argument("exception thrown");
}

int main() {
    int i = 10;
    func f(i);
    std::thread t(f);
    // when exception thrown, thread eventually stops, but not necessarily immediately.
    // exceptional_function();
    try{
        exceptional_function();
    }catch(...){
        // guaranteed to call join even when exception called
        t.join();
    }
    t.join();
}