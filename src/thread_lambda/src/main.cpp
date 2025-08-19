#include <iostream>
#include <thread>
#include <print>

/// test lambda thread
class TestLambda
{
public:
    void start()
    {
        std::thread th([this]() {
            std::cout << "name = " << name << std::endl; });
        th.join();
    }

    std::string name = "test lambda";
};


int main(int argc, char *argv[])
{
    std::cout << "main thread ID " << std::this_thread::get_id() << std::endl;

    std::thread th([](int i) { std::cout << "test lmbda " << i << std::endl; }, 123);
    th.join();

    TestLambda test;
    test.start();

    getchar();
    return 0;
}
