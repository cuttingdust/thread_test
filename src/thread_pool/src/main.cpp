
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


    getchar();

    return 0;
}
