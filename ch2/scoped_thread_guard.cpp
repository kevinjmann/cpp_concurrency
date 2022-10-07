#include <thread>
#include <iostream>
#include <chrono>
class scoped_thread {
    std::thread t;
    public:
    // with ownership, we don't want to pass things by reference?
    explicit scoped_thread(std::thread t_):t(std::move(t_)){
        if (!t.joinable()) {
            throw std::logic_error("No thread");
        }
    }
    ~scoped_thread(){
        if (t.joinable()) {
            t.join();
        }
    }
    scoped_thread(scoped_thread const&) = delete;
    scoped_thread operator=(scoped_thread const&) = delete;
};

struct func {
    int& i;
    func(int& i_): i(i_){}
    void operator()(){
        std::this_thread::sleep_for(std::chrono::seconds(1));
        std::cout << "my value is " << i << std::endl;
    }
};

int main()  {
    int i = 100;
    func f(i);
    scoped_thread st{std::thread(f)};
    for (int j = 0; j < 25; j++) {
        std::cout << j << std::endl;
    }

}