#include "XMsgServer.h"

#include <list>
#include <mutex>

class XMsgServer::PImpl
{
public:
    PImpl(XMsgServer *owenr);
    ~PImpl() = default;

public:
    XMsgServer            *owenr_ = nullptr;
    std::mutex             mtx_;
    std::list<std::string> msgs_;
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
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
        std::unique_lock<std::mutex> ul(impl_->mtx_);
        if (impl_->msgs_.empty())
            continue;
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
}
