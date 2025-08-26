#include <iostream>
#include <list>
#include <mutex>
#include <sstream>
#include <thread>

std::mutex              g_mtx;
std::condition_variable g_cv;
std::list<std::string>  g_msgs_;

void ThreadWrite()
{
    for (int i = 0;; i++)
    {
        std::stringstream ss;
        ss << "Write msg " << i;
        {
            std::unique_lock<std::mutex> lock(g_mtx);
            g_msgs_.push_back(ss.str());
            lock.unlock();
            g_cv.notify_one(); /// 发送信号
            // g_cv.notify_all(); /// 相比而言 notify_one性能要比 notify_all 好
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
}

void ThreadRead(int i)
{
    for (;;)
    {
        std::cout << "read msg" << std::endl;
        std::unique_lock<std::mutex> ul(g_mtx);
        // g_cv.wait(ul); /// 解锁、阻塞等待信号
        g_cv.wait(ul,
                  [i]
                  {
                      std::cout << i << " wait" << std::endl;
                      return !g_msgs_.empty();
                  });
        /// 获取信号后锁定
        while (!g_msgs_.empty())
        {
            std::cout << i << " read " << g_msgs_.front() << std::endl;
            g_msgs_.pop_front();
        }
    }
}

int main(int argc, char *argv[])
{
    std::cout << "main thread ID " << std::this_thread::get_id() << std::endl;
    {
        std::thread(ThreadWrite).detach();

        for (int i = 0; i < 3; i++)
        {
            std::thread(ThreadRead, i + 1).detach();
        }
    }
    getchar();
    return 0;
}
