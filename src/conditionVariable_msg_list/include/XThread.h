/**
 * @file   XThread.h
 * @brief
 *
 * @details
 *
 * @author 31667
 * @date   2025-08-23
 */

#ifndef XTHREAD_H
#define XTHREAD_H

#include <memory>
#include <iostream>
#include <thread>

class XThread
{
public:
    XThread();
    virtual ~XThread();

public:
    auto start() -> void;

    auto stop() -> void;

    auto wait() -> void;

    auto isRunning() const -> bool;

    auto getThreadID() -> std::thread::id;

protected:
    virtual auto run() -> void = 0;

private:
    class PImpl;
    std::unique_ptr<PImpl> impl_;
};


#endif // XTHREAD_H
