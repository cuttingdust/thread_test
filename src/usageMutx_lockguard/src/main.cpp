#include <iostream>
#include <mutex>
#include <thread>
#include <print>

static std::mutex mtx;

void TestLockGuard(int i)
{
    mtx.lock();
    {
        ///已经拥有锁，不lock
        std::lock_guard<std::mutex> lock(mtx, std::adopt_lock);
        /// 结束释放锁
    }

    {
        std::lock_guard<std::mutex> lg(mtx);
        std::cout << "begin thread " << i << std::endl;
    }

    for (;;)
    {
        {
            std::lock_guard<std::mutex> lg(mtx);
            std::cout << "In " << i << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
}

int main(int argc, char *argv[])
{
    std::cout << "main thread ID " << std::this_thread::get_id() << std::endl;

    for (int i = 0; i < 3; ++i)
    {
        std::thread(TestLockGuard, i).detach();
    }


    getchar();
    return 0;
}
