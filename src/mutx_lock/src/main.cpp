#include <iostream>
#include <mutex>
#include <thread>
#include <print>

static std::mutex mutx;

void TestThread()
{
    for (;;)
    {
        /// 获取锁资源，如果没有则阻塞等待
        // mutx.lock();
        // std::lock_guard<std::mutex> lg(mutx);
        if (!mutx.try_lock())
        {
            std::cout << "." << std::flush;
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            continue;
        }

        std::cout << "==============================" << std::endl;
        std::cout << "test 001 " << std::this_thread::get_id() <<  std::endl;
        std::cout << "test 002" << std::endl;
        std::cout << "test 003" << std::endl;
        std::cout << "==============================" << std::endl;
        mutx.unlock();
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
}

int main(int argc, char *argv[])
{
    std::cout << "main thread ID " << std::this_thread::get_id() << std::endl;

    for (int i = 0; i < 10; i++)
    {
        std::thread th(TestThread);
        th.detach();
    }

    getchar();
    return 0;
}
