#include <memory>
#include <thread>
#include <iostream>
class SomeObject {
    public:
    int a;
    int b;
    SomeObject (int a_, int b_): a(a_), b(b_){}
};
void func(std::unique_ptr<SomeObject> p) {
    std::cout << p->a << " " << p->b << std::endl;
}
int main() {
    std::unique_ptr<SomeObject> p(new SomeObject(10, 20));
    // required to use move here, otherwise it doesn't compile
    std::thread t(func, std::move(p));
    t.join();
}