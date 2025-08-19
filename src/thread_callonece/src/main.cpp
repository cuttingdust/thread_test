#include <iostream>
#include <thread>
#include <print>

void SystemInit()
{
    std::cout << "Call SystemInit" << std::endl;
}

void SystemInitOne()
{
    static std::once_flag flag;
    std::call_once(flag, SystemInit);
}

int main(int argc, char *argv[])
{
    std::cout << "main thread ID " << std::this_thread::get_id() << std::endl;

    SystemInitOne();
    SystemInitOne();
    for (int i = 0; i < 3; i++)
    {
        std::thread th(SystemInitOne);
        th.detach();
    }

    getchar();

    return 0;
}
