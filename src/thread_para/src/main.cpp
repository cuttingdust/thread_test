#include <iostream>
#include <thread>
#include <print>

void ThreadMain(float param_first, const std::string &param_second)
{
    std::cout << "begin sub thread main " << std::this_thread::get_id() << std::endl;
    {
        std::print("param_first = {}, param_second = {};\n", param_first, param_second);
    }
    std::cout << "end sub thread main " << std::this_thread::get_id() << std::endl;
}

int main(int argc, char *argv[])
{
    std::cout << "main thread ID " << std::this_thread::get_id() << std::endl;

    {
        float       p1 = 12.12f;
        std::string p2 = "test thread param";
        std::thread th;
        {
            th = std::thread(ThreadMain, p1, p2);
        }
        th.join();
    }


    getchar();
    return 0;
}
