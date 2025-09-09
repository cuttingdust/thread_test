#include <iostream>
#include <thread>
#include <barrier>

void ThreadMain(int i, std::barrier<> *bar)
{
    std::cout << "begin sub thread main " << std::this_thread::get_id() << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(i));
    std::cout << i << " begin wait" << std::endl;
    // bar->wait(bar->arrive()); /// 期待数 -1 阻塞等待，期待为0是返回
    bar->arrive_and_wait();
    std::cout << i << " end wait" << std::endl;
    std::cout << "end sub thread main " << std::this_thread::get_id() << std::endl;
}

int main(int argc, char *argv[])
{
    {
        std::cout << "main thread ID " << std::this_thread::get_id() << std::endl;
        int          count = 3;
        std::barrier bar(count); /// 初始数量
        for (int i = 0; i < count; i++)
        {
            std::thread(ThreadMain, i, &bar).detach();
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }
        std::cout << "end wait sub thread  " << std::endl;
    }

    getchar();
    return 0;
}
