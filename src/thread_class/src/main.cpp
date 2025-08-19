#include <iostream>
#include <thread>
#include <print>

class XThread
{
public:
    XThread()
    {
        std::cout << "XThread Create." << std::endl;
    }
    virtual ~XThread()
    {
        std::cout << "XThread Destroy!" << std::endl;
        XThread::wait();
    }

public:
    virtual void start()
    {
        is_running_ = true;
        th_         = std::thread(&XThread::exec, this);
    }

    virtual void stop()
    {
        is_running_ = false;
        wait();
    }

    virtual void wait()
    {
        if (th_.joinable())
            th_.join();
    }

    bool isRunning()
    {
        return is_running_;
    }

    std::thread::id getId() const noexcept
    {
        return th_id_;
    }

protected:
    virtual void run() = 0;

private:
    void exec()
    {
        th_id_ = std::this_thread::get_id();
        run();
        is_running_ = false;
    }

private:
    std::thread      th_;
    std::thread::id  th_id_;
    std::atomic_bool is_running_ = false;
};

class TestXThread : public XThread
{
protected:
    void run() override
    {
        std::cout << "TestXThread run begin" << std::endl;
        while (isRunning())
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            std::cout << "." << std::flush;
        }
        std::cout << std::endl;
        std::cout << "TestXThread run end" << std::endl;
    }

public:
    std::string name;
};


class MyThread
{
public:
    /// 入口线程函数
    void Main()
    {
        std::cout << "[MyThread run] " << name << ":" << age;
    }

    std::string name;
    int         age = 100;
};


int main(int argc, char *argv[])
{
    std::cout << "main thread ID " << std::this_thread::get_id() << std::endl;

    TestXThread testth;
    testth.name = "TestXThread name ";
    testth.start();
    std::this_thread::sleep_for(std::chrono::seconds(3));
    testth.stop();
    std::cout << "sub thread ID " << testth.getId() << std::endl;

    MyThread myth;
    myth.name = "Test name 001";
    myth.age  = 20;
    std::thread th(&MyThread::Main, &myth);
    th.join();

    getchar();

    return 0;
}
