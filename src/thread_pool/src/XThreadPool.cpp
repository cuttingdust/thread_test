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

    int                                       thread_num_     = 0;     ///< 线程数量
    bool                                      is_running_     = false; ///< 是否正在运行
    std::atomic<int>                          task_run_count_ = { 0 }; ///< 正在运行的任务数量
    std::mutex                                mtx_;
    std::vector<std::shared_ptr<std::thread>> threads_;

    std::list<std::shared_ptr<XTask>> tasks_;
    std::condition_variable           cv_;
};

XThreadPool::PImpl::PImpl(XThreadPool *owenr) : owenr_(owenr)
{
}

auto XThreadPool::PImpl::run() -> void
{
    std::cout << "begin XThreadPool Run " << std::this_thread::get_id() << std::endl;
    while (is_running_)
    {
        auto task = owenr_->getTask();
        if (!task)
            continue;
        ++task_run_count_;
        try
        {
            task->run();
        }
        catch (...)
        {
        }
        --task_run_count_;
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
        impl_->is_running_ = true;
        for (int i = 0; i < impl_->thread_num_; i++)
        {
            /// 启动线程
            // auto th = new std::thread(&XThreadPool::PImpl::run, impl_.get());
            auto th = std::make_shared<std::thread>(&XThreadPool::PImpl::run, impl_.get());
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
            impl_->threads_.push_back(th);
        }
    }
}

auto XThreadPool::stop() -> void
{
    impl_->is_running_ = false;
    impl_->cv_.notify_all();
    for (auto &th : impl_->threads_)
    {
        th->join();
    }
    std::unique_lock<std::mutex> lock(impl_->mtx_);
    impl_->threads_.clear();
}

auto XThreadPool::addTask(const XTask::Ptr task) -> void
{
    std::unique_lock<std::mutex> lock(impl_->mtx_);
    impl_->tasks_.push_back(task);
    task->isRunning = [this] { return isRunning(); };

    lock.unlock();
    impl_->cv_.notify_one();
}

auto XThreadPool::getTask() const -> XTask::Ptr
{
    std::unique_lock<std::mutex> lock(impl_->mtx_);
    if (impl_->tasks_.empty())
    {
        impl_->cv_.wait(lock);
    }
    if (!isRunning())
        return nullptr;

    if (impl_->tasks_.empty())
        return nullptr;
    auto task = impl_->tasks_.front();
    impl_->tasks_.pop_front();
    return task;
}

auto XThreadPool::isRunning() const -> bool
{
    return impl_->is_running_;
}

auto XThreadPool::getRunTaskCnt() const -> int
{
    return impl_->task_run_count_.load();
}
