/**
 * @file   XVideoTask.h
 * @brief  
 *
 * @details   
 *
 * @author 31667
 * @date   2025-09-08
 */

#ifndef XVIDEOTASK_H
#define XVIDEOTASK_H

#include "XThreadPool.h"

class XVideoTask : public XTask
{
public:
    using Ptr = std::shared_ptr<XVideoTask>;
    static auto create() -> XVideoTask::Ptr
    {
        return std::make_shared<XVideoTask>();
    }

public:
    auto run() -> int override;

public:
    std::string in_path  = "";
    std::string out_path = "";
    int         width    = 0;
    int         height   = 0;
};


#endif // XVIDEOTASK_H
