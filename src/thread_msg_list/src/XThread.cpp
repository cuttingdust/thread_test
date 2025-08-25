#include "XThread.h"

class XThread::PImpl
{
public:
    PImpl(XThread *owenr);
    ~PImpl() = default;

public:
    auto exec() -> void;

public:
    XThread         *owenr_ = nullptr;
    std::thread      th_;
    std::thread::id  th_id_;
    std::atomic_bool is_running_ = false;
};

XThread::PImpl::PImpl(XThread *owenr) : owenr_(owenr)
{
}

auto XThread::PImpl::exec() -> void
{
    th_id_ = std::this_thread::get_id();
    owenr_->run();
    is_running_ = false;
}


XThread::XThread()
{
    impl_ = std::make_unique<XThread::PImpl>(this);
    std::cout << "XThread Create." << std::endl;
}

XThread::~XThread()
{
    std::cout << "XThread Destroy!" << std::endl;
    XThread::wait();
}

auto XThread::start() -> void
{
    impl_->is_running_ = true;
    impl_->th_         = std::thread([this]() { impl_->exec(); }); /// 使用 lambda 表达式启动线程
}

auto XThread::stop() -> void
{
    impl_->is_running_ = false;
    XThread::wait();
}

auto XThread::wait() -> void
{
    if (impl_->th_.joinable())
        impl_->th_.join();
}

auto XThread::isRunning() const -> bool
{
    return impl_->is_running_;
}

auto XThread::getThreadID() -> std::thread::id
{
    return impl_->th_id_;
}
