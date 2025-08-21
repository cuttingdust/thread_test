#include <iostream>
#include <shared_mutex>
#include <thread>
#include <print>

static std::mutex mtx_one;
static std::mutex mtx_two;


void TestScope1()
{
    /// 模拟死锁 停100ms等另一个线程锁mux2
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    std::cout << std::this_thread::get_id() << " begin mux1 lock" << std::endl;
    // mtx_one.lock();
    std::cout << std::this_thread::get_id() << " begin mux2 lock" << std::endl;
    // mtx_two.lock(); /// 死锁
    /// c++11
    // std::lock(mtx_one, mtx_two);
    /// c++17
    std::scoped_lock lock(mtx_one, mtx_two); // 解决死锁

    std::cout << "TestScope1" << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));

    // mtx_one.unlock();
    // mtx_two.unlock();
}

void TestScope2()
{
    std::cout << std::this_thread::get_id() << " begin mux2 lock" << std::endl;
    mtx_two.lock();
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    std::cout << std::this_thread::get_id() << " begin mux1 lock" << std::endl;
    mtx_one.lock(); /// 死锁
    std::cout << "TestScope2" << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(1500));
    mtx_one.unlock();
    mtx_two.unlock();
}

int main(int argc, char *argv[])
{
    std::cout << "main thread ID " << std::this_thread::get_id() << std::endl;
    {
        /// 演示死锁情况
        {
            std::thread(TestScope1).detach();
        }
        {
            std::thread(TestScope2).detach();
        }
    }
    getchar();
    return 0;
}
