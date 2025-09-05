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
#include <functional>

class XTask
{
public:
    using Ptr = std::shared_ptr<XTask>;

public:
    virtual auto          run() -> void = 0;
    std::function<bool()> isRunning     = nullptr;
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

    /// \brief 停止所有线程
    auto stop() -> void;

    /// \brief 添加任务
    /// \param task
    auto addTask(const XTask::Ptr task) -> void;

    /// \brief 获得任务
    /// \return
    auto getTask() const -> XTask::Ptr;

    /// \brief 是否正在运行
    /// \return
    auto isRunning() const -> bool;

    /// \brief 获得正在运行的任务数量
    /// \return
    auto getRunTaskCnt() const -> int;

private:
    class PImpl;
    std::unique_ptr<PImpl> impl_;
};


#endif // XTHREADPOOL_H
