#include <iostream>
#include <shared_mutex>
#include <thread>
#include <print>

/// 共享锁
static std::shared_timed_mutex st_mtx;

int main(int argc, char *argv[])
{
    std::cout << "main thread ID " << std::this_thread::get_id() << std::endl;
    {
        {
            /// 读取锁 共享锁
            std::shared_lock<std::shared_timed_mutex> sl(st_mtx);
            std::cout << "read data" << std::endl;
            /// 退出栈区 释放共享锁
        }

        //////////////////////////////////////////////////////////////////

        {
            /// 写入锁 互斥锁
            std::unique_lock<std::shared_timed_mutex> ul(st_mtx);   
            std::cout << "write data" << std::endl;
        }
    }
    getchar();
    return 0;
}
