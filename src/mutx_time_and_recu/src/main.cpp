#include <iostream>
#include <mutex>
#include <thread>
#include <print>

static std::timed_mutex     tmutx;
static std::recursive_mutex rmutx;

void ThreadMainTime(int i)
{
    for (;;)
    {
        // mutx.lock();
        if (!tmutx.try_lock_for(std::chrono::milliseconds(500)))
        {
            /// 这样好调试 能够知道哪里死锁了
            std::cout << i << "[try_lock_for timeout]" << std::endl;
            continue;
        }

        std::cout << i << "[in]" << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(2000));
        // std::this_thread::sleep_for(std::chrono::milliseconds(200));
        tmutx.unlock();
        /// unlock(释放资源) 之后， 直接 lock， 没有时间切换到另一个线程，所有大概率会还是会自己获得
        std::this_thread::sleep_for(std::chrono::milliseconds(1)); /// 给时间切换
    }
}

void Task1()
{
    rmutx.lock();
    std::cout << "task1 [in]" << std::endl;
    rmutx.unlock();
}
void Task2()
{
    rmutx.lock();
    std::cout << "task2 [in]" << std::endl;
    rmutx.unlock();
}

void ThreadMainRec(int i)
{
    for (;;)
    {
        rmutx.lock();
        Task1();
        std::cout << i << "[in]" << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(2000));
        Task2();
        rmutx.unlock();
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
}

int main(int argc, char *argv[])
{
    std::cout << "main thread ID " << std::this_thread::get_id() << std::endl;

    for (int i = 0; i < 3; i++)
    {
        std::thread th(ThreadMainRec, i + 1);
        th.detach();
    }

    getchar();
    //////////////////////////////////////////////////////////////////

    for (int i = 0; i < 3; ++i)
    {
        std::thread(ThreadMainTime, i).detach();
    }

    getchar();
    return 0;
}
