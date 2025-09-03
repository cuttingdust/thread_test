#include "XThreadPool.h"

#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include <__msvc_ostream.hpp>

class XThreadPool::PImpl
{
public:
    PImpl(XThreadPool *owenr);
    ~PImpl() = default;

public:
    auto run() -> void;

public:
    XThreadPool *owenr_ = nullptr;

    int                        thread_num_ = 0; /// 线程数量
    std::mutex                 mtx_;
    std::vector<std::thread *> threads_;
};

XThreadPool::PImpl::PImpl(XThreadPool *owenr) : owenr_(owenr)
{
}

auto XThreadPool::PImpl::run() -> void
{
    std::cout << "begin XThreadPool Run " << std::this_thread::get_id() << std::endl;

    std::cout << "end XThreadPool Run " << std::this_thread::get_id() << std::endl;
}

XThreadPool::XThreadPool()
{
    impl_ = std::make_unique<PImpl>(this);
}

XThreadPool::~XThreadPool() = default;

auto XThreadPool::init(int num) -> void
{
    std::unique_lock<std::mutex> lock(impl_->mtx_);
    impl_->thread_num_ = num;
    std::cout << "Thread pool Init " << num << std::endl;
}

auto XThreadPool::start() -> void
{
    if (impl_->thread_num_ <= 0)
    {
        std::cout << "Thread pool not init." << std::endl;
        return;
    }

    if (!impl_->threads_.empty())
    {
        std::cerr << "Thread pool has start!" << std::endl;
        return;
    }

    {
        std::unique_lock<std::mutex> lock(impl_->mtx_);
        /// 启动线程
        for (int i = 0; i < impl_->thread_num_; i++)
        {
            auto th = new std::thread(&XThreadPool::PImpl::run, impl_.get());
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
            impl_->threads_.push_back(th);
        }
    }
}
