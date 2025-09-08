
#include "XThreadPool.h"
#include "XVideoTask.h"

#include <iostream>
#include <thread>

constexpr auto in_path  = "assert\\test.mp4";
constexpr auto out_path = "assert\\out.mp4";

class MyTask : public XTask
{
public:
    using Ptr = std::shared_ptr<MyTask>;
    static auto create() -> MyTask::Ptr
    {
        return std::make_shared<MyTask>();
    }

public:
    auto run() -> int override
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
        return 100;
    }

public:
    std::string name = "";
};


int main(int argc, char *argv[])
{
    XThreadPool pool;
    pool.init(16);
    pool.start();

    {
        // auto task1  = MyTask::create();
        // task1->name = "test name 001";
        // pool.addTask(task1);
        //
        // auto task2  = MyTask::create();
        // task2->name = "test name 002";
        // pool.addTask(task2);

        // auto task3  = MyTask::create();
        // task3->name = "test shared 003";
        // pool.addTask(task3);
        //
        // auto task4  = MyTask::create();
        // task4->name = "test shared 004";
        // pool.addTask(task4);

        auto videoTask      = XVideoTask::create();
        videoTask->in_path  = in_path;
        videoTask->out_path = out_path;
        videoTask->width    = 640;
        videoTask->height   = 480;
        pool.addTask(videoTask);

        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        std::cout << "task run  count = " << pool.getRunTaskCnt() << std::endl;

        // auto re = task4->getReturn();
        auto re = videoTask->getReturn();
        std::cout << "videoTask->GetReturn() = " << re << std::endl;

        std::this_thread::sleep_for(std::chrono::seconds(1));
        pool.stop();
        std::cout << "task run  count = " << pool.getRunTaskCnt() << std::endl;

        getchar();

        return 0;
    }
}
