#include <iostream>
#include <mutex>
#include <thread>
#include <print>

static std::mutex mtx;

class XMutex
{
public:
    XMutex(std::mutex &mutex) : mtx_(mutex)
    {
        mtx_.lock();
        std::cout << "Thread ID " << std::this_thread::get_id() << " locked mutex" << std::endl;
    }

    virtual ~XMutex()
    {
        mtx_.unlock();
        std::cout << "Thread ID " << std::this_thread::get_id() << " unlocked mutex" << std::endl;
    }

private:
    std::mutex &mtx_; // NOLINT(cppcoreguidelines-avoid-const-or-ref-data-members)
};

void TestMutex(int status)
{
    XMutex lock(mtx);

    if (status == 1)
    {
        std::cout << "=1" << std::endl;
        return;
    }
    else
    {
        std::cout << "!=1" << std::endl;
        return;
    }
}

int main(int argc, char *argv[])
{
    std::cout << "main thread ID " << std::this_thread::get_id() << std::endl;
    TestMutex(1);
    TestMutex(2);

    getchar();
    return 0;
}
