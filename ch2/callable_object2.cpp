#include <iostream>
#include <thread>

struct CallableObject {
    int& i;
    CallableObject(int& i_): i(i_) {}
    void operator()() {
        std::cout << "called from object with val: " << i << std::endl;
    }
};

int main() {
    int i = 20;
    int j = 30;
    /**
     * care has to be taken when calling with temporary variables
     * expression must have class type but it has type "std::thread (*)(CallableObject (*)())"
     */
    // std::thread t(CallableObject(i));
    // t.join();
    // either enclose in parentheses or use {}
    // note: the functions writing in cout aren't guaranteed to do so sequentially.
    std::thread t((CallableObject(i)));
    std::thread t2{CallableObject(j)};
    t.join();
    t2.join();
}