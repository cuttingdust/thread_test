#include <iostream>
#include <mutex>
#include <shared_mutex>
#include <thread>
#include <print>

static std::shared_timed_mutex st_mutx;

void ThreadRead(int i)
{
    for (;;)
    {
        st_mutx.lock_shared();
        std::cout << i << " Read" << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        st_mutx.unlock_shared();
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
}

void ThreadWrite(int i)
{
    for (;;)
    {
        st_mutx.lock_shared();
        /// 读取数据
        st_mutx.unlock_shared();
        st_mutx.lock(); /// 互斥锁 写入
        std::cout << i << " Write" << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(300));
        st_mutx.unlock();
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
}


int main(int argc, char *argv[])
{
    std::cout << "main thread ID " << std::this_thread::get_id() << std::endl;


    for (int i = 0; i < 3; ++i)
    {
        std::thread(ThreadWrite, i).detach();
    }

    for (int i = 0; i < 3; ++i)
    {
        std::thread(ThreadRead, i).detach();
    }

    getchar();
    return 0;
}
