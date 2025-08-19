#include <iostream>
#include <mutex>
#include <thread>
#include <print>

static std::mutex mutx;

void ThreadMainMux(int i)
{
    for (;;)
    {
        mutx.lock();
        std::cout << i << "[in]" << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        mutx.unlock();
        /// unlock(释放资源) 之后， 直接 lock， 没有时间切换到另一个线程，所有大概率会还是会自己获得
        std::this_thread::sleep_for(std::chrono::milliseconds(1)); /// 给时间切换
    }
}


int main(int argc, char *argv[])
{
    std::cout << "main thread ID " << std::this_thread::get_id() << std::endl;
    for (int i = 0; i < 3; ++i)
    {
        std::thread(ThreadMainMux, i).detach();
    }

    getchar();
    return 0;
}
