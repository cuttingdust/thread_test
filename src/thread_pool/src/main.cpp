
#include "XThreadPool.h"

#include <iostream>
#include <thread>

class MyTask : public XTask
{
public:
    auto run() -> void override
    {
        std::cout << "================================================" << std::endl;
        std::cout << std::this_thread::get_id() << " Mytask " << name << std::endl;
        std::cout << "================================================" << std::endl;

        for (int i = 0; i < 10; i++)
        {
            if (!isRunning())
            {
                std::this_thread::sleep_for(std::chrono::milliseconds(500));
                break;
            }
                
            std::cout << "." << std::flush;
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
        }
    }

public:
    std::string name = "";
};


int main(int argc, char *argv[])
{
    XThreadPool pool;
    pool.init(16);
    pool.start();

    MyTask task1;
    task1.name = "test name 001";
    pool.addTask(&task1);
    std::this_thread::sleep_for(std::chrono::seconds(1));
    pool.stop();

    getchar();

    return 0;
}
