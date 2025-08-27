#include <future>
#include <iostream>


std::string TestPack(int index)
{
    std::cout << "begin Test Pack " << index << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(2));
    return "Test Pack return";
}

int main(int argc, char *argv[])
{
    std::cout << "main thread ID " << std::this_thread::get_id() << std::endl;
    {
        std::packaged_task<std::string(int)> task(TestPack);
        auto                                 result = task.get_future();
        // task(100);
        std::thread(std::move(task), 101).detach();

        std::cout << "begin result get" << std::endl;
        std::cout << "result get " << result.get() << std::endl;
    }
    getchar();
    return 0;
}
