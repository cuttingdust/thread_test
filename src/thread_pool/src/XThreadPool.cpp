#include "XThreadPool.h"

#include <iostream>
#include <vector>
#include <list>
#include <thread>
#include <mutex>


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

    std::list<XTask *>      tasks_;
    std::condition_variable cv_;
};

XThreadPool::PImpl::PImpl(XThreadPool *owenr) : owenr_(owenr)
{
}

auto XThreadPool::PImpl::run() -> void
{
    std::cout << "begin XThreadPool Run " << std::this_thread::get_id() << std::endl;
    while (true)
    {
        auto task = owenr_->getTask();
        if (!task)
            continue;
        try
        {
            task->run();
        }
        catch (...)
        {
        }
    }
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

auto XThreadPool::addTask(XTask *task) -> void
{
    std::unique_lock<std::mutex> lock(impl_->mtx_);
    impl_->tasks_.push_back(task);
    lock.unlock();
    impl_->cv_.notify_one();
}

auto XThreadPool::getTask() const -> XTask *
{
    std::unique_lock<std::mutex> lock(impl_->mtx_);
    if (impl_->tasks_.empty())
    {
        impl_->cv_.wait(lock);
    }
    if (impl_->tasks_.empty())
        return nullptr;
    auto task = impl_->tasks_.front();
    impl_->tasks_.pop_front();
    return task;
}
