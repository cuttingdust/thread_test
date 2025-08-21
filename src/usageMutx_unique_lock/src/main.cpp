#include <iostream>
#include <mutex>
#include <thread>
#include <print>

static std::mutex mtx;

int main(int argc, char *argv[])
{
    std::cout << "main thread ID " << std::this_thread::get_id() << std::endl;
    {
        {
            std::unique_lock<std::mutex> ul(mtx);
            ul.unlock();
            /// 临时释放锁
            std::cout << "main thread temp unlock." << std::endl;
            ul.lock();
        }

        {
            mtx.lock();
            /// 已经拥有锁 不锁定，退出栈区解锁
            std::unique_lock<std::mutex> ul(mtx, std::adopt_lock);
        }

        {
            /// 延后加锁 不拥有 退出栈区不解锁
            std::unique_lock<std::mutex> ul(mtx, std::defer_lock);
            /// 加锁 退出栈区解锁
            ul.lock();
        }

        {
            /// mux.lock();
            /// 尝试加锁 不阻塞 失败不拥有锁
            std::unique_lock<std::mutex> ul(mtx, std::try_to_lock);

            if (ul.owns_lock())
            {
                std::cout << "owns_lock" << std::endl;
            }
            else
            {
                std::cout << "not owns_lock" << std::endl;
            }
        }
    }
    getchar();
    return 0;
}
