#include <algorithm>
#include <chrono>
#include <iostream>
#include <thread>
#include <print>
#include <vector>
#include <execution>

/// 性能比较

static const char base16[] = "0123456789abcdef";

void Base16Encode(const unsigned char* data, int size, unsigned char* out)
{
    for (int i = 0; i < size; i++)
    {
        unsigned char d = data[i];
        /// 0000 0000
        /// 1234 5678 >>4 0000 1234 //取 高四位
        /// 1234 5678 & 0000 1111   0000 5678 //  取 低四位
        char a         = base16[d >> 4];
        char b         = base16[d & 0x0F];
        out[i * 2]     = a;
        out[i * 2 + 1] = b;
    }
}

/// C++11 多核base16编码
void Base16EncodeThread(const std::vector<unsigned char>& data, std::vector<unsigned char>& out)
{
    int size     = data.size();
    int th_count = std::thread::hardware_concurrency(); /// 系统支持的线程核心数
    /// 切片数据
    int slice_count = size / th_count; /// 余数丢弃
    if (size < th_count)               /// 只切一片
    {
        th_count    = 1;
        slice_count = size;
    }

    /// 准备好线程
    std::vector<std::thread> ths;
    ths.resize(th_count);

    /// 任务分配到各个线程
    for (int i = 0; i < th_count; i++)
    {
        /// 1234 5678 9abc defg hi
        int offset = i * slice_count;
        int count  = slice_count;

        /// 最后一个线程
        if (th_count > 1 && i == th_count - 1)
        {
            count = slice_count + size % th_count;
        }
        //cout << offset << ":" << count << endl;
        ths[i] = std::thread(Base16Encode, data.data() + offset, count, out.data());
    }
    /// 等待所有线程处理结束
    for (auto& th : ths)
    {
        th.join();
    }
}

int main(int argc, char* argv[])
{
    std::cout << "main thread ID " << std::this_thread::get_id() << std::endl;
    std::string   test_data = "测试base16编码";
    unsigned char out[1024] = { 0 };
    Base16Encode(reinterpret_cast<unsigned char*>(test_data.data()), test_data.size(), out);
    std::cout << "base16:" << out << std::endl;

    /// 初始化测试数据
    std::vector<unsigned char> in_data;
    in_data.resize(1024 * 1024 * 1001); /// 20M.
    //in_data.data();
    for (int i = 0; i < in_data.size(); i++)
    {
        in_data[i] = i % 256;
    }
    std::vector<unsigned char> out_data;
    out_data.resize(in_data.size() * 2);

    /// 测试单线程base16编码效率
    {
        std::cout << "单线程base16开始计算" << std::endl;
        auto start = std::chrono::system_clock::now();
        Base16Encode(in_data.data(), in_data.size(), out_data.data());
        auto end      = std::chrono::system_clock::now();
        auto duration = duration_cast<std::chrono::milliseconds>(end - start);
        std::cout << "编码：" << in_data.size() << "字节数据花费" << static_cast<double>(duration.count()) << "毫秒"
                  << std::endl;

        // std::cout << out_data.data() << std::endl;
    }

    std::cout << "===================================================" << std::endl;
    /// 测试c++11 多线程Base16编码
    {
        /// 大数据情况下 这种最快

        std::cout << "c++11 多线程Base16编码 开始计算" << std::endl;
        auto start = std::chrono::system_clock::now();
        Base16EncodeThread(in_data, out_data);
        auto end      = std::chrono::system_clock::now();
        auto duration = duration_cast<std::chrono::milliseconds>(end - start);
        std::cout << "编码：" << in_data.size() << "字节数据花费" << static_cast<double>(duration.count()) << "毫秒"
                  << std::endl;

        // std::cout << out_data.data() << std::endl;
    }

    std::cout << "===================================================" << std::endl;

    /// 测试C++17 多线程base16编码
    {
        std::cout << "C++17 多线程base16编码 开始计算" << std::endl;
        auto           start = std::chrono::system_clock::now();
        unsigned char* idata = in_data.data();
        unsigned char* odata = out_data.data();
        //#include <execution> c++17

        std::for_each(std::execution::par, /// 并行计算 多核
                      in_data.begin(), in_data.end(),
                      [&](auto& d) /// 多线程进入此函数  /// 这个里面最大的耗时和麻烦就是频繁的进栈 出栈
                      {
                          char a               = base16[(d >> 4)];
                          char b               = base16[(d & 0x0F)];
                          int  index           = &d - idata;
                          odata[index * 2]     = a;
                          odata[index * 2 + 1] = b;
                      });

        auto end      = std::chrono::system_clock::now();
        auto duration = duration_cast<std::chrono::milliseconds>(end - start);
        std::cout << "编码：" << in_data.size() << "字节数据花费" << static_cast<double>(duration.count()) << "毫秒"
                  << std::endl;

        // std::cout << out_data.data() << std::endl;
    }
    std::cout << "===================================================" << std::endl;

    /// 测试omp 多线程base16编码
    {
        std::cout << "omp 多线程base16编码 开始计算" << std::endl;
        auto           start = std::chrono::system_clock::now();
        unsigned char* idata = in_data.data();
        unsigned char* odata = out_data.data();
        /// omp 多线程
        /// omp 从测试看出 比 std::for_each 要慢

#pragma omp parallel for /// 多线程进入此函数  /// 这个里面最大的耗时和麻烦就是频繁的进栈 出栈
        for (int i = 0; i < in_data.size(); ++i)
        {
            unsigned char d     = in_data[i];         /// 读取输入数据
            char          a     = base16[(d >> 4)];   /// 高四位编码
            char          b     = base16[(d & 0x0F)]; /// 低四位编码
            out_data[i * 2]     = a;                  /// 存储编码结果
            out_data[i * 2 + 1] = b;
        }

        auto end      = std::chrono::system_clock::now();
        auto duration = duration_cast<std::chrono::milliseconds>(end - start);
        std::cout << "编码：" << in_data.size() << "字节数据花费" << static_cast<double>(duration.count()) << "毫秒"
                  << std::endl;

        // std::cout << out_data.data() << std::endl;
    }
    std::cout << "===================================================" << std::endl;

    getchar();

    return 0;
}
