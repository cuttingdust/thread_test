#include "XMsgServer.h"

#include <list>
#include <mutex>

class XMsgServer::PImpl
{
public:
    PImpl(XMsgServer *owenr);
    ~PImpl() = default;

public:
    XMsgServer             *owenr_ = nullptr;
    std::mutex              mtx_;
    std::list<std::string>  msgs_;
    std::condition_variable cv_;
};

XMsgServer::PImpl::PImpl(XMsgServer *owenr) : owenr_(owenr)
{
}


XMsgServer::XMsgServer()
{
    impl_ = std::make_unique<XMsgServer::PImpl>(this);
}

XMsgServer::~XMsgServer() = default;

auto XMsgServer::run() -> void
{
    while (isRunning())
    {
        // std::this_thread::sleep_for(std::chrono::milliseconds(10));

        std::unique_lock<std::mutex> ul(impl_->mtx_);
        impl_->cv_.wait(ul,
                        [this]() -> bool
                        {
                            std::cout << "wait cv" << std::endl;
                            if (!isRunning())
                                return true;
                            return !impl_->msgs_.empty();
                        });
        while (!impl_->msgs_.empty())
        {
            /// 消息处理业务逻辑
            std::cout << "recv : " << impl_->msgs_.front() << std::endl;
            impl_->msgs_.pop_front();
        }
    }
}

auto XMsgServer::sendMsg(const std::string &msg) -> void
{
    std::unique_lock<std::mutex> ul(impl_->mtx_);
    impl_->msgs_.emplace_back(msg);
    ul.unlock();
    impl_->cv_.notify_one();
}
