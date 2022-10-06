#include <thread>
#include <iostream>

class thread_guard {
    std::thread& t;
    public:
    explicit thread_guard(std::thread& t_): t(t_) {}
    ~thread_guard() {
        if(t.joinable()) {
            t.join();
        }
    }
    // delete copy constructor
    thread_guard(thread_guard const&) = delete;
    // delete equals operator
    thread_guard& operator=(thread_guard const&) = delete;

};

struct func {
    int& i;
    func(int& i_): i(i_){
        
    }

    void operator()() {
        for(int j = 0; j <1000; j++) {
            std::cout << j << " : " << i <<std::endl; 
        }
    }
};

int main() {
    int i = 100;
    func f(i);
    std::thread t(f);
    thread_guard tg(t);
    // thread guard guaranteed to join thread on delete.
    // objects are destroyed in reverse order of construction, so i
    // gets deleted after the thread making this thread safe.
}