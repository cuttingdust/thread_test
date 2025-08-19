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

void ThreadMain(float param_first, const std::string &param_second, const Para &param_third)
{
    std::cout << "begin sub thread main " << std::this_thread::get_id() << std::endl;
    {
        std::print("param_first = {}, param_second = {}, param_third = {};\n", param_first, param_second,
                   param_third.string());
    }
    std::cout << "end sub thread main " << std::this_thread::get_id() << std::endl;
}

int main(int argc, char *argv[])
{
    //////////////////////////////////////////////////////////////////
    /// print
    /// main thread ID 36012
    /// Para Create.
    /// Para Copy.
    /// begin sub thread main 33592
    /// param_first = 12.12, param_second = test thread param, param_third = test Para class;
    /// end sub thread main 33592
    /// Para Destroy!
    /// Para Destroy!
    //////////////////////////////////////////////////////////////////


    std::cout << "main thread ID " << std::this_thread::get_id() << std::endl;

    {
        float       p1 = 12.12f;
        std::string p2 = "test thread param";
        Para        p3("test Para class");
        std::thread th;
        {
            /// 所有的参数做复制
            th = std::thread(ThreadMain, p1, p2, p3);
        }
        th.join();
    }


    getchar();
    return 0;
}
