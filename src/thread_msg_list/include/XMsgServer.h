/**
 * @file   XMsgServer.h
 * @brief
 *
 * @details
 *
 * @author 31667
 * @date   2025-08-23
 */

#ifndef XMSGSERVER_H
#define XMSGSERVER_H

#include "XThread.h"

class XMsgServer : public XThread
{
public:
    XMsgServer();
    ~XMsgServer() override;

public:
    /// \brief 处理消息的线程入口函数
    auto run() -> void override;

    /// \brief 给当前线程发消息
    /// \param msg
    auto sendMsg(const std::string& msg) -> void;

private:
    class PImpl;
    std::unique_ptr<PImpl> impl_;
};


#endif // XMSGSERVER_H
