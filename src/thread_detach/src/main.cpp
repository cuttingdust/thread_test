#include <iostream>
#include <thread>

std::atomic_bool is_exit = false;

void ThreadMain()
{
    std::cout << "begin sub thread main " << std::this_thread::get_id() << std::endl;
    for (int i = 0; i < 10; i++)
    {
        if (is_exit)
            break;

        std::cout << "in thread " << i << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(200)); /// 1000ms
    }
    std::cout << "end sub thread main " << std::this_thread::get_id() << std::endl;
}

int main(int argc, char *argv[])
{
    std::cout << "main thread ID " << std::this_thread::get_id() << std::endl;

    {
        // std::thread th(ThreadMain); /// 出错，thread对象被销毁 子线程还在运行
    }


    {
        is_exit = false;
        std::thread th(ThreadMain);
        std::cout << "begin join sub thread  " << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(1000)); ///1000ms
        std::cout << "主线程阻塞，等待子线程退出" << std::endl;
        is_exit = true; /// 通知子线程退出
        th.join(); /// 主线程阻塞，等待子线程退出
        std::cout << "子线程已经退出！" << std::endl;
        std::cout << "end join sub thread  " << std::endl;
    }

    //////////////////////////////////////////////////////////////////
    {
        is_exit = false;
        std::thread th(ThreadMain);
        std::cout << "begin detach sub thread  " << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        th.detach(); /// 子线程与主线程分离 守护线程
        is_exit = true;
        std::cout << "end detach sub thread  " << std::endl;
        /// 坑 ：主线程退出后 子线程不一定退出
    }
    //////////////////////////////////////////////////////////////////

    getchar();
    return 0;
}
