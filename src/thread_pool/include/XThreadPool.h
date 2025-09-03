/**
 * @file   XThreadPool.h
 * @brief  
 *
 * @details   
 *
 * @author 31667
 * @date   2025-09-03
 */

#ifndef XTHREADPOOL_H
#define XTHREADPOOL_H

#include <memory>

class XTask
{
public:
    virtual auto run() -> void = 0;
};

class XThreadPool
{
public:
    XThreadPool();
    virtual ~XThreadPool();

public:
    /// \brief 初始化线程池
    /// \param num 线程数量
    auto init(int num) -> void;

    /// \brief 启动所有线程，必须先调用Init
    auto start() -> void;

    auto addTask(XTask *task) -> void;

    auto getTask() const -> XTask *;

private:
    class PImpl;
    std::unique_ptr<PImpl> impl_;
};


#endif // XTHREADPOOL_H
