#include <future>
#include <iostream>

std::string TestAsync(int index)
{
    std::cout << index << " begin in TestAsync " << std::this_thread::get_id() << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(2));
    return "TestAsync string return";
}

int main(int argc, char *argv[])
{
    /// 创建异步线程

    std::cout << "main thread ID " << std::this_thread::get_id() << std::endl;
    {
        /// 不创建线程启动异步任务
        auto future = std::async(std::launch::deferred, TestAsync, 100); /// 调用者的线程中运行（同步执行）。
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        std::cout << "begin future get " << std::endl;
        std::cout << "future.get() = " << future.get() << std::endl;
        std::cout << "end future get" << std::endl;
    }
    {
        std::cout << "=====创建异步线程====" << std::endl;
        auto future2 = std::async(TestAsync, 101); /// std::thread(FUNC).detch();
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        std::cout << "begin future2 get " << std::endl;
        std::cout << "future2.get() = " << future2.get() << std::endl;
        std::cout << "end future2 get" << std::endl;
    }
    {
        std::cout << "=====可选创建异步线程====" << std::endl;
        /// 默认就是这样 但是启动的线程id 和 future2 一样
        auto future3 = std::async(std::launch::async | std::launch::deferred, TestAsync, 101);
        if (future3.wait_for(std::chrono::milliseconds(100)) != std::future_status::ready)
        {
            std::cout << "wait" << std::endl;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        std::cout << "begin future3 get " << std::endl;
        if (future3.wait_for(std::chrono::milliseconds(1800)) == std::future_status::timeout)
        {
            std::cout << "wait" << std::endl;
        }
        else
        {
            std::cout << "future3.get() = " << future3.get() << std::endl;
        }

        std::cout << "end future3 get" << std::endl;
    }
    {
        std::future<int> future = std::async(std::launch::async,
                                             []()
                                             {
                                                 std::this_thread::sleep_for(std::chrono::seconds(3));
                                                 return 8;
                                             });

        std::cout << "waiting...\n";
        std::future_status status;
        do
        {
            status = future.wait_for(std::chrono::seconds(1));
            if (status == std::future_status::deferred)
            {
                std::cout << "deferred\n";
            }
            else if (status == std::future_status::timeout)
            {
                std::cout << "timeout\n";
            }
            else if (status == std::future_status::ready)
            {
                std::cout << "ready!\n";
            }
        }
        while (status != std::future_status::ready);

        std::cout << "result is " << future.get() << '\n';
    }
    getchar();
    return 0;
}
