#include <future>
#include <iostream>

void TestFuture(std::promise<std::string> p)
{
    std::cout << "begin TestFuture" << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(3));
    std::cout << "begin set value" << std::endl;
    p.set_value("TestFuture value");
    std::this_thread::sleep_for(std::chrono::seconds(3));
    std::cout << "end TestFuture" << std::endl;
}

int main(int argc, char *argv[])
{
    /// 异步传输变量存储
    std::promise<std::string> p;
    auto                      future = p.get_future();
    std::thread(TestFuture, std::move(p)).detach();

    std::cout << "begin future.get()" << std::endl;
    std::cout << "future get() = " << future.get() << std::endl; /// 阻塞等待, p.set_value
    std::cout << "end future.get()" << std::endl;

    getchar();
    return 0;
}
