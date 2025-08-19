#include <iostream>
#include <thread>
#include <print>

class Para
{
public:
    Para(const std::string &string) : string_(string)
    {
        std::cout << "Para Create." << std::endl;
    }

    Para(const Para &other)
    {
        std::cout << "Para Copy." << std::endl;
        string_ = other.string();
    }

    Para(const Para &&other)
    {
        std::cout << "Para Move." << std::endl;
    }

    ~Para()
    {
        std::cout << "Para Destroy!" << std::endl;
    }

public:
    auto string() const noexcept -> std::string
    {
        return string_;
    }

private:
    std::string string_;
};

void ThreadPtr(const Para *param_third)
{
    std::cout << "begin sub thread main " << std::this_thread::get_id() << std::endl;
    {
        std::print("param = {};\n", param_third->string());
    }
    std::cout << "end sub thread main " << std::this_thread::get_id() << std::endl;
}

void ThreadMainRef(Para &param_third)
{
    std::cout << "begin sub thread main " << std::this_thread::get_id() << std::endl;
    {
        std::print("param = {};\n", param_third.string());
    }
    std::cout << "end sub thread main " << std::this_thread::get_id() << std::endl;
}

int main(int argc, char *argv[])
{
    std::cout << "main thread ID " << std::this_thread::get_id() << std::endl;

    {
        Para        p3("test Para class");
        std::thread th;
        {
            /// 传递线程指针
            th = std::thread(ThreadPtr, &p3); /// 错误 ，线程访问的p空间会提前释放
        }
        th.detach();

        /// main thread ID 31072
        /// Para Create.
        /// Para Destroy!
        /// begin sub thread main 37544
        /// param = ;
        /// end sub thread main 37544
    }

    std::cout << "===================================================" << std::endl;

    {
        Para        p3("test Para class");
        std::thread th;
        {
            /// 传递线程指针
            th = std::thread(ThreadPtr, &p3); /// 错误 ，线程访问的p空间会提前释放
        }
        th.join();

        /// main thread ID 33884
        /// Para Create.
        /// begin sub thread main 37152
        /// param = test Para class;
        /// end sub thread main 37152
    }

    std::cout << "===================================================" << std::endl;

    {
        /// 传递引用
        Para        p("test ref");
        std::thread th(ThreadMainRef, std::ref(p));
        th.join();
    }
    std::cout << "===================================================" << std::endl;

    getchar();

    return 0;
}
